#pragma once
#include <atomic>
#include "util/jxx.util.concurrent.ForkJoinTask.h"
namespace jxx::util::concurrent {
template<typename T> class CountedCompleter:public ForkJoinTask<T>{
public:
 explicit CountedCompleter(const ::jxx::Ptr<CountedCompleter<T>>& completer=nullptr,::jxx::lang::jint initialPendingCount=0):completer_(completer),pending_(initialPendingCount){}
 ::jxx::Ptr<CountedCompleter<T>> getCompleter()const{return completer_;}
 ::jxx::lang::jint getPendingCount()const noexcept{return pending_.load();}
 void setPendingCount(::jxx::lang::jint count)noexcept{pending_.store(count);}
 void addToPendingCount(::jxx::lang::jint delta)noexcept{pending_.fetch_add(delta);}
 ::jxx::lang::jbool compareAndSetPendingCount(::jxx::lang::jint expected,::jxx::lang::jint count)noexcept{return pending_.compare_exchange_strong(expected,count);}
 ::jxx::lang::jint decrementPendingCountUnlessZero()noexcept{auto value=pending_.load();while(value!=0&&!pending_.compare_exchange_weak(value,value-1)){}return value;}
 void tryComplete(){auto current=::jxx::CAST<CountedCompleter<T>>(this->thisPtr());for(;;){auto count=current->getPendingCount();if(count==0){current->onCompletion(current);auto parent=current->getCompleter();if(!parent){current->quietlyComplete();return;}current=parent;}else if(current->compareAndSetPendingCount(count,count-1))return;}}
 void propagateCompletion(){auto current=::jxx::CAST<CountedCompleter<T>>(this->thisPtr());for(;;){auto count=current->getPendingCount();if(count==0){auto parent=current->getCompleter();if(!parent){current->quietlyComplete();return;}current=parent;}else if(current->compareAndSetPendingCount(count,count-1))return;}}
 virtual void onCompletion(const ::jxx::Ptr<CountedCompleter<T>>& caller){(void)caller;}
 virtual ::jxx::lang::jbool onExceptionalCompletion(const ::jxx::Ptr<::jxx::lang::Throwable>& exception,const ::jxx::Ptr<CountedCompleter<T>>& caller){(void)exception;(void)caller;return true;}
 ::jxx::Ptr<T> getRawResult(){return result_;}
protected:
 virtual void compute()=0;
 void setRawResult(const ::jxx::Ptr<T>& value){result_=value;}
 ::jxx::Ptr<T> computeTask()override{compute();return result_;}
private:
 ::jxx::Ptr<CountedCompleter<T>> completer_;
 std::atomic<::jxx::lang::jint> pending_{0};
 ::jxx::Ptr<T> result_;
};}
