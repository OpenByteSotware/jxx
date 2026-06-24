#pragma once

#include <memory>
#include "lang/jxx.lang.Exception.h"

namespace jxx::lang {

    class RuntimeException : public jxx::lang::Exception {
    public:
        using jxx::lang::Exception::Exception;
        
    protected:
        JXX_OBJECT_CLONE(RuntimeException)
        const char* typeName() const noexcept override { return "RuntimeException"; }
    };

} // namespace jxx::lang
