#pragma once

#include <memory>
#include "lang/jxx.lang.Exception.h"

namespace jxx::util {

    class TooManyListenersException : public jxx::lang::Exception {
    public:
        using Exception::Exception;
        JXX_OBJECT_CLONE(TooManyListenersException)
    protected:
        const char* typeName() const noexcept override { return "TooManyListenersException"; }
    };

} // namespace jxx::util
