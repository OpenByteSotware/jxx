#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.ObjectStreamException.h"

namespace jxx::io {

class StreamCorruptedException : public ObjectStreamException {
public:
    using JxxSuper = ObjectStreamException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<StreamCorruptedException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    StreamCorruptedException();
    explicit StreamCorruptedException(const jxx::Ptr<jxx::lang::String>& message);
    explicit StreamCorruptedException(const char* message);
    explicit StreamCorruptedException(const std::string& message);

public:
    StreamCorruptedException(const StreamCorruptedException&) = default;
    StreamCorruptedException(StreamCorruptedException&&) noexcept = default;
    StreamCorruptedException& operator=(const StreamCorruptedException&) = default;
    StreamCorruptedException& operator=(StreamCorruptedException&&) noexcept = default;
    ~StreamCorruptedException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
