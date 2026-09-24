#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.Exception.h"

namespace jxx::lang {

class InterruptedException : public Exception {
public:
    using JxxSuper = Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<InterruptedException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    InterruptedException() = default;

    InterruptedException(const InterruptedException&) = default;
    InterruptedException(InterruptedException&&) noexcept = default;
    InterruptedException& operator=(const InterruptedException&) = default;
    InterruptedException& operator=(InterruptedException&&) noexcept = default;
    ~InterruptedException() override = default;

public:

    explicit InterruptedException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit InterruptedException(const char* message);
    JXX_OBJECT_CLONE(InterruptedException)
protected:
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang