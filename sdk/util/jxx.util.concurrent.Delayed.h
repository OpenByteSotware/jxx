#pragma once
#include "jxx_types.h"
#include "lang/jxx.lang.Comparable.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::util::concurrent {class Delayed:public virtual jxx::lang::Comparable<Delayed>{public:virtual ~Delayed()=default;virtual jxx::lang::jlong getDelay(const jxx::Ptr<TimeUnit>& unit)=0;};}
