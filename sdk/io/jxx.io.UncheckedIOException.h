#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "io/jxx.io.IOException.h"

namespace jxx::io {

class UncheckedIOException : public jxx::lang::RuntimeException {
public:
    using JxxSuper = jxx::lang::RuntimeException;
    using JxxClassInfoMarker =
        jxx::lang::ClassInfo<UncheckedIOException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    explicit UncheckedIOException(const jxx::Ptr<IOException>& cause);
    UncheckedIOException(
        const jxx::Ptr<jxx::lang::String>& message,
        const jxx::Ptr<IOException>& cause);

    UncheckedIOException(const UncheckedIOException&) = default;
    UncheckedIOException(UncheckedIOException&&) noexcept = default;
    UncheckedIOException& operator=(const UncheckedIOException&) = default;
    UncheckedIOException& operator=(UncheckedIOException&&) noexcept = default;
    ~UncheckedIOException() override = default;

    jxx::Ptr<IOException> getCause() const;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;

private:
    jxx::Ptr<IOException> cause_;
};

} // namespace jxx::io
