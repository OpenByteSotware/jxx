
#pragma once

#include <memory>
#include "jxx.lang.Exception.h"

namespace jxx::lang {

    class RuntimeException : public Exception {
    public:
        using Exception::Exception;
        JXX_THROWABLE_CLONE(RuntimeException)
    protected:
        const char* typeName() const noexcept override { return "RuntimeException"; }
    };

} // namespace jxx::lang
