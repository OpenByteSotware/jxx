#pragma once
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::util::concurrent {
template <typename V> class Future {
public:
 virtual ~Future()=default;
 virtual jxx::lang::jbool cancel(jxx::lang::jbool mayInterruptIfRunning)=0;
 virtual jxx::lang::jbool isCancelled()=0;
 virtual jxx::lang::jbool isDone()=0;
 virtual jxx::Ptr<V> get()=0;
 virtual jxx::Ptr<V> get(const jxx::lang::jlong timeout,const jxx::Ptr<TimeUnit>& unit)=0;
};
}
