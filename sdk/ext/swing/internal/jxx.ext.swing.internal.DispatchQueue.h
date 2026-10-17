#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
namespace jxx::ext::swing::internal {
class DispatchQueue final {
public:
    static DispatchQueue& instance();
    void invokeLater(std::function<void()> action);
    void invokeAndWait(std::function<void()> action);
    bool isDispatchThread() const noexcept;
private:
    DispatchQueue();
    ~DispatchQueue();
    void loop_();
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<std::function<void()>> queue_;
    std::thread worker_;
    std::thread::id workerId_{};
    bool stopping_{false};
};
} // namespace jxx::ext::swing::internal
