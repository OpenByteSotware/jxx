#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderAdapter.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.AttributeList.h"
#include "org/xml/sax/jxx.org.xml.sax.Attributes.h"
#include "org/xml/sax/jxx.org.xml.sax.DocumentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"

namespace jxx::org::xml::sax::helpers {

namespace {

const char* namespacePrefixesFeature() {
    return "http://xml.org/sax/features/namespace-prefixes";
}

class AttributeListAdapter final
    : public ::jxx::lang::ClassBase<
          AttributeListAdapter,
          ::jxx::lang::Object,
          ::jxx::org::xml::sax::AttributeList> {
public:
    explicit AttributeListAdapter(
        const ::jxx::Ptr<
            ::jxx::org::xml::sax::Attributes>& values)
        : Super()
        , values_(values) {
    }

    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        AttributeListAdapter,
        JxxSuper,
        ::jxx::org::xml::sax::AttributeList>;

    ::jxx::lang::jint getLength() const override {
        return values_ ? values_->getLength() : 0;
    }

    ::jxx::Ptr<::jxx::lang::String> getName(
        ::jxx::lang::jint index) const override {
        return values_ ? values_->getQName(index) : nullptr;
    }

    ::jxx::Ptr<::jxx::lang::String> getType(
        ::jxx::lang::jint index) const override {
        return values_ ? values_->getType(index) : nullptr;
    }

    ::jxx::Ptr<::jxx::lang::String> getValue(
        ::jxx::lang::jint index) const override {
        return values_ ? values_->getValue(index) : nullptr;
    }

    ::jxx::Ptr<::jxx::lang::String> getType(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override {
        return values_ ? values_->getType(name) : nullptr;
    }

    ::jxx::Ptr<::jxx::lang::String> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override {
        return values_ ? values_->getValue(name) : nullptr;
    }

private:
    ::jxx::Ptr<::jxx::org::xml::sax::Attributes> values_;
};

} // namespace

XMLReaderAdapter::XMLReaderAdapter()
    : Super()
    , reader_(XMLReaderFactory::createXMLReader())
    , documentHandler_(nullptr) {

    if (!reader_) {
        throw ::jxx::org::xml::sax::SAXException(
            ::jxx::NEW<::jxx::lang::String>(
                "Unable to create XMLReader"));
    }
}

XMLReaderAdapter::XMLReaderAdapter(
    const ::jxx::Ptr<
        ::jxx::org::xml::sax::XMLReader>& parentReader)
    : Super()
    , reader_(parentReader)
    , documentHandler_(nullptr) {

    if (!parentReader) {
        throw ::jxx::lang::NullPointerException();
    }
}

XMLReaderAdapter::~XMLReaderAdapter() = default;

void XMLReaderAdapter::setLocale(
    const ::jxx::Ptr<::jxx::util::Locale>& locale) {
    (void)locale;
    throw ::jxx::org::xml::sax::SAXException(
        ::jxx::NEW<::jxx::lang::String>(
            "Locale selection is not supported"));
}

void XMLReaderAdapter::setEntityResolver(
    const ::jxx::Ptr<
        ::jxx::org::xml::sax::EntityResolver>& resolver) {
    reader_->setEntityResolver(resolver);
}

void XMLReaderAdapter::setDTDHandler(
    const ::jxx::Ptr<
        ::jxx::org::xml::sax::DTDHandler>& handler) {
    reader_->setDTDHandler(handler);
}

void XMLReaderAdapter::setDocumentHandler(
    const ::jxx::Ptr<
        ::jxx::org::xml::sax::DocumentHandler>& handler) {
    documentHandler_ = handler;
}

void XMLReaderAdapter::setErrorHandler(
    const ::jxx::Ptr<
        ::jxx::org::xml::sax::ErrorHandler>& handler) {
    reader_->setErrorHandler(handler);
}

void XMLReaderAdapter::configureReader_() {
    reader_->setContentHandler(
        ::jxx::CAST<::jxx::org::xml::sax::ContentHandler>(
            thisPtr()));

    reader_->setFeature(
        ::jxx::NEW<::jxx::lang::String>(
            namespacePrefixesFeature()),
        true);
}

void XMLReaderAdapter::parse(
    const ::jxx::Ptr<
        ::jxx::org::xml::sax::InputSource>& input) {
    configureReader_();
    reader_->parse(input);
}

void XMLReaderAdapter::parse(
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    configureReader_();
    reader_->parse(systemId);
}

void XMLReaderAdapter::setDocumentLocator(
    const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator) {
    if (documentHandler_) {
        documentHandler_->setDocumentLocator(locator);
    }
}

void XMLReaderAdapter::startDocument() {
    if (documentHandler_) {
        documentHandler_->startDocument();
    }
}

void XMLReaderAdapter::endDocument() {
    if (documentHandler_) {
        documentHandler_->endDocument();
    }
}

void XMLReaderAdapter::startPrefixMapping(
    const ::jxx::Ptr<::jxx::lang::String>& prefix,
    const ::jxx::Ptr<::jxx::lang::String>& uri) {
    (void)prefix;
    (void)uri;
}

void XMLReaderAdapter::endPrefixMapping(
    const ::jxx::Ptr<::jxx::lang::String>& prefix) {
    (void)prefix;
}

void XMLReaderAdapter::startElement(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
    const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes) {
    (void)uri;
    (void)localName;

    if (documentHandler_) {
        documentHandler_->startElement(
            qualifiedName,
            ::jxx::NEW<AttributeListAdapter>(attributes));
    }
}

void XMLReaderAdapter::endElement(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) {
    (void)uri;
    (void)localName;

    if (documentHandler_) {
        documentHandler_->endElement(qualifiedName);
    }
}

void XMLReaderAdapter::characters(
    const ::jxx::lang::CharArray& charactersValue,
    ::jxx::lang::jint start,
    ::jxx::lang::jint length) {
    if (documentHandler_) {
        documentHandler_->characters(charactersValue, start, length);
    }
}

void XMLReaderAdapter::ignorableWhitespace(
    const ::jxx::lang::CharArray& charactersValue,
    ::jxx::lang::jint start,
    ::jxx::lang::jint length) {
    if (documentHandler_) {
        documentHandler_->ignorableWhitespace(
            charactersValue,
            start,
            length);
    }
}

void XMLReaderAdapter::processingInstruction(
    const ::jxx::Ptr<::jxx::lang::String>& target,
    const ::jxx::Ptr<::jxx::lang::String>& data) {
    if (documentHandler_) {
        documentHandler_->processingInstruction(target, data);
    }
}

void XMLReaderAdapter::skippedEntity(
    const ::jxx::Ptr<::jxx::lang::String>& name) {
    (void)name;
}

} // namespace jxx::org::xml::sax::helpers
