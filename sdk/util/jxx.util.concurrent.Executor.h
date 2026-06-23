#pragma once

#include "io/jxx.lang.Object.h"
#include "io/jxx.lang.Runnable.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {
namespace concurrent {

class Executor : virtual public jxx::lang::Object {
public:
    virtual ~Executor() = default;

    virtual void execute(jxx::Ptr<jxx::lang::Runnable> command) = 0;
};

} // namespace concurrent
} // namespace util
} // namespace jxx
