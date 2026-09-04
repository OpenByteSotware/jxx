#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class NegativeArraySizeException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NegativeArraySizeException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NegativeArraySizeException() = default;

    NegativeArraySizeException(const NegativeArraySizeException&) = default;
    NegativeArraySizeException(NegativeArraySizeException&&) noexcept = default;
    NegativeArraySizeException& operator=(const NegativeArraySizeException&) = default;
    NegativeArraySizeException& operator=(NegativeArraySizeException&&) noexcept = default;
    ~NegativeArraySizeException() override = default;

    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(NegativeArraySizeException)
        const char* typeName() const noexcept override { return "NegativeArraySizeException"; }
    };

} // namespace jxx::lang
