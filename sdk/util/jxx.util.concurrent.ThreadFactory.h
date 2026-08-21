#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
namespace jxx::util::concurrent {class ThreadFactory{public:virtual ~ThreadFactory()=default;virtual jxx::Ptr<jxx::lang::Thread> newThread(const jxx::Ptr<jxx::lang::Runnable>& runnable)=0;};}
