#pragma once
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include <vector>
#include "beans/jxx.beans.PropertyChangeListener.h"
#include "beans/jxx.beans.PropertyChangeSupport.h"
#include "ext/swing/internal/jxx.ext.swing.internal.DispatchQueue.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.List.h"
#include "util/concurrent/jxx.util.concurrent.CancellationException.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.Future.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
#include "util/concurrent/jxx.util.concurrent.TimeoutException.h"
namespace jxx::ext::swing {
template <typename T, typename V>
class SwingWorker : public ::jxx::lang::ClassBase<SwingWorker<T,V>, ::jxx::lang::Object,
    ::jxx::lang::Runnable, ::jxx::util::concurrent::Future<T>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SwingWorker<T,V>, JxxSuper,
        ::jxx::lang::Runnable, ::jxx::util::concurrent::Future<T>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    enum class StateValue { PENDING, STARTED, DONE };
    SwingWorker() = default;
    ~SwingWorker() override = default;
    void execute() {
        ::jxx::Ptr<SwingWorker<T,V>> self;
        { std::lock_guard<std::mutex> lock(mutex_);
          if (state_ != StateValue::PENDING || started_) return;
          started_ = true; self = ::jxx::CAST<SwingWorker<T,V>>(this->thisPtr()); }
        setState_(StateValue::STARTED);
        worker_ = std::thread([self] { self->run(); });
        worker_.detach();
    }
    void run() override {
        { std::lock_guard<std::mutex> lock(mutex_);
          if (completed_) return; started_ = true; workerId_ = std::this_thread::get_id(); }
        setState_(StateValue::STARTED);
        try { finish_(doInBackground(), nullptr); }
        catch (...) { finish_(nullptr, std::current_exception()); }
    }
    ::jxx::lang::jbool cancel(::jxx::lang::jbool mayInterruptIfRunning) override {
        { std::lock_guard<std::mutex> lock(mutex_);
          if (completed_) return false; cancelled_ = true; completed_ = true;
          interruptRequested_ = mayInterruptIfRunning; }
        condition_.notify_all(); setState_(StateValue::DONE); dispatchDone_(); return true;
    }
    ::jxx::lang::jbool isCancelled() override { std::lock_guard<std::mutex> lock(mutex_); return cancelled_; }
    ::jxx::lang::jbool isDone() override { std::lock_guard<std::mutex> lock(mutex_); return completed_; }
    ::jxx::Ptr<T> get() override {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return completed_; }); return report_();
    }
    ::jxx::Ptr<T> get(::jxx::lang::jlong timeout,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        std::unique_lock<std::mutex> lock(mutex_);
        if (!completed_ && !condition_.wait_for(lock, unit->toChrono(timeout),
            [this] { return completed_; })) throw ::jxx::util::concurrent::TimeoutException();
        return report_();
    }
    ::jxx::lang::jint getProgress() const { std::lock_guard<std::mutex> lock(mutex_); return progress_; }
    StateValue getState() const { std::lock_guard<std::mutex> lock(mutex_); return state_; }
    void addPropertyChangeListener(const ::jxx::Ptr<::jxx::beans::PropertyChangeListener>& listener) {
        support_()->addPropertyChangeListener(listener);
    }
    void removePropertyChangeListener(const ::jxx::Ptr<::jxx::beans::PropertyChangeListener>& listener) {
        support_()->removePropertyChangeListener(listener);
    }
    ::jxx::Ptr<::jxx::beans::PropertyChangeSupport> getPropertyChangeSupport() { return support_(); }
protected:
    virtual ::jxx::Ptr<T> doInBackground() = 0;
    virtual void process(const ::jxx::Ptr<::jxx::util::List<V>>& chunks) { (void)chunks; }
    virtual void done() {}
    void publish(const std::vector<::jxx::Ptr<V>>& chunks) {
        if (chunks.empty()) return;
        ::jxx::Ptr<SwingWorker<T,V>> self = ::jxx::CAST<SwingWorker<T,V>>(this->thisPtr());
        { std::lock_guard<std::mutex> lock(publishMutex_);
          pending_.insert(pending_.end(), chunks.begin(), chunks.end());
          if (publishScheduled_) return; publishScheduled_ = true; }
        internal::DispatchQueue::instance().invokeLater([self] { self->flushPublished_(); });
    }
    template <typename... Values>
    void publish(const ::jxx::Ptr<V>& first, const Values&... remaining) {
        publish(std::vector<::jxx::Ptr<V>>{first, remaining...});
    }
    void setProgress(::jxx::lang::jint progress) {
        if (progress < 0 || progress > 100) throw ::jxx::lang::IllegalArgumentException();
        ::jxx::lang::jint oldValue;
        { std::lock_guard<std::mutex> lock(mutex_); oldValue = progress_; if (oldValue == progress) return; progress_ = progress; }
        firePropertyChange_("progress", ::jxx::NEW<::jxx::lang::Integer>(oldValue),
                                      ::jxx::NEW<::jxx::lang::Integer>(progress));
    }
    ::jxx::lang::jbool isCancellationRequested() const {
        std::lock_guard<std::mutex> lock(mutex_); return interruptRequested_;
    }
    void firePropertyChange(const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
        const ::jxx::Ptr<::jxx::lang::Object>& newValue) {
        support_()->firePropertyChange(name, oldValue, newValue);
    }
private:
    ::jxx::Ptr<::jxx::beans::PropertyChangeSupport> support_() {
        std::lock_guard<std::mutex> lock(supportMutex_);
        if (propertyChangeSupport_ == nullptr)
            propertyChangeSupport_ = ::jxx::NEW<::jxx::beans::PropertyChangeSupport>(
                ::jxx::CAST<::jxx::lang::Object>(this->thisPtr()));
        return propertyChangeSupport_;
    }
    void firePropertyChange_(const char* name, const ::jxx::Ptr<::jxx::lang::Object>& oldValue,
        const ::jxx::Ptr<::jxx::lang::Object>& newValue) {
        const auto property = ::jxx::NEW<::jxx::lang::String>(name);
        internal::DispatchQueue::instance().invokeLater([self = ::jxx::CAST<SwingWorker<T,V>>(this->thisPtr()), property, oldValue, newValue] {
            self->support_()->firePropertyChange(property, oldValue, newValue);
        });
    }
    void setState_(StateValue value) {
        StateValue old;
        { std::lock_guard<std::mutex> lock(mutex_); old = state_; if (old == value) return; state_ = value; }
        firePropertyChange_("state", nullptr, nullptr);
    }
    void flushPublished_() {
        std::vector<::jxx::Ptr<V>> values;
        { std::lock_guard<std::mutex> lock(publishMutex_); values.swap(pending_); publishScheduled_ = false; }
        auto list = ::jxx::NEW<::jxx::util::ArrayList<V>>();
        for (const auto& value : values) list->add(value);
        process(::jxx::CAST<::jxx::util::List<V>>(list));
    }
    void dispatchDone_() {
        auto self = ::jxx::CAST<SwingWorker<T,V>>(this->thisPtr());
        internal::DispatchQueue::instance().invokeLater([self] { self->done(); });
    }
    void finish_(const ::jxx::Ptr<T>& value, std::exception_ptr failure) {
        { std::lock_guard<std::mutex> lock(mutex_); if (completed_) return;
          result_ = value; failure_ = failure; completed_ = true; }
        condition_.notify_all(); setState_(StateValue::DONE); dispatchDone_();
    }
    ::jxx::Ptr<T> report_() {
        if (cancelled_) throw ::jxx::util::concurrent::CancellationException();
        if (failure_) { try { std::rethrow_exception(failure_); }
            catch (const ::jxx::lang::Throwable& failure) {
                throw ::jxx::util::concurrent::ExecutionException(failure.cloneThrowable()); }
            catch (...) { throw ::jxx::util::concurrent::ExecutionException(); } }
        return result_;
    }
    mutable std::mutex mutex_, supportMutex_, publishMutex_;
    std::condition_variable condition_;
    std::thread worker_;
    std::thread::id workerId_{};
    StateValue state_{StateValue::PENDING};
    ::jxx::lang::jint progress_{0};
    ::jxx::lang::jbool started_{false}, completed_{false}, cancelled_{false}, interruptRequested_{false};
    ::jxx::Ptr<T> result_;
    std::exception_ptr failure_;
    ::jxx::Ptr<::jxx::beans::PropertyChangeSupport> propertyChangeSupport_;
    std::vector<::jxx::Ptr<V>> pending_;
    bool publishScheduled_{false};
};
} // namespace jxx::ext::swing
