#pragma once

#include <memory>
#include "lang/jxx.lang.Exception.h"

namespace jxx::lang {

    class RuntimeException : public jxx::lang::Exception {
    public:
        using jxx::lang::Exception::Exception;
        
    protected:
        std::shared_ptr<jxx::lang::Object> cloneImpl() const override {
            return jxx::NEW<RuntimeException>(*this);
        }
        const char* typeName() const noexcept override { return "RuntimeException"; }
    };

} // namespace jxx::lang
