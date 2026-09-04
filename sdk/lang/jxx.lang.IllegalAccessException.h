#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class IllegalAccessException : public ReflectiveOperationException {
public:
    using JxxSuper = ReflectiveOperationException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalAccessException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalAccessException() = default;

    IllegalAccessException(const IllegalAccessException&) = default;
    IllegalAccessException(IllegalAccessException&&) noexcept = default;
    IllegalAccessException& operator=(const IllegalAccessException&) = default;
    IllegalAccessException& operator=(IllegalAccessException&&) noexcept = default;
    ~IllegalAccessException() override = default;

public:
    using ReflectiveOperationException::ReflectiveOperationException;
    JXX_OBJECT_CLONE(IllegalAccessException)
protected:
    const char* typeName() const noexcept override { return "IllegalAccessException"; }
};

} // namespace jxx::lang
