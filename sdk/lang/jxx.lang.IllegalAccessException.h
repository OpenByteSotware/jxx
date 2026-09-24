#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.ReflectiveOperationException.h"

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

    explicit IllegalAccessException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit IllegalAccessException(const char* message);
    JXX_OBJECT_CLONE(IllegalAccessException)
protected:
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
