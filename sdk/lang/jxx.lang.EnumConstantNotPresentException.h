#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace jxx::lang {

class EnumConstantNotPresentException : public RuntimeException {
public:
    using JxxSuper = RuntimeException;
    using JxxClassInfoMarker =
        ClassInfo<EnumConstantNotPresentException, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    EnumConstantNotPresentException(
        const ::jxx::Ptr<ClassAny>& enumType,
        const ::jxx::Ptr<String>& constantName);

    EnumConstantNotPresentException(
        const EnumConstantNotPresentException&) = default;
    EnumConstantNotPresentException(
        EnumConstantNotPresentException&&) noexcept = default;
    EnumConstantNotPresentException& operator=(
        const EnumConstantNotPresentException&) = default;
    EnumConstantNotPresentException& operator=(
        EnumConstantNotPresentException&&) noexcept = default;
    ~EnumConstantNotPresentException() override = default;

    ::jxx::Ptr<ClassAny> enumType() const;
    ::jxx::Ptr<String> constantName() const;

protected:
    JXX_OBJECT_CLONE(EnumConstantNotPresentException)
    const char* typeName() const noexcept override;

private:
    ::jxx::Ptr<ClassAny> enumType_;
    ::jxx::Ptr<String> constantName_;
};

} // namespace jxx::lang
