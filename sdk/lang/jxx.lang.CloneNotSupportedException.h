#pragma once

#include <memory>
#include "jxx.lang.Exception.h"

namespace jxx::lang {

class CloneNotSupportedException : public Exception {
public:
    using Exception::Exception;
    JXX_THROWABLE_CLONE(CloneNotSupportedException)
protected:
    const char* typeName() const noexcept override { return "CloneNotSupportedException"; }
};

} // namespace jxx::lang
``