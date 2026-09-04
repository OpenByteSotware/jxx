#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class InterruptedIOException : public IOException {
public:
    using JxxSuper = IOException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<InterruptedIOException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    InterruptedIOException();
    explicit InterruptedIOException(const jxx::Ptr<jxx::lang::String>& message);
    explicit InterruptedIOException(const char* message);
    explicit InterruptedIOException(const std::string& message);

public:
    InterruptedIOException(const InterruptedIOException&) = default;
    InterruptedIOException(InterruptedIOException&&) noexcept = default;
    InterruptedIOException& operator=(const InterruptedIOException&) = default;
    InterruptedIOException& operator=(InterruptedIOException&&) noexcept = default;
    ~InterruptedIOException() override = default;

public:
    jxx::lang::jint bytesTransferred = 0;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
