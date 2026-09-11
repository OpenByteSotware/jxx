#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

namespace jxx::org::xml::sax::helpers {

DefaultHandler::DefaultHandler() = default;
DefaultHandler::~DefaultHandler() = default;

::jxx::Ptr<::jxx::org::xml::sax::InputSource>
DefaultHandler::resolveEntity(
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    (void)publicId;
    (void)systemId;
    return nullptr;
}

void DefaultHandler::notationDecl(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    (void)name; (void)publicId; (void)systemId;
}

void DefaultHandler::unparsedEntityDecl(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId,
    const ::jxx::Ptr<::jxx::lang::String>& notationName) {
    (void)name; (void)publicId; (void)systemId; (void)notationName;
}

void DefaultHandler::setDocumentLocator(const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator) { (void)locator; }
void DefaultHandler::startDocument() {}
void DefaultHandler::endDocument() {}
void DefaultHandler::startPrefixMapping(const ::jxx::Ptr<::jxx::lang::String>& prefix, const ::jxx::Ptr<::jxx::lang::String>& uri) { (void)prefix; (void)uri; }
void DefaultHandler::endPrefixMapping(const ::jxx::Ptr<::jxx::lang::String>& prefix) { (void)prefix; }
void DefaultHandler::startElement(const ::jxx::Ptr<::jxx::lang::String>& uri, const ::jxx::Ptr<::jxx::lang::String>& localName, const ::jxx::Ptr<::jxx::lang::String>& qName, const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes) { (void)uri; (void)localName; (void)qName; (void)attributes; }
void DefaultHandler::endElement(const ::jxx::Ptr<::jxx::lang::String>& uri, const ::jxx::Ptr<::jxx::lang::String>& localName, const ::jxx::Ptr<::jxx::lang::String>& qName) { (void)uri; (void)localName; (void)qName; }
void DefaultHandler::characters(const ::jxx::lang::CharArray& characters, ::jxx::lang::jint start, ::jxx::lang::jint length) { (void)characters; (void)start; (void)length; }
void DefaultHandler::ignorableWhitespace(const ::jxx::lang::CharArray& characters, ::jxx::lang::jint start, ::jxx::lang::jint length) { (void)characters; (void)start; (void)length; }
void DefaultHandler::processingInstruction(const ::jxx::Ptr<::jxx::lang::String>& target, const ::jxx::Ptr<::jxx::lang::String>& data) { (void)target; (void)data; }
void DefaultHandler::skippedEntity(const ::jxx::Ptr<::jxx::lang::String>& name) { (void)name; }
void DefaultHandler::warning(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) { (void)exception; }
void DefaultHandler::error(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) { (void)exception; }
void DefaultHandler::fatalError(const ::jxx::Ptr<::jxx::org::xml::sax::SAXParseException>& exception) {
    if (exception == nullptr) throw ::jxx::lang::NullPointerException();
    throw *exception;
}

} // namespace jxx::org::xml::sax::helpers
