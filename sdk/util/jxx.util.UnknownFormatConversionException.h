#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "jxx.util.IllegalFormatException.h"

namespace jxx::util {

class UnknownFormatConversionException : public IllegalFormatException {
public:
    using JxxSuper = IllegalFormatException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<UnknownFormatConversionException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    UnknownFormatConversionException(const UnknownFormatConversionException&) = default;
    UnknownFormatConversionException(UnknownFormatConversionException&&) noexcept = default;
    UnknownFormatConversionException& operator=(const UnknownFormatConversionException&) = default;
    UnknownFormatConversionException& operator=(UnknownFormatConversionException&&) noexcept = default;
    ~UnknownFormatConversionException() override = default;

public:
    explicit UnknownFormatConversionException(const jxx::Ptr<jxx::lang::String> msg)
        : IllegalFormatException(std::move(msg)) {}
};

} // namespace jxx::util::format
