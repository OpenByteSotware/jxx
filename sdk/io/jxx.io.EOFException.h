#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class EOFException : public IOException {
public:
    using JxxSuper = IOException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<EOFException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    EOFException();
    explicit EOFException(const jxx::Ptr<jxx::lang::String>& message);
    explicit EOFException(const char* message);
    explicit EOFException(const std::string& message);

public:
    EOFException(const EOFException&) = default;
    EOFException(EOFException&&) noexcept = default;
    EOFException& operator=(const EOFException&) = default;
    EOFException& operator=(EOFException&&) noexcept = default;
    ~EOFException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
