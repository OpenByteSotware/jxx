#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class IllegalFormatConversionException : public IllegalFormatException {
public:
    using JxxSuper = IllegalFormatException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalFormatConversionException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalFormatConversionException() = default;

    IllegalFormatConversionException(const IllegalFormatConversionException&) = default;
    IllegalFormatConversionException(IllegalFormatConversionException&&) noexcept = default;
    IllegalFormatConversionException& operator=(const IllegalFormatConversionException&) = default;
    IllegalFormatConversionException& operator=(IllegalFormatConversionException&&) noexcept = default;
    ~IllegalFormatConversionException() override = default;

    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(IllegalFormatConversionException)
    protected:
        const char* typeName() const noexcept override { return "IllegalFormatConversionException"; }
    };

} // namespace jxx::util
