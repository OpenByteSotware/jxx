#pragma once

#include <string>

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class SyncFailedException : public IOException {
public:
    using JxxSuper = IOException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<SyncFailedException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

public:
    SyncFailedException();
    explicit SyncFailedException(const jxx::Ptr<jxx::lang::String>& message);
    explicit SyncFailedException(const char* message);
    explicit SyncFailedException(const std::string& message);

public:
    SyncFailedException(const SyncFailedException&) = default;
    SyncFailedException(SyncFailedException&&) noexcept = default;
    SyncFailedException& operator=(const SyncFailedException&) = default;
    SyncFailedException& operator=(SyncFailedException&&) noexcept = default;
    ~SyncFailedException() override = default;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::io
