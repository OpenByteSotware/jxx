#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/concurrent/jxx.util.concurrent.ForkJoinTask.h"
namespace jxx::util::concurrent {class RecursiveAction:public ForkJoinTask<::jxx::lang::Object>{public:using Super=ForkJoinTask<::jxx::lang::Object>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<RecursiveAction,Super>;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}~RecursiveAction()override=default;protected:RecursiveAction()=default;virtual void compute()=0;::jxx::Ptr<::jxx::lang::Object>computeTask()override{compute();return nullptr;}};}
