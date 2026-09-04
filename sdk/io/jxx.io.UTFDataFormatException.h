#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class UTFDataFormatException : public IOException {
public:
    using JxxSuper = IOException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<UTFDataFormatException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    UTFDataFormatException();
    explicit UTFDataFormatException(const jxx::Ptr<jxx::lang::String>& message);
    explicit UTFDataFormatException(const char* message);
    explicit UTFDataFormatException(const std::string& message);

public:
    UTFDataFormatException(const UTFDataFormatException&) = default;
    UTFDataFormatException(UTFDataFormatException&&) noexcept = default;
    UTFDataFormatException& operator=(const UTFDataFormatException&) = default;
    UTFDataFormatException& operator=(UTFDataFormatException&&) noexcept = default;
    ~UTFDataFormatException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
