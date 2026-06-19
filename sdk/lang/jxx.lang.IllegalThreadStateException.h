#pragma once

#include <memory>
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::lang {

    class IllegalThreadStateException : public IllegalArgumentException {
    public:
        using IllegalArgumentException::IllegalArgumentException;
        JXX_OBJECT_CLONE(IllegalThreadStateException)
    protected:
        const char* typeName() const noexcept override { return "IllegalThreadStateException"; }
    };

} // namespace jxx::lang
