#pragma once
#include "jxx_types.h"
#include <condition_variable>
#include <mutex>
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::util::concurrent {class CountDownLatch final:public jxx::lang::Object{mutable std::mutex mutex_;std::condition_variable condition_;jxx::lang::jlong count_;public:explicit CountDownLatch(jxx::lang::jint count):count_(count){if(count<0)throw jxx::lang::IllegalArgumentException();}void await(){std::unique_lock<std::mutex> l(mutex_);condition_.wait(l,[&]{return count_==0;});}jxx::lang::jbool await(jxx::lang::jlong timeout,const jxx::Ptr<TimeUnit>& unit){if(unit==nullptr)throw jxx::lang::NullPointerException();std::unique_lock<std::mutex> l(mutex_);return condition_.wait_for(l,unit->toChrono(timeout),[&]{return count_==0;});}void countDown(){std::lock_guard<std::mutex> l(mutex_);if(count_>0&&--count_==0)condition_.notify_all();}jxx::lang::jlong getCount() const{std::lock_guard<std::mutex> l(mutex_);return count_;}protected:jxx::Ptr<jxx::lang::Object> cloneImpl() const override{throw jxx::lang::CloneNotSupportedException();}};}
