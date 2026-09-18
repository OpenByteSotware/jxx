#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Throwable.h"
#include "lang/jxx.lang.Void.h"
#include "util/function/jxx.util.function.BiConsumer.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.Supplier.h"
#include "util/jxx.util.concurrent.CancellationException.h"
#include "util/jxx.util.concurrent.CompletionException.h"
#include "util/jxx.util.concurrent.CompletionStage.h"
#include "util/jxx.util.concurrent.ExecutionException.h"
#include "util/jxx.util.concurrent.Future.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {

template<typename T>
class CompletableFuture final
    : public ::jxx::lang::ClassBase<CompletableFuture<T>,
          ::jxx::lang::Object, Future<T>, CompletionStage<T>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<CompletableFuture<T>,JxxSuper,Future<T>,CompletionStage<T>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class(){return JxxClassInfoMarker::Class();}

    CompletableFuture():Super(){}

    static ::jxx::Ptr<CompletableFuture<T>> completedFuture(const ::jxx::Ptr<T>& value){
        auto result=::jxx::NEW<CompletableFuture<T>>(); result->complete(value); return result;
    }
    static ::jxx::Ptr<CompletableFuture<T>> supplyAsync(
        const ::jxx::Ptr<::jxx::util::function::Supplier<T>>& supplier){
        if(!supplier)throw ::jxx::lang::NullPointerException();
        auto result=::jxx::NEW<CompletableFuture<T>>();
        std::thread([result,supplier]{try{result->complete(supplier->get());}catch(const ::jxx::lang::Throwable& e){result->completeExceptionally(e.cloneThrowable());}catch(...){result->completeExceptionally(::jxx::NEW<::jxx::lang::RuntimeException>());}}).detach();
        return result;
    }
    static ::jxx::Ptr<CompletableFuture<::jxx::lang::Void>> runAsync(
        const ::jxx::Ptr<::jxx::lang::Runnable>& action){
        if(!action)throw ::jxx::lang::NullPointerException();
        auto result=::jxx::NEW<CompletableFuture<::jxx::lang::Void>>();
        std::thread([result,action]{try{action->run();result->complete(nullptr);}catch(const ::jxx::lang::Throwable& e){result->completeExceptionally(e.cloneThrowable());}catch(...){result->completeExceptionally(::jxx::NEW<::jxx::lang::RuntimeException>());}}).detach();
        return result;
    }

    ::jxx::lang::jbool complete(const ::jxx::Ptr<T>& value){return finish_(value,nullptr,false);}
    ::jxx::lang::jbool completeExceptionally(const ::jxx::Ptr<::jxx::lang::Throwable>& failure){if(!failure)throw ::jxx::lang::NullPointerException();return finish_(nullptr,failure,false);}
    ::jxx::lang::jbool cancel(::jxx::lang::jbool) override {return finish_(nullptr,::jxx::NEW<CancellationException>(),true);}
    ::jxx::lang::jbool isCancelled() override {std::lock_guard<std::mutex>l(mutex_);return cancelled_;}
    ::jxx::lang::jbool isDone() override {std::lock_guard<std::mutex>l(mutex_);return done_;}
    ::jxx::lang::jbool isCompletedExceptionally(){std::lock_guard<std::mutex>l(mutex_);return done_&&failure_!=nullptr;}
    ::jxx::lang::jint getNumberOfDependents(){std::lock_guard<std::mutex>l(mutex_);return static_cast<::jxx::lang::jint>(callbacks_.size());}

    ::jxx::Ptr<T> get() override {std::unique_lock<std::mutex>l(mutex_);condition_.wait(l,[&]{return done_;});return report_(false);}
    ::jxx::Ptr<T> get(const ::jxx::lang::jlong timeout,const ::jxx::Ptr<TimeUnit>& unit) override {if(!unit)throw ::jxx::lang::NullPointerException();std::unique_lock<std::mutex>l(mutex_);if(!condition_.wait_for(l,unit->toChrono(timeout),[&]{return done_;}))throw TimeoutException();return report_(false);}
    ::jxx::Ptr<T> join(){std::unique_lock<std::mutex>l(mutex_);condition_.wait(l,[&]{return done_;});return report_(true);}
    ::jxx::Ptr<T> getNow(const ::jxx::Ptr<T>& fallback){std::lock_guard<std::mutex>l(mutex_);return done_?report_(true):fallback;}

    template<typename U>
    ::jxx::Ptr<CompletionStage<U>> thenApply(const ::jxx::Ptr<::jxx::util::function::Function<T,U>>& fn){
        if(!fn)throw ::jxx::lang::NullPointerException();auto next=::jxx::NEW<CompletableFuture<U>>();
        addCallback_([next,fn](const ::jxx::Ptr<T>& v,const ::jxx::Ptr<::jxx::lang::Throwable>& e){if(e)next->completeExceptionally(e);else try{next->complete(fn->apply(v));}catch(const ::jxx::lang::Throwable& x){next->completeExceptionally(x.cloneThrowable());}});return ::jxx::CAST<CompletionStage<U>>(next);
    }
    ::jxx::Ptr<CompletionStage<::jxx::lang::Void>> thenAccept(const ::jxx::Ptr<::jxx::util::function::Consumer<T>>& action){
        if(!action)throw ::jxx::lang::NullPointerException();auto next=::jxx::NEW<CompletableFuture<::jxx::lang::Void>>();addCallback_([next,action](const ::jxx::Ptr<T>&v,const ::jxx::Ptr<::jxx::lang::Throwable>&e){if(e)next->completeExceptionally(e);else try{action->accept(v);next->complete(nullptr);}catch(const ::jxx::lang::Throwable&x){next->completeExceptionally(x.cloneThrowable());}});return ::jxx::CAST<CompletionStage<::jxx::lang::Void>>(next);
    }
    ::jxx::Ptr<CompletionStage<::jxx::lang::Void>> thenRun(const ::jxx::Ptr<::jxx::lang::Runnable>& action){
        if(!action)throw ::jxx::lang::NullPointerException();auto next=::jxx::NEW<CompletableFuture<::jxx::lang::Void>>();addCallback_([next,action](const ::jxx::Ptr<T>&,const ::jxx::Ptr<::jxx::lang::Throwable>&e){if(e)next->completeExceptionally(e);else try{action->run();next->complete(nullptr);}catch(const ::jxx::lang::Throwable&x){next->completeExceptionally(x.cloneThrowable());}});return ::jxx::CAST<CompletionStage<::jxx::lang::Void>>(next);
    }
    ::jxx::Ptr<CompletionStage<T>> exceptionally(const ::jxx::Ptr<::jxx::util::function::Function<::jxx::lang::Throwable,T>>& fn){
        if(!fn)throw ::jxx::lang::NullPointerException();auto next=::jxx::NEW<CompletableFuture<T>>();addCallback_([next,fn](const ::jxx::Ptr<T>&v,const ::jxx::Ptr<::jxx::lang::Throwable>&e){if(!e)next->complete(v);else try{next->complete(fn->apply(e));}catch(const ::jxx::lang::Throwable&x){next->completeExceptionally(x.cloneThrowable());}});return ::jxx::CAST<CompletionStage<T>>(next);
    }
    template<typename U>
    ::jxx::Ptr<CompletionStage<U>> handle(const ::jxx::Ptr<::jxx::util::function::BiFunction<T,::jxx::lang::Throwable,U>>& fn){
        if(!fn)throw ::jxx::lang::NullPointerException();auto next=::jxx::NEW<CompletableFuture<U>>();addCallback_([next,fn](const ::jxx::Ptr<T>&v,const ::jxx::Ptr<::jxx::lang::Throwable>&e){try{next->complete(fn->apply(v,e));}catch(const ::jxx::lang::Throwable&x){next->completeExceptionally(x.cloneThrowable());}});return ::jxx::CAST<CompletionStage<U>>(next);
    }
    ::jxx::Ptr<CompletionStage<T>> whenComplete(const ::jxx::Ptr<::jxx::util::function::BiConsumer<T,::jxx::lang::Throwable>>& action){
        if(!action)throw ::jxx::lang::NullPointerException();auto next=::jxx::NEW<CompletableFuture<T>>();addCallback_([next,action](const ::jxx::Ptr<T>&v,const ::jxx::Ptr<::jxx::lang::Throwable>&e){try{action->accept(v,e);if(e)next->completeExceptionally(e);else next->complete(v);}catch(const ::jxx::lang::Throwable&x){next->completeExceptionally(x.cloneThrowable());}});return ::jxx::CAST<CompletionStage<T>>(next);
    }
    ::jxx::Ptr<CompletableFuture<T>> toCompletableFuture() override {return ::jxx::CAST<CompletableFuture<T>>(this->thisPtr());}

private:
    using Callback=std::function<void(const ::jxx::Ptr<T>&,const ::jxx::Ptr<::jxx::lang::Throwable>&)>;
    ::jxx::lang::jbool finish_(const ::jxx::Ptr<T>&value,const ::jxx::Ptr<::jxx::lang::Throwable>&failure,::jxx::lang::jbool cancelled){
        std::vector<Callback> callbacks;{std::lock_guard<std::mutex>l(mutex_);if(done_)return false;done_=true;value_=value;failure_=failure;cancelled_=cancelled;callbacks.swap(callbacks_);}condition_.notify_all();for(auto& cb:callbacks)cb(value,failure);return true;
    }
    void addCallback_(Callback callback){::jxx::Ptr<T>v;::jxx::Ptr<::jxx::lang::Throwable>e;{std::lock_guard<std::mutex>l(mutex_);if(!done_){callbacks_.push_back(std::move(callback));return;}v=value_;e=failure_;}callback(v,e);}
    ::jxx::Ptr<T> report_(::jxx::lang::jbool joinMode){if(cancelled_)throw CancellationException();if(failure_){if(joinMode)throw CompletionException(failure_);throw ExecutionException(failure_);}return value_;}
    mutable std::mutex mutex_;std::condition_variable condition_;::jxx::lang::jbool done_=false,cancelled_=false;::jxx::Ptr<T>value_;::jxx::Ptr<::jxx::lang::Throwable>failure_;std::vector<Callback>callbacks_;
};

// CompletionStage forwarding definitions.
template<typename T> template<typename U>
::jxx::Ptr<CompletionStage<U>> CompletionStage<T>::thenApply(const ::jxx::Ptr<::jxx::util::function::Function<T,U>>& f){return toCompletableFuture()->template thenApply<U>(f);}
template<typename T>::jxx::Ptr<CompletionStage<::jxx::lang::Void>> CompletionStage<T>::thenAccept(const ::jxx::Ptr<::jxx::util::function::Consumer<T>>&a){return toCompletableFuture()->thenAccept(a);}
template<typename T>::jxx::Ptr<CompletionStage<::jxx::lang::Void>> CompletionStage<T>::thenRun(const ::jxx::Ptr<::jxx::lang::Runnable>&a){return toCompletableFuture()->thenRun(a);}
template<typename T>::jxx::Ptr<CompletionStage<T>> CompletionStage<T>::exceptionally(const ::jxx::Ptr<::jxx::util::function::Function<::jxx::lang::Throwable,T>>&f){return toCompletableFuture()->exceptionally(f);}
template<typename T> template<typename U>
::jxx::Ptr<CompletionStage<U>> CompletionStage<T>::handle(const ::jxx::Ptr<::jxx::util::function::BiFunction<T,::jxx::lang::Throwable,U>>&f){return toCompletableFuture()->template handle<U>(f);}
template<typename T>::jxx::Ptr<CompletionStage<T>> CompletionStage<T>::whenComplete(const ::jxx::Ptr<::jxx::util::function::BiConsumer<T,::jxx::lang::Throwable>>&a){return toCompletableFuture()->whenComplete(a);}

} // namespace jxx::util::concurrent
