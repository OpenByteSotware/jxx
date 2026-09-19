#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.ForkJoinPool.h"
namespace jxx::util::concurrent {
class ForkJoinWorkerThread : public ::jxx::lang::Thread {
public:
 using JxxSuper=::jxx::lang::Thread;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<ForkJoinWorkerThread,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 ::jxx::Ptr<ForkJoinPool> getPool() const;
 ::jxx::lang::jint getPoolIndex() const noexcept;
 void run() override;
protected:
 explicit ForkJoinWorkerThread(const ::jxx::Ptr<ForkJoinPool>& pool);
 virtual void onStart();
 virtual void onTermination(const ::jxx::Ptr<::jxx::lang::Throwable>& exception);
private:
 ::jxx::Ptr<ForkJoinPool> pool_;
 ::jxx::lang::jint poolIndex_=0;
};}
