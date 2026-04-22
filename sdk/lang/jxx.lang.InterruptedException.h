#pragma once

#include <memory>
#include "jxx.lang.Exception.h"

namespace jxx::lang {

class InterruptedException : public Exception {
public:
    using Exception::Exception;
    JXX_THROWABLE_CLONE(InterruptedException)
protected:
    const char* typeName() const noexcept override { return "InterruptedException"; }
};

} // namespace jxx::lang