#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.ObjectStreamException.h"

namespace jxx::io {

class NotActiveException : public ObjectStreamException {
public:
    using JxxSuper = ObjectStreamException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<NotActiveException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    NotActiveException();
    explicit NotActiveException(const jxx::Ptr<jxx::lang::String>& message);
    explicit NotActiveException(const char* message);
    explicit NotActiveException(const std::string& message);

public:
    NotActiveException(const NotActiveException&) = default;
    NotActiveException(NotActiveException&&) noexcept = default;
    NotActiveException& operator=(const NotActiveException&) = default;
    NotActiveException& operator=(NotActiveException&&) noexcept = default;
    ~NotActiveException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
