#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.ObjectStreamException.h"

namespace jxx::io {

class InvalidObjectException : public ObjectStreamException {
public:
    using JxxSuper = ObjectStreamException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<InvalidObjectException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    InvalidObjectException();
    explicit InvalidObjectException(const jxx::Ptr<jxx::lang::String>& message);
    explicit InvalidObjectException(const char* message);
    explicit InvalidObjectException(const std::string& message);

public:
    InvalidObjectException(const InvalidObjectException&) = default;
    InvalidObjectException(InvalidObjectException&&) noexcept = default;
    InvalidObjectException& operator=(const InvalidObjectException&) = default;
    InvalidObjectException& operator=(InvalidObjectException&&) noexcept = default;
    ~InvalidObjectException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
