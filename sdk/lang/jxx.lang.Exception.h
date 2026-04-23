#include <stdexcept>
#include <string>
#include "jxx.lang.Throwable.h"

namespace jxx::lang {
    class Exception : public Throwable {
    public:
        using Throwable::Throwable;
        JXX_THROWABLE_CLONE(Exception)
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
        InvocationTargetException(String message, std::shared_ptr<Throwable> cause)
            : Exception(std::move(message), std::move(cause)) {}
        JXX_THROWABLE_CLONE(InvocationTargetException)
    protected:
        const char* typeName() const noexcept override { return "InvocationTargetException"; }
    };
}
