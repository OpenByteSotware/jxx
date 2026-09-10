#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiDocumentBuilder.h"

#include <string>

#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.PugiDom.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

namespace jxx::ext::xml::parsers::internal {

namespace {

std::string readBytes(const ::jxx::Ptr<::jxx::io::InputStream>& stream) {
    std::string result;
    for (;;) {
        const auto value = stream->read();
        if (value < 0) break;
        result.push_back(static_cast<char>(value));
    }
    return result;
}

std::string readCharacters(const ::jxx::Ptr<::jxx::io::Reader>& reader) {
    std::u16string characters;
    for (;;) {
        const auto value = reader->read();
        if (value < 0) break;
        characters.push_back(static_cast<char16_t>(value));
    }
    return ::jxx::NEW<::jxx::lang::String>(characters)->utf8();
}

} // namespace

PugiDocumentBuilder::PugiDocumentBuilder(
    ::jxx::lang::jbool namespaceAware,
    ::jxx::lang::jbool validating,
    ::jxx::lang::jbool ignoreWhitespace,
    ::jxx::lang::jbool expandEntities,
    ::jxx::lang::jbool ignoreComments,
    ::jxx::lang::jbool coalescing,
    ::jxx::lang::jbool xincludeAware,
    const ::jxx::Ptr<::jxx::ext::xml::validation::Schema>& schema)
    : Super()
    , namespaceAware_(namespaceAware)
    , validating_(validating)
    , ignoreWhitespace_(ignoreWhitespace)
    , expandEntities_(expandEntities)
    , ignoreComments_(ignoreComments)
    , coalescing_(coalescing)
    , xincludeAware_(xincludeAware)
    , schema_(schema) {
}

PugiDocumentBuilder::~PugiDocumentBuilder() = default;

::jxx::Ptr<::jxx::org::w3c::dom::Document> PugiDocumentBuilder::parse(
    const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input) {
    if (input == nullptr) {
        throw ::jxx::lang::IllegalArgumentException(
            ::jxx::NEW<::jxx::lang::String>("input"));
    }

    std::string error;
    ::jxx::lang::jint offset = 0;
    ::jxx::Ptr<::jxx::org::w3c::dom::Document> document;

    const auto characterStream = input->getCharacterStream();
    const auto byteStream = input->getByteStream();
    const auto systemId = input->getSystemId();

    if (characterStream != nullptr) {
        document = ::jxx::org::w3c::dom::internal::parseDocument(
            readCharacters(characterStream), ignoreComments_, coalescing_, error, offset);
    }
    else if (byteStream != nullptr) {
        document = ::jxx::org::w3c::dom::internal::parseDocument(
            readBytes(byteStream), ignoreComments_, coalescing_, error, offset);
    }
    else if (systemId != nullptr) {
        document = ::jxx::org::w3c::dom::internal::loadDocument(
            systemId->utf8(), ignoreComments_, coalescing_, error, offset);
    }
    else {
        throw ::jxx::lang::IllegalArgumentException(
            ::jxx::NEW<::jxx::lang::String>("InputSource has no input"));
    }

    if (document == nullptr) {
        auto exception = ::jxx::NEW<::jxx::org::xml::sax::SAXParseException>(
            ::jxx::NEW<::jxx::lang::String>(error),
            input->getPublicId(), input->getSystemId(), 1, offset + 1);
        if (handler_ != nullptr) handler_->fatalError(exception);
        throw *exception;
    }

    return document;
}

::jxx::Ptr<::jxx::org::w3c::dom::Document> PugiDocumentBuilder::newDocument() {
    return ::jxx::org::w3c::dom::internal::newDocument();
}

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation> PugiDocumentBuilder::getDOMImplementation() {
    return ::jxx::org::w3c::dom::internal::implementation();
}

void PugiDocumentBuilder::setEntityResolver(
    const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& resolver) { resolver_ = resolver; }
void PugiDocumentBuilder::setErrorHandler(
    const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& handler) { handler_ = handler; }
::jxx::lang::jbool PugiDocumentBuilder::isNamespaceAware() const { return namespaceAware_; }
::jxx::lang::jbool PugiDocumentBuilder::isValidating() const { return validating_; }
::jxx::Ptr<::jxx::ext::xml::validation::Schema> PugiDocumentBuilder::getSchema() const { return schema_; }
::jxx::lang::jbool PugiDocumentBuilder::isXIncludeAware() const { return xincludeAware_; }
void PugiDocumentBuilder::reset() { resolver_.reset(); handler_.reset(); }

} // namespace jxx::ext::xml::parsers::internal
