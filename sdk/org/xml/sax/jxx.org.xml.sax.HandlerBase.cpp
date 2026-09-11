#include "org/xml/sax/jxx.org.xml.sax.HandlerBase.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

namespace jxx::org::xml::sax {

HandlerBase::HandlerBase()
    : Super() {
}

HandlerBase::~HandlerBase() = default;

::jxx::Ptr<InputSource> HandlerBase::resolveEntity(
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    (void)publicId;
    (void)systemId;
    return nullptr;
}

void HandlerBase::notationDecl(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    (void)name;
    (void)publicId;
    (void)systemId;
}

void HandlerBase::unparsedEntityDecl(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId,
    const ::jxx::Ptr<::jxx::lang::String>& notationName) {
    (void)name;
    (void)publicId;
    (void)systemId;
    (void)notationName;
}

void HandlerBase::setDocumentLocator(const ::jxx::Ptr<Locator>& locator) {
    (void)locator;
}

void HandlerBase::startDocument() {
}

void HandlerBase::endDocument() {
}

void HandlerBase::startElement(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<AttributeList>& attributes) {
    (void)name;
    (void)attributes;
}

void HandlerBase::endElement(
    const ::jxx::Ptr<::jxx::lang::String>& name) {
    (void)name;
}

void HandlerBase::characters(
    const ::jxx::lang::CharArray& characters,
    ::jxx::lang::jint start,
    ::jxx::lang::jint length) {
    (void)characters;
    (void)start;
    (void)length;
}

void HandlerBase::ignorableWhitespace(
    const ::jxx::lang::CharArray& characters,
    ::jxx::lang::jint start,
    ::jxx::lang::jint length) {
    (void)characters;
    (void)start;
    (void)length;
}

void HandlerBase::processingInstruction(
    const ::jxx::Ptr<::jxx::lang::String>& target,
    const ::jxx::Ptr<::jxx::lang::String>& data) {
    (void)target;
    (void)data;
}

void HandlerBase::warning(
    const ::jxx::Ptr<SAXParseException>& exception) {
    (void)exception;
}

void HandlerBase::error(
    const ::jxx::Ptr<SAXParseException>& exception) {
    (void)exception;
}

void HandlerBase::fatalError(
    const ::jxx::Ptr<SAXParseException>& exception) {
    if (!exception) {
        throw ::jxx::lang::NullPointerException();
    }
    throw *exception;
}

} // namespace jxx::org::xml::sax
