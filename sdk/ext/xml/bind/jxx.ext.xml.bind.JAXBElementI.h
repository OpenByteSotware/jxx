#pragma once

#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind {

class JAXBElementI
    : public ::jxx::lang::InterfaceBase<JAXBElementI> {
public:
    ~JAXBElementI() override = default;

    virtual ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> getName() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::ClassAny> getDeclaredType() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::ClassAny> getScope() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> getValueObject() const = 0;
    virtual ::jxx::lang::jbool isNil() const noexcept = 0;
    virtual void setNil(::jxx::lang::jbool value) noexcept = 0;
    virtual ::jxx::lang::jbool isGlobalScope() const noexcept = 0;
    virtual ::jxx::lang::jbool isTypeSubstituted() const = 0;
};

} // namespace jxx::ext::xml::bind
