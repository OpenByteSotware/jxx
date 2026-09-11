#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ParserAdapter.h"

#include <string>
#include <utility>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributesImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.NamespaceSupport.h"
#include "org/xml/sax/jxx.org.xml.sax.AttributeList.h"
#include "org/xml/sax/jxx.org.xml.sax.ContentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DTDHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.Locator.h"
#include "org/xml/sax/jxx.org.xml.sax.Parser.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXNotRecognizedException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXNotSupportedException.h"

namespace jxx::org::xml::sax::helpers {

namespace {
const char* namespacesFeature() { return "http://xml.org/sax/features/namespaces"; }
const char* prefixesFeature() { return "http://xml.org/sax/features/namespace-prefixes"; }
const char* urisFeature() { return "http://xml.org/sax/features/xmlns-uris"; }

bool matches(const ::jxx::Ptr<::jxx::lang::String>& name, const char* value) {
    return name && name->utf8() == value;
}
}

struct ParserAdapter::Impl {
    ::jxx::Ptr<::jxx::org::xml::sax::Parser> parser;
    ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> entityResolver;
    ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler> dtdHandler;
    ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler> contentHandler;
    ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> errorHandler;
    ::jxx::Ptr<NamespaceSupport> namespaces = ::jxx::NEW<NamespaceSupport>();
    ::jxx::lang::jbool namespacesEnabled = true;
    ::jxx::lang::jbool prefixesEnabled = false;
    ::jxx::lang::jbool xmlnsUris = false;
    ::jxx::lang::jbool parsing = false;
};

ParserAdapter::ParserAdapter()
    : Super(), impl_(std::make_shared<Impl>()) {
    throw ::jxx::org::xml::sax::SAXException(
        ::jxx::NEW<::jxx::lang::String>("No SAX1 parser provider is configured"));
}

ParserAdapter::ParserAdapter(
    const ::jxx::Ptr<::jxx::org::xml::sax::Parser>& parser)
    : Super(), impl_(std::make_shared<Impl>()) {
    if (!parser) throw ::jxx::lang::NullPointerException();
    impl_->parser = parser;
}

ParserAdapter::~ParserAdapter() = default;

::jxx::lang::jbool ParserAdapter::getFeature(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    if (!name) throw ::jxx::lang::NullPointerException();
    if (matches(name, namespacesFeature())) return impl_->namespacesEnabled;
    if (matches(name, prefixesFeature())) return impl_->prefixesEnabled;
    if (matches(name, urisFeature())) return impl_->xmlnsUris;
    throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);
}

void ParserAdapter::setFeature(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    ::jxx::lang::jbool value) {
    if (!name) throw ::jxx::lang::NullPointerException();
    if (impl_->parsing) throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
    if (matches(name, namespacesFeature())) {
        if (!value && !impl_->prefixesEnabled) {
            throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
        }
        impl_->namespacesEnabled = value;
        return;
    }
    if (matches(name, prefixesFeature())) {
        if (!value && !impl_->namespacesEnabled) {
            throw ::jxx::org::xml::sax::SAXNotSupportedException(name);
        }
        impl_->prefixesEnabled = value;
        return;
    }
    if (matches(name, urisFeature())) {
        impl_->xmlnsUris = value;
        return;
    }
    throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);
}

::jxx::Ptr<::jxx::lang::Object> ParserAdapter::getProperty(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    if (!name) throw ::jxx::lang::NullPointerException();
    throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);
}

void ParserAdapter::setProperty(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    (void)value;
    if (!name) throw ::jxx::lang::NullPointerException();
    throw ::jxx::org::xml::sax::SAXNotRecognizedException(name);
}

void ParserAdapter::setEntityResolver(const ::jxx::Ptr<::jxx::org::xml::sax::EntityResolver>& v){impl_->entityResolver=v;}
::jxx::Ptr<::jxx::org::xml::sax::EntityResolver> ParserAdapter::getEntityResolver()const{return impl_->entityResolver;}
void ParserAdapter::setDTDHandler(const ::jxx::Ptr<::jxx::org::xml::sax::DTDHandler>& v){impl_->dtdHandler=v;}
::jxx::Ptr<::jxx::org::xml::sax::DTDHandler> ParserAdapter::getDTDHandler()const{return impl_->dtdHandler;}
void ParserAdapter::setContentHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ContentHandler>& v){impl_->contentHandler=v;}
::jxx::Ptr<::jxx::org::xml::sax::ContentHandler> ParserAdapter::getContentHandler()const{return impl_->contentHandler;}
void ParserAdapter::setErrorHandler(const ::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler>& v){impl_->errorHandler=v;}
::jxx::Ptr<::jxx::org::xml::sax::ErrorHandler> ParserAdapter::getErrorHandler()const{return impl_->errorHandler;}

void ParserAdapter::setupParser_() {
    impl_->parser->setEntityResolver(impl_->entityResolver);
    impl_->parser->setDTDHandler(impl_->dtdHandler);
    impl_->parser->setErrorHandler(impl_->errorHandler);
    impl_->parser->setDocumentHandler(
        ::jxx::CAST<::jxx::org::xml::sax::DocumentHandler>(thisPtr()));
}

void ParserAdapter::parse(const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input) {
    if (!input) throw ::jxx::lang::NullPointerException();
    if (impl_->parsing) {
        throw ::jxx::org::xml::sax::SAXException(
            ::jxx::NEW<::jxx::lang::String>("Parser is already in use"));
    }
    setupParser_();
    impl_->parsing = true;
    try { impl_->parser->parse(input); impl_->parsing = false; }
    catch (...) { impl_->parsing = false; throw; }
}

void ParserAdapter::parse(const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    if (!systemId) throw ::jxx::lang::NullPointerException();
    parse(::jxx::NEW<::jxx::org::xml::sax::InputSource>(systemId));
}

void ParserAdapter::setDocumentLocator(const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator){if(impl_->contentHandler)impl_->contentHandler->setDocumentLocator(locator);}
void ParserAdapter::startDocument(){impl_->namespaces->reset();impl_->namespaces->setNamespaceDeclUris(impl_->xmlnsUris);if(impl_->contentHandler)impl_->contentHandler->startDocument();}
void ParserAdapter::endDocument(){if(impl_->contentHandler)impl_->contentHandler->endDocument();}

void ParserAdapter::startElement(
    const ::jxx::Ptr<::jxx::lang::String>& qName,
    const ::jxx::Ptr<::jxx::org::xml::sax::AttributeList>& atts) {
    impl_->namespaces->pushContext();
    if (impl_->namespacesEnabled && atts) {
        for (::jxx::lang::jint i=0;i<atts->getLength();++i) {
            const auto name=atts->getName(i);
            if (!name) continue;
            const auto raw=name->utf8();
            if (raw=="xmlns") impl_->namespaces->declarePrefix(::jxx::NEW<::jxx::lang::String>(""),atts->getValue(i));
            else if (raw.rfind("xmlns:",0)==0) impl_->namespaces->declarePrefix(::jxx::NEW<::jxx::lang::String>(raw.substr(6)),atts->getValue(i));
        }
    }
    if (!impl_->contentHandler) return;
    auto result=::jxx::NEW<AttributesImpl>();
    if (atts) {
        for (::jxx::lang::jint i=0;i<atts->getLength();++i) {
            auto name=atts->getName(i);
            auto parts=::jxx::NEW<NamespaceSupport::StringArray>(3);
            auto processed=impl_->namespacesEnabled?impl_->namespaces->processName(name,parts,true):nullptr;
            auto uri=processed? (*processed)[0] : ::jxx::NEW<::jxx::lang::String>("");
            auto local=processed? (*processed)[1] : ::jxx::NEW<::jxx::lang::String>("");
            const auto raw=name?name->utf8():std::string();
            const bool declaration=raw=="xmlns"||raw.rfind("xmlns:",0)==0;
            if (!declaration||impl_->prefixesEnabled) result->addAttribute(uri,local,name,atts->getType(i),atts->getValue(i));
        }
    }
    auto parts=::jxx::NEW<NamespaceSupport::StringArray>(3);
    auto processed=impl_->namespacesEnabled?impl_->namespaces->processName(qName,parts,false):nullptr;
    auto uri=processed? (*processed)[0] : ::jxx::NEW<::jxx::lang::String>("");
    auto local=processed? (*processed)[1] : ::jxx::NEW<::jxx::lang::String>("");
    impl_->contentHandler->startElement(uri,local,impl_->prefixesEnabled ? qName : ::jxx::NEW<::jxx::lang::String>(""),result);
}

void ParserAdapter::endElement(const ::jxx::Ptr<::jxx::lang::String>& qName) {
    if (impl_->contentHandler) {
        auto parts=::jxx::NEW<NamespaceSupport::StringArray>(3);
        auto processed=impl_->namespacesEnabled?impl_->namespaces->processName(qName,parts,false):nullptr;
        auto uri=processed? (*processed)[0] : ::jxx::NEW<::jxx::lang::String>("");
        auto local=processed? (*processed)[1] : ::jxx::NEW<::jxx::lang::String>("");
        impl_->contentHandler->endElement(uri,local,impl_->prefixesEnabled ? qName : ::jxx::NEW<::jxx::lang::String>(""));
    }
    impl_->namespaces->popContext();
}

void ParserAdapter::characters(const ::jxx::lang::CharArray& c,::jxx::lang::jint s,::jxx::lang::jint n){if(impl_->contentHandler)impl_->contentHandler->characters(c,s,n);}
void ParserAdapter::ignorableWhitespace(const ::jxx::lang::CharArray& c,::jxx::lang::jint s,::jxx::lang::jint n){if(impl_->contentHandler)impl_->contentHandler->ignorableWhitespace(c,s,n);}
void ParserAdapter::processingInstruction(const ::jxx::Ptr<::jxx::lang::String>& t,const ::jxx::Ptr<::jxx::lang::String>& d){if(impl_->contentHandler)impl_->contentHandler->processingInstruction(t,d);}

} // namespace jxx::org::xml::sax::helpers
