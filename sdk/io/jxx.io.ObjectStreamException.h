#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class ObjectStreamException : public IOException {
public:
    using JxxSuper = IOException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<ObjectStreamException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

protected:
    ObjectStreamException();
    explicit ObjectStreamException(const jxx::Ptr<jxx::lang::String>& message);
    explicit ObjectStreamException(const char* message);
    explicit ObjectStreamException(const std::string& message);

public:
    ObjectStreamException(const ObjectStreamException&) = default;
    ObjectStreamException(ObjectStreamException&&) noexcept = default;
    ObjectStreamException& operator=(const ObjectStreamException&) = default;
    ObjectStreamException& operator=(ObjectStreamException&&) noexcept = default;
    ~ObjectStreamException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
