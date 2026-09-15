#pragma once

#include "io/jxx.io.ObjectStreamException.h"
#include "lang/jxx.lang.ClassInfoMarker.h"

namespace jxx::io {

class ObjectInputStream;

class OptionalDataException : public ObjectStreamException {
public:
    using JxxSuper = ObjectStreamException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<OptionalDataException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    OptionalDataException(const OptionalDataException&) = default;
    OptionalDataException(OptionalDataException&&) noexcept = default;
    OptionalDataException& operator=(
        const OptionalDataException&) = default;
    OptionalDataException& operator=(
        OptionalDataException&&) noexcept = default;
    ~OptionalDataException() override = default;

    ::jxx::lang::jint length = 0;
    ::jxx::lang::jbool eof = false;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;

private:
    friend class ObjectInputStream;

    explicit OptionalDataException(::jxx::lang::jint lengthValue);
    explicit OptionalDataException(::jxx::lang::jbool endOfData);
};

} // namespace jxx::io
