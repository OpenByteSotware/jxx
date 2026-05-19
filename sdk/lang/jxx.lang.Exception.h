#pragma once

#include <stdexcept>
#include <string>
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {
    class Exception : public jxx::lang::Throwable {
    public:
        using jxx::lang::Throwable::Throwable;
        JXX_OBJECT_CLONE(jxx::lang::Exception)
    protected:
        const char* typeName() const noexcept override { return "Exception"; }
    };

    class InvocationTargetException : public Exception {
    public:
        InvocationTargetException(jxx::Ptr<String> message, jxx::Ptr<Throwable> cause)
            : Exception(message, cause) {}
        JXX_OBJECT_CLONE(InvocationTargetException)
    protected:
        const char* typeName() const noexcept override { return "InvocationTargetException"; }
    };
}
