#pragma once

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class IllegalMonitorStateException : public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(IllegalMonitorStateException)
        const char* typeName() const noexcept override { return "IllegalMonitorStateException"; }
    };

} // namespace jxx::lang
