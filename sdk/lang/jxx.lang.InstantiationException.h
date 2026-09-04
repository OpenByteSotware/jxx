#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class InstantiationException : public ReflectiveOperationException {
public:
    using JxxSuper = ReflectiveOperationException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<InstantiationException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    InstantiationException() = default;

    InstantiationException(const InstantiationException&) = default;
    InstantiationException(InstantiationException&&) noexcept = default;
    InstantiationException& operator=(const InstantiationException&) = default;
    InstantiationException& operator=(InstantiationException&&) noexcept = default;
    ~InstantiationException() override = default;

public:
    using ReflectiveOperationException::ReflectiveOperationException;
    
protected:
    JXX_OBJECT_CLONE(InstantiationException)
    const char* typeName() const noexcept override { return "InstantiationException"; }
};

} // namespace jxx::lang