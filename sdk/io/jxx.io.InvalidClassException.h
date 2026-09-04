#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.ObjectStreamException.h"

namespace jxx::io {

class InvalidClassException : public ObjectStreamException {
public:
    using JxxSuper = ObjectStreamException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<InvalidClassException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    InvalidClassException();
    explicit InvalidClassException(const jxx::Ptr<jxx::lang::String>& message);
    explicit InvalidClassException(const char* message);
    explicit InvalidClassException(const std::string& message);

public:
    InvalidClassException(const InvalidClassException&) = default;
    InvalidClassException(InvalidClassException&&) noexcept = default;
    InvalidClassException& operator=(const InvalidClassException&) = default;
    InvalidClassException& operator=(InvalidClassException&&) noexcept = default;
    ~InvalidClassException() override = default;

public:   
    InvalidClassException(
        const jxx::Ptr<jxx::lang::String>& className,
        const jxx::Ptr<jxx::lang::String>& reason);

    jxx::Ptr<jxx::lang::String> getMessage() const;

    jxx::Ptr<jxx::lang::String> classname;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
