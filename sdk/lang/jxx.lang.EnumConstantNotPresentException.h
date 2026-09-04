#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"

#include <memory>
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

    class EnumConstantNotPresentException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<EnumConstantNotPresentException, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    EnumConstantNotPresentException() = default;

    EnumConstantNotPresentException(const EnumConstantNotPresentException&) = default;
    EnumConstantNotPresentException(EnumConstantNotPresentException&&) noexcept = default;
    EnumConstantNotPresentException& operator=(const EnumConstantNotPresentException&) = default;
    EnumConstantNotPresentException& operator=(EnumConstantNotPresentException&&) noexcept = default;
    ~EnumConstantNotPresentException() override = default;

    public:
        using RuntimeException::RuntimeException;
        
    protected:
        JXX_OBJECT_CLONE(EnumConstantNotPresentException)
        const char* typeName() const noexcept override { return "EnumConstantNotPresentException"; }
    };

} // namespace jxx::lang
