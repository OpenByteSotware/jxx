#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ParserFactory.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderAdapter.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"
#include "org/xml/sax/jxx.org.xml.sax.Parser.h"

namespace jxx::org::xml::sax::helpers {

::jxx::Ptr<::jxx::org::xml::sax::Parser>
ParserFactory::makeParser() {
    return ::jxx::CAST<::jxx::org::xml::sax::Parser>(
        ::jxx::NEW<XMLReaderAdapter>(
            XMLReaderFactory::createXMLReader()));
}

::jxx::Ptr<::jxx::org::xml::sax::Parser>
ParserFactory::makeParser(
    const ::jxx::Ptr<::jxx::lang::String>& className) {
    if (className == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    return ::jxx::CAST<::jxx::org::xml::sax::Parser>(
        ::jxx::NEW<XMLReaderAdapter>(
            XMLReaderFactory::createXMLReader(className)));
}

} // namespace jxx::org::xml::sax::helpers
