#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class UnsupportedEncodingException : public IOException {
public:
    using JxxSuper = IOException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<UnsupportedEncodingException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    UnsupportedEncodingException();
    explicit UnsupportedEncodingException(const jxx::Ptr<jxx::lang::String>& message);
    explicit UnsupportedEncodingException(const char* message);
    explicit UnsupportedEncodingException(const std::string& message);

public:
    UnsupportedEncodingException(const UnsupportedEncodingException&) = default;
    UnsupportedEncodingException(UnsupportedEncodingException&&) noexcept = default;
    UnsupportedEncodingException& operator=(const UnsupportedEncodingException&) = default;
    UnsupportedEncodingException& operator=(UnsupportedEncodingException&&) noexcept = default;
    ~UnsupportedEncodingException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
