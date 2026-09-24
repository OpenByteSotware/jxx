#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class NoSuchFieldException : public ReflectiveOperationException {
public:
    using JxxSuper = ReflectiveOperationException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NoSuchFieldException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NoSuchFieldException() = default;

    NoSuchFieldException(const NoSuchFieldException&) = default;
    NoSuchFieldException(NoSuchFieldException&&) noexcept = default;
    NoSuchFieldException& operator=(const NoSuchFieldException&) = default;
    NoSuchFieldException& operator=(NoSuchFieldException&&) noexcept = default;
    ~NoSuchFieldException() override = default;

public:

    explicit NoSuchFieldException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit NoSuchFieldException(const char* message);
    
protected:
    JXX_OBJECT_CLONE(NoSuchFieldException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang