#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementation.h"

namespace jxx::org::w3c::dom::internal {

class DOMImplementationImpl final
    : public ::jxx::lang::ClassBase<
          DOMImplementationImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::DOMImplementation> {
public:
    using JxxSuper = ::jxx::lang::Object;

    using Super = ::jxx::lang::ClassBase<
        DOMImplementationImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::DOMImplementation>;

    DOMImplementationImpl();

    DOMImplementationImpl(
        const DOMImplementationImpl&) = delete;

    DOMImplementationImpl(
        DOMImplementationImpl&&) = delete;

    DOMImplementationImpl& operator=(
        const DOMImplementationImpl&) = delete;

    DOMImplementationImpl& operator=(
        DOMImplementationImpl&&) = delete;

    ~DOMImplementationImpl() override;

    ::jxx::lang::jbool hasFeature(
        const ::jxx::Ptr<::jxx::lang::String>& feature,
        const ::jxx::Ptr<::jxx::lang::String>& version) const override;

    ::jxx::Ptr<::jxx::org::w3c::dom::DocumentType>
    createDocumentType(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    ::jxx::Ptr<::jxx::org::w3c::dom::Document>
    createDocument(
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<
            ::jxx::org::w3c::dom::DocumentType>& documentType) override;

    ::jxx::Ptr<::jxx::lang::Object> getFeature(
        const ::jxx::Ptr<::jxx::lang::String>& feature,
        const ::jxx::Ptr<::jxx::lang::String>& version) override;
};

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>
implementation();

} // namespace jxx::org::w3c::dom::internal
