#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class SecurityException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<SecurityException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    SecurityException() = default;

    SecurityException(const SecurityException&) = default;
    SecurityException(SecurityException&&) noexcept = default;
    SecurityException& operator=(const SecurityException&) = default;
    SecurityException& operator=(SecurityException&&) noexcept = default;
    ~SecurityException() override = default;

public:

    explicit SecurityException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit SecurityException(const char* message);
    JXX_OBJECT_CLONE(SecurityException)
protected:
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang