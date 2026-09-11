#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "org/xml/sax/jxx.org.xml.sax.ContentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.Parser.h"

namespace jxx::lang {
class String;
}

namespace jxx::util {
class Locale;
}

namespace jxx::org::xml::sax {
class Attributes;
class DTDHandler;
class DocumentHandler;
class EntityResolver;
class ErrorHandler;
class InputSource;
class Locator;
class XMLReader;
}

namespace jxx::org::xml::sax::helpers {

class XMLReaderAdapter
    : public ::jxx::lang::ClassBase<
          XMLReaderAdapter,
          ::jxx::lang::Object,
          ::jxx::org::xml::sax::Parser,
          ::jxx::org::xml::sax::ContentHandler> {
public:
    using JxxSuper = ::jxx::lang::Object;

    using Super = ::jxx::lang::ClassBase<
        XMLReaderAdapter,
        JxxSuper,
        ::jxx::org::xml::sax::Parser,
        ::jxx::org::xml::sax::ContentHandler>;

    XMLReaderAdapter();

    explicit XMLReaderAdapter(
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::XMLReader>& parentReader);

    ~XMLReaderAdapter() override;

    void setLocale(
        const ::jxx::Ptr<::jxx::util::Locale>& locale) override;

    void setEntityResolver(
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::EntityResolver>& resolver) override;

    void setDTDHandler(
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::DTDHandler>& handler) override;

    void setDocumentHandler(
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::DocumentHandler>& handler) override;

    void setErrorHandler(
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::ErrorHandler>& handler) override;

    void parse(
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::InputSource>& input) override;

    void parse(
        const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    void setDocumentLocator(
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::Locator>& locator) override;

    void startDocument() override;
    void endDocument() override;

    void startPrefixMapping(
        const ::jxx::Ptr<::jxx::lang::String>& prefix,
        const ::jxx::Ptr<::jxx::lang::String>& uri) override;

    void endPrefixMapping(
        const ::jxx::Ptr<::jxx::lang::String>& prefix) override;

    void startElement(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::Attributes>& attributes) override;

    void endElement(
        const ::jxx::Ptr<::jxx::lang::String>& uri,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) override;

    void characters(
        const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) override;

    void ignorableWhitespace(
        const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start,
        ::jxx::lang::jint length) override;

    void processingInstruction(
        const ::jxx::Ptr<::jxx::lang::String>& target,
        const ::jxx::Ptr<::jxx::lang::String>& data) override;

    void skippedEntity(
        const ::jxx::Ptr<::jxx::lang::String>& name) override;

private:
    void configureReader_();

    ::jxx::Ptr<::jxx::org::xml::sax::XMLReader> reader_;
    ::jxx::Ptr<::jxx::org::xml::sax::DocumentHandler> documentHandler_;
};

} // namespace jxx::org::xml::sax::helpers
