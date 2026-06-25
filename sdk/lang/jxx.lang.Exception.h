#pragma once

#include <stdexcept>
#include <string>
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {
    class Exception : public jxx::lang::Throwable {
    public:
        using jxx::lang::Throwable::Throwable;
        
    protected:
        JXX_OBJECT_CLONE(jxx::lang::Exception)
        const char* typeName() const noexcept override { return "Exception"; }
    };

    class InvocationTargetException : public Exception {
    public:
        InvocationTargetException(jxx::Ptr<String> message, jxx::Ptr<Throwable> cause)
            : Exception(message, cause) {}
        
    protected:
        JXX_OBJECT_CLONE(InvocationTargetException)
        const char* typeName() const noexcept override { return "InvocationTargetException"; }
    };
}
