#pragma once
#include "lang/jxx_types.h"
#include <condition_variable>
#include <exception>
#include <mutex>
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.CancellationException.h"
#include "util/jxx.util.concurrent.ExecutionException.h"
#include "util/jxx.util.concurrent.RunnableFuture.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {
template <typename V> class FutureTask:public jxx::lang::Object,public virtual RunnableFuture<V>{
 enum class State{NEW,RUNNING,NORMAL,EXCEPTIONAL,CANCELLED};
 mutable std::mutex mutex_; std::condition_variable condition_; State state_=State::NEW; jxx::Ptr<Callable<V>> callable_; jxx::Ptr<V> result_; std::exception_ptr failure_;
public:
 explicit FutureTask(const jxx::Ptr<Callable<V>>& callable):callable_(callable){if(callable_==nullptr)throw jxx::lang::NullPointerException();}
 jxx::lang::jbool cancel(jxx::lang::jbool) override{std::lock_guard<std::mutex> l(mutex_);if(state_!=State::NEW)return false;state_=State::CANCELLED;condition_.notify_all();return true;}
 jxx::lang::jbool isCancelled() override{std::lock_guard<std::mutex> l(mutex_);return state_==State::CANCELLED;}
 jxx::lang::jbool isDone() override{std::lock_guard<std::mutex> l(mutex_);return state_==State::NORMAL||state_==State::EXCEPTIONAL||state_==State::CANCELLED;}
 void run() override{ {std::lock_guard<std::mutex> l(mutex_);if(state_!=State::NEW)return;state_=State::RUNNING;} try{auto r=callable_->call();{std::lock_guard<std::mutex> l(mutex_);result_=r;state_=State::NORMAL;}}catch(...){std::lock_guard<std::mutex> l(mutex_);failure_=std::current_exception();state_=State::EXCEPTIONAL;} condition_.notify_all();}
 jxx::Ptr<V> get() override{std::unique_lock<std::mutex> l(mutex_);condition_.wait(l,[&]{return state_==State::NORMAL||state_==State::EXCEPTIONAL||state_==State::CANCELLED;});return report_();}
 jxx::Ptr<V> get(jxx::lang::jlong timeout,const jxx::Ptr<TimeUnit>& unit) override{if(unit==nullptr)throw jxx::lang::NullPointerException();std::unique_lock<std::mutex> l(mutex_);if(!condition_.wait_for(l,unit->toChrono(timeout),[&]{return state_==State::NORMAL||state_==State::EXCEPTIONAL||state_==State::CANCELLED;}))throw TimeoutException();return report_();}
protected:jxx::Ptr<jxx::lang::Object> cloneImpl() const override{throw jxx::lang::CloneNotSupportedException();}
private:jxx::Ptr<V> report_(){if(state_==State::CANCELLED)throw CancellationException();if(state_==State::EXCEPTIONAL){try{std::rethrow_exception(failure_);}catch(const jxx::lang::Throwable& e){throw ExecutionException();}catch(...){throw ExecutionException();}}return result_;}
};}
