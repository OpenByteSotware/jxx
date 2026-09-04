#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::util {

class IllegalFormatWidthException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalFormatWidthException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalFormatWidthException() = default;

    IllegalFormatWidthException(const IllegalFormatWidthException&) = default;
    IllegalFormatWidthException(IllegalFormatWidthException&&) noexcept = default;
    IllegalFormatWidthException& operator=(const IllegalFormatWidthException&) = default;
    IllegalFormatWidthException& operator=(IllegalFormatWidthException&&) noexcept = default;
    ~IllegalFormatWidthException() override = default;

public:
    using jxx::lang::RuntimeException::RuntimeException;
    JXX_OBJECT_CLONE(IllegalFormatWidthException)
protected:
    const char* typeName() const noexcept override { return "IllegalFormatWidthException"; }
};

} // namespace jxx::util