#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class UnsupportedOperationException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<UnsupportedOperationException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    UnsupportedOperationException() = default;

    UnsupportedOperationException(const UnsupportedOperationException&) = default;
    UnsupportedOperationException(UnsupportedOperationException&&) noexcept = default;
    UnsupportedOperationException& operator=(const UnsupportedOperationException&) = default;
    UnsupportedOperationException& operator=(UnsupportedOperationException&&) noexcept = default;
    ~UnsupportedOperationException() override = default;

public:
    using RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(UnsupportedOperationException)
protected:
    const char* typeName() const noexcept override { return "UnsupportedOperationException"; }
};

} // namespace jxx::lang