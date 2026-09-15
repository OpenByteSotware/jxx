#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.Exception.h"

namespace jxx::lang {

class CloneNotSupportedException : public Exception {
public:
    using JxxSuper = Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<CloneNotSupportedException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    CloneNotSupportedException() = default;

    CloneNotSupportedException(const CloneNotSupportedException&) = default;
    CloneNotSupportedException(CloneNotSupportedException&&) noexcept = default;
    CloneNotSupportedException& operator=(const CloneNotSupportedException&) = default;
    CloneNotSupportedException& operator=(CloneNotSupportedException&&) noexcept = default;
    ~CloneNotSupportedException() override = default;

public:

    explicit CloneNotSupportedException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit CloneNotSupportedException(const char* message);
    JXX_OBJECT_CLONE(CloneNotSupportedException)
protected:
    const char* typeName() const noexcept override { return "CloneNotSupportedException"; }
};

} // namespace jxx::lang
