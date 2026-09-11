#include "org/xml/sax/internal/jxx.org.xml.sax.internal.PugiXMLReader.h"

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include <pugixml.hpp>

#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2Impl.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.DeclHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.LexicalHandler.h"
#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2Impl.h"
#include "org/xml/sax/jxx.org.xml.sax.ContentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DTDHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXNotRecognizedException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXNotSupportedException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

namespace jxx::org::xml::sax::internal {

namespace {

const char* namespacesFeature() {
    return "http://xml.org/sax/features/namespaces";
}

const char* namespacePrefixesFeature() {
    return "http://xml.org/sax/features/namespace-prefixes";
}

const char* validationFeature() {
    return "http://xml.org/sax/features/validation";
}

const char* useAttributes2Feature() {
    return "http://xml.org/sax/features/use-attributes2";
}

const char* useLocator2Feature() {
    return "http://xml.org/sax/features/use-locator2";
}

const char* lexicalHandlerProperty() {
    return "http://xml.org/sax/properties/lexical-handler";
}

const char* declarationHandlerProperty() {
    return "http://xml.org/sax/properties/declaration-handler";
}

bool matches(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const char* value) {
    return name != nullptr && name->utf8() == value;
}

std::string prefixPart(const std::string& qualifiedName) {
    const auto separator = qualifiedName.find(':');
    return separator == std::string::npos
        ? std::string()
        : qualifiedName.substr(0, separator);
}

std::string localPart(const std::string& qualifiedName) {
    const auto separator = qualifiedName.find(':');
    return separator == std::string::npos
        ? qualifiedName
        : qualifiedName.substr(separator + 1);
}

std::string namespaceURI(
    pugi::xml_node node,
    const std::string& qualifiedName) {
    const auto prefix = prefixPart(qualifiedName);
    const auto declaration = prefix.empty()
        ? std::string("xmlns")
        : std::string("xmlns:") + prefix;

    for (auto current = node; current; current = current.parent()) {
        const auto attribute = current.attribute(declaration.c_str());
        if (attribute) {
            return attribute.value();
        }
    }
    return std::string();
}

std::string readCharacters(
    const ::jxx::Ptr<::jxx::io::Reader>& input) {
    std::u16string value;
    for (;;) {
        const auto character = input->read();
        if (character < 0) {
            break;
        }
        value.push_back(static_cast<char16_t>(character));
    }
    return ::jxx::NEW<::jxx::lang::String>(value)->utf8();
}

std::string readBytes(
    const ::jxx::Ptr<::jxx::io::InputStream>& input) {
    std::string value;
    for (;;) {
        const auto byteValue = input->read();
        if (byteValue < 0) {
            break;
        }
        value.push_back(static_cast<char>(byteValue));
    }
    return value;
}

::jxx::lang::CharArray toCharacters(const char* value) {
    return ::jxx::NEW<::jxx::lang::String>(
        value == nullptr ? "" : value)->toCharArray();
}

} // namespace

struct PugiXMLReader::State {
    ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> entityResolver;
    ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler> dtdHandler;
    ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler> contentHandler;
    ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> errorHandler;
    ::jxx::Ptr<::jxx::org::xml::sax::ext::LexicalHandler> lexicalHandler;
    ::jxx::Ptr<::jxx::org::xml::sax::ext::DeclHandler> declarationHandler;
    ::jxx::lang::jbool namespaces = true;
    ::jxx::lang::jbool namespacePrefixes = false;
    ::jxx::lang::jbool parsing = false;
};

PugiXMLReader::PugiXMLReader()
    : Super(), state_(std::make_unique<State>()) {
}

PugiXMLReader::~PugiXMLReader() = default;

::jxx::lang::jbool PugiXMLReader::getFeature(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    if (name == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (matches(name, namespacesFeature())) return state_->namespaces;
    if (matches(name, namespacePrefixesFeature())) return state_->namespacePrefixes;
    if (matches(name, validationFeature())) return false;
    if (matches(name, useAttributes2Feature())) return true;
    if (matches(name, useLocator2Feature())) return true;
    throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);
}

void PugiXMLReader::setFeature(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    ::jxx::lang::jbool value) {
    if (name == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (state_->parsing) {
        throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
    }
    if (matches(name, namespacesFeature())) {
        state_->namespaces = value;
        return;
    }
    if (matches(name, namespacePrefixesFeature())) {
        state_->namespacePrefixes = value;
        return;
    }
    if (matches(name, validationFeature())) {
        if (value) {
            throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
        }
        return;
    }
    if (matches(name, useAttributes2Feature()) ||
        matches(name, useLocator2Feature())) {
        if (!value) {
            throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
        }
        return;
    }
    throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);
}

::jxx::Ptr<::jxx::lang::Object> PugiXMLReader::getProperty(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    if (name == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (matches(name, lexicalHandlerProperty())) {
        return state_->lexicalHandler == nullptr
            ? nullptr
            : ::jxx::CAST<::jxx::lang::Object>(state_->lexicalHandler);
    }
    if (matches(name, declarationHandlerProperty())) {
        return state_->declarationHandler == nullptr
            ? nullptr
            : ::jxx::CAST<::jxx::lang::Object>(state_->declarationHandler);
    }
    throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);
}

void PugiXMLReader::setProperty(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    if (name == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (state_->parsing) {
        throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
    }
    if (matches(name, lexicalHandlerProperty())) {
        const auto handler =
            ::jxx::CAST<::jxx::org::xml::sax::ext::LexicalHandler>(value);
        if (value != nullptr && handler == nullptr) {
            throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
        }
        state_->lexicalHandler = handler;
        return;
    }
    if (matches(name, declarationHandlerProperty())) {
        const auto handler =
            ::jxx::CAST<::jxx::org::xml::sax::ext::DeclHandler>(value);
        if (value != nullptr && handler == nullptr) {
            throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
        }
        state_->declarationHandler = handler;
        return;
    }
    throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);
}

void PugiXMLReader::setEntityResolver(
    const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& value) {
    state_->entityResolver = value;
}

::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>
PugiXMLReader::getEntityResolver() const {
    return state_->entityResolver;
}

void PugiXMLReader::setDTDHandler(
    const ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>& value) {
    state_->dtdHandler = value;
}

::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>
PugiXMLReader::getDTDHandler() const {
    return state_->dtdHandler;
}

void PugiXMLReader::setContentHandler(
    const ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>& value) {
    state_->contentHandler = value;
}

::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>
PugiXMLReader::getContentHandler() const {
    return state_->contentHandler;
}

void PugiXMLReader::setErrorHandler(
    const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& value) {
    state_->errorHandler = value;
}

::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>
PugiXMLReader::getErrorHandler() const {
    return state_->errorHandler;
}

void PugiXMLReader::parse(
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    if (systemId == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    parse(::jxx::NEW<::jxx::org::xml::sax::InputSource>(systemId));
}

void PugiXMLReader::parse(
    const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input) {
    if (input == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (state_->parsing) {
        throw ::jxx::org::xml::sax::SAXNotSupportedException(
            ::jxx::NEW<::jxx::lang::String>("Reader is already parsing"));
    }

    struct ParsingGuard {
        explicit ParsingGuard(::jxx::lang::jbool& valueRef)
            : value(valueRef) { value = true; }
        ~ParsingGuard() { value = false; }
        ::jxx::lang::jbool& value;
    } guard(state_->parsing);

    pugi::xml_document document;
    pugi::xml_parse_result result;
    std::string sourceData;

    const auto characterStream = input->getCharacterStream();
    const auto byteStream = input->getByteStream();

    const auto parseOptions =
        pugi::parse_default |
        pugi::parse_comments |
        pugi::parse_pi |
        pugi::parse_cdata;

    if (characterStream != nullptr) {
        sourceData = readCharacters(characterStream);
        result = document.load_buffer(
            sourceData.data(), sourceData.size(), parseOptions);
    }
    else if (byteStream != nullptr) {
        sourceData = readBytes(byteStream);
        result = document.load_buffer(
            sourceData.data(), sourceData.size(), parseOptions);
    }
    else if (input->getSystemId() != nullptr) {
        result = document.load_file(
            input->getSystemId()->utf8().c_str(), parseOptions);
    }
    else {
        throw ::jxx::lang::NullPointerException();
    }

    if (!result) {
        const auto exception =
            ::jxx::NEW<::jxx::org::xml::sax::SAXParseException>(
                ::jxx::NEW<::jxx::lang::String>(result.description()),
                input->getPublicId(),
                input->getSystemId(),
                1,
                static_cast<::jxx::lang::jint>(result.offset + 1));
        if (state_->errorHandler != nullptr) {
            state_->errorHandler->fatalError(exception);
        }
        throw *exception;
    }

    const auto contentHandler = state_->contentHandler;
    if (contentHandler == nullptr) {
        return;
    }

    const auto locator =
        ::jxx::NEW<::jxx::org::xml::sax::ext::Locator2Impl>();
    locator->setPublicId(input->getPublicId());
    locator->setSystemId(input->getSystemId());
    locator->setLineNumber(1);
    locator->setColumnNumber(1);
    locator->setXMLVersion(
        ::jxx::NEW<::jxx::lang::String>("1.0"));
    locator->setEncoding(input->getEncoding());

    contentHandler->setDocumentLocator(locator);
    contentHandler->startDocument();

    const auto lexicalHandler = state_->lexicalHandler;

    std::function<void(pugi::xml_node)> visit;
    visit = [&](pugi::xml_node node) {
        if (node.type() == pugi::node_element) {
            std::vector<std::string> mappings;
            for (const auto attribute : node.attributes()) {
                const std::string name = attribute.name();
                if (name == "xmlns" || name.rfind("xmlns:", 0) == 0) {
                    const auto prefix = name == "xmlns"
                        ? std::string()
                        : name.substr(6);
                    contentHandler->startPrefixMapping(
                        ::jxx::NEW<::jxx::lang::String>(prefix),
                        ::jxx::NEW<::jxx::lang::String>(attribute.value()));
                    mappings.push_back(prefix);
                }
            }

            const auto attributes =
                ::jxx::NEW<::jxx::org::xml::sax::ext::Attributes2Impl>();

            for (const auto attribute : node.attributes()) {
                const std::string name = attribute.name();
                const bool declaration =
                    name == "xmlns" || name.rfind("xmlns:", 0) == 0;
                if (declaration && !state_->namespacePrefixes) {
                    continue;
                }
                const auto uri = declaration
                    ? std::string("http://www.w3.org/2000/xmlns/")
                    : (state_->namespaces
                        ? namespaceURI(node, name)
                        : std::string());
                attributes->addAttribute(
                    ::jxx::NEW<::jxx::lang::String>(uri),
                    ::jxx::NEW<::jxx::lang::String>(
                        state_->namespaces ? localPart(name) : std::string()),
                    ::jxx::NEW<::jxx::lang::String>(
                        state_->namespaces && !state_->namespacePrefixes
                            ? std::string()
                            : name),
                    ::jxx::NEW<::jxx::lang::String>("CDATA"),
                    ::jxx::NEW<::jxx::lang::String>(attribute.value()));
            }

            const std::string qualifiedName = node.name();
            const auto uri = state_->namespaces
                ? namespaceURI(node, qualifiedName)
                : std::string();
            const auto localName = state_->namespaces
                ? localPart(qualifiedName)
                : std::string();
            const auto reportedName =
                state_->namespaces && !state_->namespacePrefixes
                    ? std::string()
                    : qualifiedName;

            contentHandler->startElement(
                ::jxx::NEW<::jxx::lang::String>(uri),
                ::jxx::NEW<::jxx::lang::String>(localName),
                ::jxx::NEW<::jxx::lang::String>(reportedName),
                attributes);

            for (const auto child : node.children()) {
                visit(child);
            }

            contentHandler->endElement(
                ::jxx::NEW<::jxx::lang::String>(uri),
                ::jxx::NEW<::jxx::lang::String>(localName),
                ::jxx::NEW<::jxx::lang::String>(reportedName));

            for (auto iterator = mappings.rbegin();
                 iterator != mappings.rend();
                 ++iterator) {
                contentHandler->endPrefixMapping(
                    ::jxx::NEW<::jxx::lang::String>(*iterator));
            }
            return;
        }

        if (node.type() == pugi::node_pcdata) {
            const auto charactersValue = toCharacters(node.value());
            contentHandler->characters(
                charactersValue, 0, charactersValue->length);
            return;
        }

        if (node.type() == pugi::node_cdata) {
            if (lexicalHandler != nullptr) {
                lexicalHandler->startCDATA();
            }
            const auto charactersValue = toCharacters(node.value());
            contentHandler->characters(
                charactersValue, 0, charactersValue->length);
            if (lexicalHandler != nullptr) {
                lexicalHandler->endCDATA();
            }
            return;
        }

        if (node.type() == pugi::node_comment) {
            if (lexicalHandler != nullptr) {
                const auto charactersValue = toCharacters(node.value());
                lexicalHandler->comment(
                    charactersValue, 0, charactersValue->length);
            }
            return;
        }

        if (node.type() == pugi::node_pi) {
            contentHandler->processingInstruction(
                ::jxx::NEW<::jxx::lang::String>(node.name()),
                ::jxx::NEW<::jxx::lang::String>(node.value()));
        }
    };

    for (const auto node : document.children()) {
        visit(node);
    }

    contentHandler->endDocument();
}

} // namespace jxx::org::xml::sax::internal
