#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParser.h"

#include "io/jxx.io.File.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"

namespace jxx::ext::xml::parsers {

SAXParser::SAXParser() : Super() {}
SAXParser::~SAXParser() = default;
::jxx::Ptr<::jxx::ext::xml::validation::Schema> SAXParser::getSchema() const { return nullptr; }
::jxx::lang::jbool SAXParser::isXIncludeAware() const { return false; }
void SAXParser::reset() {}

void SAXParser::parse(const ::jxx::Ptr<::jxx::org::xml::sax::InputSource>& input,
    const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler) {
    if (!input || !handler) throw ::jxx::lang::NullPointerException();
    auto reader=getXMLReader(); reader->setContentHandler(handler); reader->setEntityResolver(handler);
    reader->setDTDHandler(handler); reader->setErrorHandler(handler); reader->parse(input);
}
void SAXParser::parse(const ::jxx::Ptr<::jxx::io::InputStream>& input,
    const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler) {
    if (!input) throw ::jxx::lang::NullPointerException(); parse(::jxx::NEW<::jxx::org::xml::sax::InputSource>(input),handler);
}
void SAXParser::parse(const ::jxx::Ptr<::jxx::io::InputStream>& input,
    const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler,
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    if (!input || !systemId) throw ::jxx::lang::NullPointerException(); auto source=::jxx::NEW<::jxx::org::xml::sax::InputSource>(input);source->setSystemId(systemId);parse(source,handler);
}
void SAXParser::parse(const ::jxx::Ptr<::jxx::io::File>& file,
    const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler) {
    if (!file) throw ::jxx::lang::NullPointerException(); parse(file->getAbsolutePath(),handler);
}
void SAXParser::parse(const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::org::xml::sax::helpers::DefaultHandler>& handler) {
    if (!uri) throw ::jxx::lang::NullPointerException(); parse(::jxx::NEW<::jxx::org::xml::sax::InputSource>(uri),handler);
}

} // namespace jxx::ext::xml::parsers
