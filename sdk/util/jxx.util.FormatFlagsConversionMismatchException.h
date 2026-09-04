#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

    class FormatFlagsConversionMismatchException : public IllegalFormatException {
public:
    using JxxSuper = IllegalFormatException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<FormatFlagsConversionMismatchException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    FormatFlagsConversionMismatchException() = default;

    FormatFlagsConversionMismatchException(const FormatFlagsConversionMismatchException&) = default;
    FormatFlagsConversionMismatchException(FormatFlagsConversionMismatchException&&) noexcept = default;
    FormatFlagsConversionMismatchException& operator=(const FormatFlagsConversionMismatchException&) = default;
    FormatFlagsConversionMismatchException& operator=(FormatFlagsConversionMismatchException&&) noexcept = default;
    ~FormatFlagsConversionMismatchException() override = default;

    public:
        using IllegalFormatException::IllegalFormatException;
        JXX_OBJECT_CLONE(FormatFlagsConversionMismatchException)
    protected:
        const char* typeName() const noexcept override { return "FormatFlagsConversionMismatchException"; }
    };

} // namespace jxx::util
