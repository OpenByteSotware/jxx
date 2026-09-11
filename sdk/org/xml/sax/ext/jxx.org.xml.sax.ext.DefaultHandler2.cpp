#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DefaultHandler2.h"

namespace jxx::org::xml::sax::ext {

DefaultHandler2::DefaultHandler2()
    : Super() {
}

DefaultHandler2::~DefaultHandler2() = default;

void DefaultHandler2::elementDecl(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& model) {
    (void)name;
    (void)model;
}

void DefaultHandler2::attributeDecl(
    const ::jxx::Ptr<::jxx::lang::String>& elementName,
    const ::jxx::Ptr<::jxx::lang::String>& attributeName,
    const ::jxx::Ptr<::jxx::lang::String>& type,
    const ::jxx::Ptr<::jxx::lang::String>& mode,
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    (void)elementName;
    (void)attributeName;
    (void)type;
    (void)mode;
    (void)value;
}

void DefaultHandler2::internalEntityDecl(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    (void)name;
    (void)value;
}

void DefaultHandler2::externalEntityDecl(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    (void)name;
    (void)publicId;
    (void)systemId;
}

::jxx::Ptr<::jxx::org::xml::sax::InputSource>
DefaultHandler2::getExternalSubset(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& baseURI) {
    (void)name;
    (void)baseURI;
    return nullptr;
}

::jxx::Ptr<::jxx::org::xml::sax::InputSource>
DefaultHandler2::resolveEntity(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& baseURI,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    (void)name;
    (void)publicId;
    (void)baseURI;
    (void)systemId;
    return nullptr;
}

::jxx::Ptr<::jxx::org::xml::sax::InputSource>
DefaultHandler2::resolveEntity(
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    return resolveEntity(nullptr, publicId, nullptr, systemId);
}

void DefaultHandler2::startDTD(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    (void)name;
    (void)publicId;
    (void)systemId;
}

void DefaultHandler2::endDTD() {
}

void DefaultHandler2::startEntity(
    const ::jxx::Ptr<::jxx::lang::String>& name) {
    (void)name;
}

void DefaultHandler2::endEntity(
    const ::jxx::Ptr<::jxx::lang::String>& name) {
    (void)name;
}

void DefaultHandler2::startCDATA() {
}

void DefaultHandler2::endCDATA() {
}

void DefaultHandler2::comment(
    const ::jxx::lang::CharArray& characters,
    ::jxx::lang::jint start,
    ::jxx::lang::jint length) {
    (void)characters;
    (void)start;
    (void)length;
}

} // namespace jxx::org::xml::sax::ext
