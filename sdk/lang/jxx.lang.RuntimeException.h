#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.Exception.h"

namespace jxx::lang {

    class RuntimeException : public jxx::lang::Exception {
public:
    using JxxSuper = jxx::lang::Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<RuntimeException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    RuntimeException() = default;

    RuntimeException(const RuntimeException&) = default;
    RuntimeException(RuntimeException&&) noexcept = default;
    RuntimeException& operator=(const RuntimeException&) = default;
    RuntimeException& operator=(RuntimeException&&) noexcept = default;
    ~RuntimeException() override = default;

    public:
        using jxx::lang::Exception::Exception;
        
    protected:
        JXX_OBJECT_CLONE(RuntimeException)
        const char* typeName() const noexcept override { return "RuntimeException"; }
    };

} // namespace jxx::lang
