#include "util/jxx.util.concurrent.ForkJoinWorkerThread.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::util::concurrent {
::jxx::Ptr<::jxx::lang::ClassAny> ForkJoinWorkerThread::Class(){return JxxClassInfoMarker::Class();}
ForkJoinWorkerThread::ForkJoinWorkerThread(const ::jxx::Ptr<ForkJoinPool>& pool):JxxSuper(),pool_(pool){if(!pool_)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<ForkJoinPool> ForkJoinWorkerThread::getPool()const{return pool_;}
::jxx::lang::jint ForkJoinWorkerThread::getPoolIndex()const noexcept{return poolIndex_;}
void ForkJoinWorkerThread::onStart(){}
void ForkJoinWorkerThread::onTermination(const ::jxx::Ptr<::jxx::lang::Throwable>& exception){(void)exception;}
void ForkJoinWorkerThread::run(){onStart();try{onTermination(nullptr);}catch(...){onTermination(nullptr);throw;}}
}
