#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "ObjectStreamException.h"

namespace jxx::io {

class NotSerializableException : public ObjectStreamException {
public:
    using JxxSuper = ObjectStreamException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<NotSerializableException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    NotSerializableException();
    explicit NotSerializableException(const jxx::Ptr<jxx::lang::String>& message);
    explicit NotSerializableException(const char* message);
    explicit NotSerializableException(const std::string& message);

public:
    NotSerializableException(const NotSerializableException&) = default;
    NotSerializableException(NotSerializableException&&) noexcept = default;
    NotSerializableException& operator=(const NotSerializableException&) = default;
    NotSerializableException& operator=(NotSerializableException&&) noexcept = default;
    ~NotSerializableException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
