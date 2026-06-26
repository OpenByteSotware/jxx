#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Thread.h"

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
