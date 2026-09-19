#pragma once
#include "util/jxx.util.concurrent.ForkJoinTask.h"
namespace jxx::util::concurrent {class RecursiveAction:public ForkJoinTask<::jxx::lang::Object>{public:using JxxSuper=ForkJoinTask<::jxx::lang::Object>;using Super=JxxSuper;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;~RecursiveAction()override=default;protected:RecursiveAction()=default;virtual void compute()=0;::jxx::Ptr<::jxx::lang::Object>computeTask()override{compute();return nullptr;}};}
