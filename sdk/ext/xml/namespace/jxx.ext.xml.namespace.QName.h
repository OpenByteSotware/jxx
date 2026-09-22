#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::namespace_ {

class QName final
    : public ::jxx::lang::ClassBase<QName, ::jxx::lang::Object> {
public:
    explicit QName(
        const ::jxx::Ptr<::jxx::lang::String>& localPart);
    QName(
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& localPart);
    QName(
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& localPart,
        const ::jxx::Ptr<::jxx::lang::String>& prefix);

    ::jxx::Ptr<::jxx::lang::String> getNamespaceURI() const;
    ::jxx::Ptr<::jxx::lang::String> getLocalPart() const;
    ::jxx::Ptr<::jxx::lang::String> getPrefix() const;

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    static ::jxx::Ptr<QName> valueOf(
        const ::jxx::Ptr<::jxx::lang::String>& value);

private:
    ::jxx::Ptr<::jxx::lang::String> namespaceURI_;
    ::jxx::Ptr<::jxx::lang::String> localPart_;
    ::jxx::Ptr<::jxx::lang::String> prefix_;
};

} // namespace jxx::ext::xml::namespace_
