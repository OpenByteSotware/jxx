#include <stdexcept>
#include <string>
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {
    class Exception : public jxx::lang::Throwable {
    public:
        using jxx::lang::Throwable::Throwable;
        JXX_THROWABLE_CLONE(jxx::lang::Exception)
    protected:
        const char* typeName() const noexcept override { return "Exception"; }
    };

    class IllegalStateException : public Exception {
    public:
        using Exception::Exception;
        JXX_THROWABLE_CLONE(IllegalStateException)
    protected:
        const char* typeName() const noexcept override { return "IllegalStateException"; }
    };

    class InvocationTargetException : public Exception {
    public:
        InvocationTargetException(jxx::Ptr<String> message, jxx::Ptr<Throwable> cause)
            : Exception(std::move(message), std::move(cause)) {}
        JXX_THROWABLE_CLONE(InvocationTargetException)
    protected:
        const char* typeName() const noexcept override { return "InvocationTargetException"; }
    };
}
