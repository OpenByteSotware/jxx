#pragma once

#include "io/jxx.io.ObjectStreamException.h"
#include "lang/jxx.lang.ClassInfoMarker.h"

namespace jxx::io {

class InvalidClassException : public ObjectStreamException {
public:
    using JxxSuper = ObjectStreamException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<InvalidClassException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    explicit InvalidClassException(
        const ::jxx::Ptr<::jxx::lang::String>& reason);

    InvalidClassException(
        const ::jxx::Ptr<::jxx::lang::String>& className,
        const ::jxx::Ptr<::jxx::lang::String>& reason);

    InvalidClassException(const InvalidClassException&) = default;
    InvalidClassException(InvalidClassException&&) noexcept = default;
    InvalidClassException& operator=(
        const InvalidClassException&) = default;
    InvalidClassException& operator=(
        InvalidClassException&&) noexcept = default;
    ~InvalidClassException() override = default;

    ::jxx::Ptr<::jxx::lang::String> getMessage() const;

    ::jxx::Ptr<::jxx::lang::String> classname;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
