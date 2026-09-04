#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::util {

class IllegalFormatFlagsException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalFormatFlagsException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalFormatFlagsException() = default;

    IllegalFormatFlagsException(const IllegalFormatFlagsException&) = default;
    IllegalFormatFlagsException(IllegalFormatFlagsException&&) noexcept = default;
    IllegalFormatFlagsException& operator=(const IllegalFormatFlagsException&) = default;
    IllegalFormatFlagsException& operator=(IllegalFormatFlagsException&&) noexcept = default;
    ~IllegalFormatFlagsException() override = default;

public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(IllegalFormatFlagsException)
protected:
    const char* typeName() const noexcept override { return "NoSuchElementException"; }
};

} // namespace jxx::util