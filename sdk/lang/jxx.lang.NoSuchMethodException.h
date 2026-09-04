#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class NoSuchMethodException : public ReflectiveOperationException {
public:
    using JxxSuper = ReflectiveOperationException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NoSuchMethodException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NoSuchMethodException() = default;

    NoSuchMethodException(const NoSuchMethodException&) = default;
    NoSuchMethodException(NoSuchMethodException&&) noexcept = default;
    NoSuchMethodException& operator=(const NoSuchMethodException&) = default;
    NoSuchMethodException& operator=(NoSuchMethodException&&) noexcept = default;
    ~NoSuchMethodException() override = default;

public:
    using ReflectiveOperationException::ReflectiveOperationException;
    
protected:
    JXX_OBJECT_CLONE(NoSuchMethodException)
    const char* typeName() const noexcept override { return "NoSuchMethodException"; }
};

} // namespace jxx::lang
