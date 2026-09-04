#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class ArithmeticException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ArithmeticException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ArithmeticException() = default;

    ArithmeticException(const ArithmeticException&) = default;
    ArithmeticException(ArithmeticException&&) noexcept = default;
    ArithmeticException& operator=(const ArithmeticException&) = default;
    ArithmeticException& operator=(ArithmeticException&&) noexcept = default;
    ~ArithmeticException() override = default;

public:
    using RuntimeException::RuntimeException;
    
protected:
    JXX_OBJECT_CLONE(ArithmeticException)
    const char* typeName() const noexcept override { return "ArithmeticException"; }
};

} // namespace jxx::lang