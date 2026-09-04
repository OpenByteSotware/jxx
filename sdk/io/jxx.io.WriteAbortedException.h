#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.ObjectStreamException.h"

namespace jxx::io {

class WriteAbortedException : public ObjectStreamException {
public:
    using JxxSuper = ObjectStreamException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<WriteAbortedException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    WriteAbortedException();
    explicit WriteAbortedException(const jxx::Ptr<jxx::lang::String>& message);
    explicit WriteAbortedException(const char* message);
    explicit WriteAbortedException(const std::string& message);

public:
    WriteAbortedException(const WriteAbortedException&) = default;
    WriteAbortedException(WriteAbortedException&&) noexcept = default;
    WriteAbortedException& operator=(const WriteAbortedException&) = default;
    WriteAbortedException& operator=(WriteAbortedException&&) noexcept = default;
    ~WriteAbortedException() override = default;

public:
    WriteAbortedException(
        const jxx::Ptr<jxx::lang::String>& message,
        const jxx::Ptr<jxx::lang::Throwable>& detail);

    jxx::Ptr<jxx::lang::Throwable> getCause() const;
    jxx::Ptr<jxx::lang::String> getMessage() const;

    jxx::Ptr<jxx::lang::Throwable> detail;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
