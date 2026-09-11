#pragma once

#include <memory>

#include "lang/jxx.lang.ClassInfo.h"
#include "org/xml/sax/jxx.org.xml.sax.DocumentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"

namespace jxx::org::xml::sax {
class AttributeList;
class Attributes;
class ContentHandler;
class DTDHandler;
class EntityResolver;
class ErrorHandler;
class InputSource;
class Locator;
class Parser;
}

namespace jxx::org::xml::sax::helpers {

class NamespaceSupport;

class ParserAdapter
    : public ::jxx::lang::ClassBase<
          ParserAdapter,
          ::jxx::lang::Object,
          ::jxx::org::xml::sax::XMLReader,
          ::jxx::org::xml::sax::DocumentHandler> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        ParserAdapter,
        JxxSuper,
        ::jxx::org::xml::sax::XMLReader,
        ::jxx::org::xml::sax::DocumentHandler>;

    ParserAdapter();
    explicit ParserAdapter(
        const ::jxx::Ptr<::jxx::org::xml::sax::Parser>& parser);
    ~ParserAdapter() override;

    ::jxx::lang::jbool getFeature(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    void setFeature(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        ::jxx::lang::jbool value) override;
    ::jxx::Ptr<::jxx::lang::Object> getProperty(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    void setProperty(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;

    void setEntityResolver(const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& resolver) override;
    ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> getEntityResolver() const override;
    void setDTDHandler(const ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>& handler) override;
    ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler> getDTDHandler() const override;
    void setContentHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>& handler) override;
    ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler> getContentHandler() const override;
    void setErrorHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& handler) override;
    ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> getErrorHandler() const override;

    void parse(const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input) override;
    void parse(const ::jxx::Ptr<::jxx::lang::String>& systemId) override;

    void setDocumentLocator(const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator) override;
    void startDocument() override;
    void endDocument() override;
    void startElement(
        const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
        const ::jxx::Ptr<::jxx::org::xml::sax::AttributeList>& attributes) override;
    void endElement(const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) override;
    void characters(const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start, ::jxx::lang::jint length) override;
    void ignorableWhitespace(const ::jxx::lang::CharArray& characters,
        ::jxx::lang::jint start, ::jxx::lang::jint length) override;
    void processingInstruction(
        const ::jxx::Ptr<::jxx::lang::String>& target,
        const ::jxx::Ptr<::jxx::lang::String>& data) override;

private:
    void setupParser_();
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

} // namespace jxx::org::xml::sax::helpers
