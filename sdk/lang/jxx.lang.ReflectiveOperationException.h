#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "jxx.lang.Exception.h"

namespace jxx::lang {

    class ReflectiveOperationException : public Exception {
public:
    using JxxSuper = Exception;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<ReflectiveOperationException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    ReflectiveOperationException() = default;

    ReflectiveOperationException(const ReflectiveOperationException&) = default;
    ReflectiveOperationException(ReflectiveOperationException&&) noexcept = default;
    ReflectiveOperationException& operator=(const ReflectiveOperationException&) = default;
    ReflectiveOperationException& operator=(ReflectiveOperationException&&) noexcept = default;
    ~ReflectiveOperationException() override = default;

    public:
        using Exception::Exception;
        JXX_OBJECT_CLONE(ReflectiveOperationException)
    protected:
        const char* typeName() const noexcept override { return "ReflectiveOperationException"; }
    };
}