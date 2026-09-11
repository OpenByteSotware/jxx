#include "ext/xml/parsers/internal/jxx.ext.xml.parsers.internal.PugiSAXParser.h"

#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderAdapter.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"

namespace jxx::ext::xml::parsers::internal {

PugiSAXParser::PugiSAXParser(
    ::jxx::lang::jbool namespaceAware,
    ::jxx::lang::jbool validating,
    const ::jxx::Ptr<::jxx::ext::xml::validation::Schema>& schema,
    ::jxx::lang::jbool xIncludeAware)
    : SAXParser()
    , reader_(
          ::jxx::org::xml::sax::helpers::XMLReaderFactory::
              createXMLReader())
    , namespaceAware_(namespaceAware)
    , validating_(validating)
    , schema_(schema)
    , xIncludeAware_(xIncludeAware) {

    reader_->setFeature(
        ::jxx::NEW<::jxx::lang::String>(
            "http://xml.org/sax/features/namespaces"),
        namespaceAware_);
}

PugiSAXParser::~PugiSAXParser() = default;

::jxx::Ptr<::jxx::org::xml::sax::Parser>
PugiSAXParser::getParser() {
    return ::jxx::CAST<::jxx::org::xml::sax::Parser>(
        ::jxx::NEW<::jxx::org::xml::sax::helpers::XMLReaderAdapter>(reader_));
}

::jxx::Ptr<::jxx::org::xml::sax::XMLReader>
PugiSAXParser::getXMLReader() {
    return reader_;
}

::jxx::lang::jbool PugiSAXParser::isNamespaceAware() const {
    return namespaceAware_;
}

::jxx::lang::jbool PugiSAXParser::isValidating() const {
    return validating_;
}

void PugiSAXParser::setProperty(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    reader_->setProperty(name, value);
}

::jxx::Ptr<::jxx::lang::Object>
PugiSAXParser::getProperty(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    return reader_->getProperty(name);
}

::jxx::Ptr<::jxx::ext::xml::validation::Schema>
PugiSAXParser::getSchema() const {
    return schema_;
}

::jxx::lang::jbool PugiSAXParser::isXIncludeAware() const {
    return xIncludeAware_;
}

void PugiSAXParser::reset() {
    reader_ =
        ::jxx::org::xml::sax::helpers::XMLReaderFactory::
            createXMLReader();

    reader_->setFeature(
        ::jxx::NEW<::jxx::lang::String>(
            "http://xml.org/sax/features/namespaces"),
        namespaceAware_);
}

} // namespace jxx::ext::xml::parsers::internal
