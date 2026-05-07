#pragma once

#include <memory>
#include "jxx.lang.Exception.h"

namespace jxx::lang {

    class ReflectiveOperationException : public Exception {
    public:
        using Exception::Exception;
        JXX_OBJECT_CLONE(ReflectiveOperationException)
    protected:
        const char* typeName() const noexcept override { return "ReflectiveOperationException"; }
    };
}