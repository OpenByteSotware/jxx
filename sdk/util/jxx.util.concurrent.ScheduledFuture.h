#pragma once
#include "jxx_types.h"
#include "util/jxx.util.concurrent.Delayed.h"
#include "util/jxx.util.concurrent.Future.h"
namespace jxx::util::concurrent {template <typename V> class ScheduledFuture:public virtual Delayed,public virtual Future<V>{public:virtual ~ScheduledFuture()=default;};}
