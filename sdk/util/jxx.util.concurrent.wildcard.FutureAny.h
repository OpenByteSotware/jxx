#pragma once

#include "io/jxx.lang.Object.h"
#include "io/jxx.util.concurrent.TimeUnit.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {
namespace concurrent {
namespace wildcard {

class FutureAny : virtual public jxx::lang::Object {
public:
    virtual ~FutureAny() = default;

    virtual jxx::lang::jbool cancel(jxx::lang::jbool mayInterruptIfRunning) = 0;
    virtual jxx::lang::jbool isCancelled() = 0;
    virtual jxx::lang::jbool isDone() = 0;
    virtual jxx::Ptr<jxx::lang::Object> get() = 0;
    virtual jxx::Ptr<jxx::lang::Object> get(
        jxx::lang::jlong timeout,
        jxx::Ptr<jxx::util::concurrent::TimeUnit> unit) = 0;
};

} // namespace wildcard
} // namespace concurrent
} // namespace util
} // namespace jxx
