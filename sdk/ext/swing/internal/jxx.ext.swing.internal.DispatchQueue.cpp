#include "ext/swing/internal/jxx.ext.swing.internal.DispatchQueue.h"
#include <future>
namespace jxx::ext::swing::internal {
DispatchQueue& DispatchQueue::instance() { static DispatchQueue queue; return queue; }
DispatchQueue::DispatchQueue() : worker_([this] { loop_(); }) {}
DispatchQueue::~DispatchQueue() {
    { std::lock_guard<std::mutex> lock(mutex_); stopping_ = true; }
    condition_.notify_all();
    if (worker_.joinable()) worker_.join();
}
void DispatchQueue::invokeLater(std::function<void()> action) {
    { std::lock_guard<std::mutex> lock(mutex_); queue_.push(std::move(action)); }
    condition_.notify_one();
}
void DispatchQueue::invokeAndWait(std::function<void()> action) {
    if (isDispatchThread()) { action(); return; }
    auto completion = std::make_shared<std::promise<void>>();
    auto future = completion->get_future();
    invokeLater([action = std::move(action), completion] {
        try { action(); completion->set_value(); }
        catch (...) { completion->set_exception(std::current_exception()); }
    });
    future.get();
}
bool DispatchQueue::isDispatchThread() const noexcept {
    return std::this_thread::get_id() == workerId_;
}
void DispatchQueue::loop_() {
    workerId_ = std::this_thread::get_id();
    for (;;) {
        std::function<void()> action;
        { std::unique_lock<std::mutex> lock(mutex_);
          condition_.wait(lock, [this] { return stopping_ || !queue_.empty(); });
          if (stopping_ && queue_.empty()) return;
          action = std::move(queue_.front()); queue_.pop(); }
        try { action(); } catch (...) {}
    }
}
} // namespace jxx::ext::swing::internal
