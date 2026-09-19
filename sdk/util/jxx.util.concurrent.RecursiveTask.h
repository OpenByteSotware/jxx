#pragma once
#include "util/jxx.util.concurrent.ForkJoinTask.h"
namespace jxx::util::concurrent {template<typename V>class RecursiveTask:public ForkJoinTask<V>{public:using JxxSuper=ForkJoinTask<V>;using Super=JxxSuper;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;~RecursiveTask()override=default;protected:RecursiveTask()=default;virtual ::jxx::Ptr<V>compute()=0;::jxx::Ptr<V>computeTask()override{return compute();}};}
