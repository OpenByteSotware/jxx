#pragma once

#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind {

template <typename T>
class JAXBElement
    : public ::jxx::lang::ClassBase<JAXBElement<T>, ::jxx::lang::Object> {
public:
    JAXBElement(
        const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& name,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& declaredType,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& scope,
        const ::jxx::Ptr<T>& value)
        : name_(name), declaredType_(declaredType), scope_(scope),
          value_(value), nil_(false) {
        if (name_ == nullptr || declaredType_ == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
    }

    JAXBElement(
        const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& name,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& declaredType,
        const ::jxx::Ptr<T>& value)
        : JAXBElement(name, declaredType, nullptr, value) {}

    ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> getName() const {
        return name_;
    }

    ::jxx::Ptr<::jxx::lang::ClassAny> getDeclaredType() const {
        return declaredType_;
    }

    ::jxx::Ptr<::jxx::lang::ClassAny> getScope() const {
        return scope_;
    }

    ::jxx::Ptr<T> getValue() const { return value_; }

    void setValue(const ::jxx::Ptr<T>& value) { value_ = value; }

    ::jxx::lang::jbool isNil() const noexcept {
        return nil_ || value_ == nullptr;
    }

    void setNil(::jxx::lang::jbool value) noexcept { nil_ = value; }

    ::jxx::lang::jbool isGlobalScope() const noexcept {
        return scope_ == nullptr;
    }

    ::jxx::lang::jbool isTypeSubstituted() const {
        return value_ != nullptr &&
            !declaredType_->isInstance(
                ::jxx::CAST<::jxx::lang::Object>(value_));
    }

private:
    ::jxx::Ptr<::jxx::ext::xml::namespace_::QName> name_;
    ::jxx::Ptr<::jxx::lang::ClassAny> declaredType_;
    ::jxx::Ptr<::jxx::lang::ClassAny> scope_;
    ::jxx::Ptr<T> value_;
    ::jxx::lang::jbool nil_;
};

} // namespace jxx::ext::xml::bind
