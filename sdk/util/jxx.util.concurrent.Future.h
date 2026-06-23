#pragma once

#include "io/jxx.lang.Object.h"
#include "io/jxx.util.concurrent.TimeUnit.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {
namespace concurrent {

template <typename V>
class Future : virtual public jxx::lang::Object {
public:
    virtual ~Future() = default;

    virtual jxx::lang::jbool cancel(jxx::lang::jbool mayInterruptIfRunning) = 0;
    virtual jxx::lang::jbool isCancelled() = 0;
    virtual jxx::lang::jbool isDone() = 0;
    virtual jxx::Ptr<V> get() = 0;
    virtual jxx::Ptr<V> get(
        jxx::lang::jlong timeout,
        jxx::Ptr<TimeUnit> unit) = 0;
};

} // namespace concurrent
} // namespace util
} // namespace jxx
