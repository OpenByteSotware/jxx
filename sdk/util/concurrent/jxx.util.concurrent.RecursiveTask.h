#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/concurrent/jxx.util.concurrent.ForkJoinTask.h"
namespace jxx::util::concurrent {template<typename V>class RecursiveTask:public ForkJoinTask<V>{public:using Super=ForkJoinTask<V>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<RecursiveTask<V>,Super>;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}~RecursiveTask()override=default;protected:RecursiveTask()=default;virtual ::jxx::Ptr<V>compute()=0;::jxx::Ptr<V>computeTask()override{return compute();}};}
