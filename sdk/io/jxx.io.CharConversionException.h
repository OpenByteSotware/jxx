#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class CharConversionException : public IOException {
public:
    using JxxSuper = IOException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<CharConversionException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    CharConversionException();
    explicit CharConversionException(const jxx::Ptr<jxx::lang::String>& message);
    explicit CharConversionException(const char* message);
    explicit CharConversionException(const std::string& message);

public:
    CharConversionException(const CharConversionException&) = default;
    CharConversionException(CharConversionException&&) noexcept = default;
    CharConversionException& operator=(const CharConversionException&) = default;
    CharConversionException& operator=(CharConversionException&&) noexcept = default;
    ~CharConversionException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
