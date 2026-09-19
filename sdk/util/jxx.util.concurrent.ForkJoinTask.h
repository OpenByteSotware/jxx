#pragma once
#include <chrono>
#include <future>
#include <mutex>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.concurrent.Future.h"
#include "util/jxx.util.concurrent.TimeoutException.h"
#include "util/jxx.util.concurrent.ForkJoinPool.h"
namespace jxx::util::concurrent {
template<typename V> class ForkJoinTask:public ::jxx::lang::ClassBase<ForkJoinTask<V>,::jxx::lang::Object,Future<V>,::jxx::io::SerializableI>{
public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<ForkJoinTask<V>,JxxSuper,Future<V>,::jxx::io::SerializableI>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}virtual ~ForkJoinTask()=default;
 ::jxx::Ptr<ForkJoinTask<V>> fork(){std::lock_guard<std::mutex>l(mutex_);if(!future_.valid()){auto self=::jxx::CAST<ForkJoinTask<V>>(this->thisPtr());future_=std::async(std::launch::async,[self]{return self->execute_();}).share();}return ::jxx::CAST<ForkJoinTask<V>>(this->thisPtr());}
 ::jxx::Ptr<V> join(){return ensureFuture_().get();}::jxx::Ptr<V> invoke(){return execute_();}::jxx::lang::jbool cancel(::jxx::lang::jbool)override{std::lock_guard<std::mutex>l(mutex_);if(done_)return false;cancelled_=true;done_=true;return true;}::jxx::lang::jbool isCancelled()override{std::lock_guard<std::mutex>l(mutex_);return cancelled_;}::jxx::lang::jbool isDone()override{std::lock_guard<std::mutex>l(mutex_);if(done_)return true;if(future_.valid()&&future_.wait_for(std::chrono::seconds(0))==std::future_status::ready)done_=true;return done_;}::jxx::Ptr<V>get()override{return join();}::jxx::Ptr<V>get(::jxx::lang::jlong timeout,const ::jxx::Ptr<TimeUnit>&unit)override{if(!unit)throw ::jxx::lang::NullPointerException();auto f=ensureFuture_();if(f.wait_for(unit->toChrono(timeout))!=std::future_status::ready)throw TimeoutException();return f.get();}
 ::jxx::Ptr<V> getRawResult(){std::lock_guard<std::mutex>l(mutex_);return result_;}void complete(const ::jxx::Ptr<V>&value){std::lock_guard<std::mutex>l(mutex_);result_=value;done_=true;}void quietlyComplete(){std::lock_guard<std::mutex>l(mutex_);done_=true;}void quietlyJoin(){(void)join();}void quietlyInvoke(){(void)invoke();}void reinitialize(){std::lock_guard<std::mutex>l(mutex_);future_={};result_.reset();done_=false;cancelled_=false;}
 static void invokeAll(const ::jxx::Ptr<ForkJoinTask<V>>&first,const ::jxx::Ptr<ForkJoinTask<V>>&second){if(!first||!second)throw ::jxx::lang::NullPointerException();first->fork();second->invoke();first->join();}
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o)override{(void)o;}void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i)override{(void)i;}void readObjectNoData()override{}
protected:ForkJoinTask():Super(){}virtual ::jxx::Ptr<V>computeTask()=0;
private: ::jxx::Ptr<V>execute_(){std::lock_guard<std::mutex>l(mutex_);if(cancelled_)throw ::jxx::lang::IllegalStateException();if(done_)return result_;result_=computeTask();done_=true;return result_;}std::shared_future<::jxx::Ptr<V>>ensureFuture_(){std::lock_guard<std::mutex>l(mutex_);if(!future_.valid()){auto self=::jxx::CAST<ForkJoinTask<V>>(this->thisPtr());future_=std::async(std::launch::async,[self]{return self->execute_();}).share();}return future_;}mutable std::mutex mutex_;std::shared_future<::jxx::Ptr<V>>future_;::jxx::Ptr<V>result_;::jxx::lang::jbool done_=false,cancelled_=false;};}
