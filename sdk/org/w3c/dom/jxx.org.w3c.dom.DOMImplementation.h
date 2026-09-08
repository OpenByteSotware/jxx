#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class Object;
class String;

} // namespace jxx::lang

namespace jxx::org::w3c::dom {

class Document;
class DocumentType;

class DOMImplementation
    : public ::jxx::lang::InterfaceBase<DOMImplementation> {
public:
    virtual ~DOMImplementation() = default;

    virtual ::jxx::lang::jbool hasFeature(
        const ::jxx::Ptr<::jxx::lang::String>& feature,
        const ::jxx::Ptr<::jxx::lang::String>& version) const = 0;

    virtual ::jxx::Ptr<DocumentType> createDocumentType(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;

    virtual ::jxx::Ptr<Document> createDocument(
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<DocumentType>& documentType) = 0;

    virtual ::jxx::Ptr<::jxx::lang::Object> getFeature(
        const ::jxx::Ptr<::jxx::lang::String>& feature,
        const ::jxx::Ptr<::jxx::lang::String>& version) = 0;
};

} // namespace jxx::org::w3c::dom
