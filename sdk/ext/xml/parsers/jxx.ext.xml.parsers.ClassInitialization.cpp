#include "ext/xml/parsers/jxx.ext.xml.parsers.ClassInitialization.h"

#include <mutex>

#include "ext/xml/parsers/jxx.ext.xml.parsers.FactoryConfigurationError.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.ParserConfigurationException.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilder.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.DocumentBuilderFactory.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParser.h"
#include "ext/xml/parsers/jxx.ext.xml.parsers.SAXParserFactory.h"

namespace jxx::ext::xml::parsers {

void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)FactoryConfigurationError::Class();
        (void)ParserConfigurationException::Class();
        (void)DocumentBuilder::JxxClassInfoMarker::Class();
        (void)DocumentBuilderFactory::JxxClassInfoMarker::Class();
        (void)SAXParser::JxxClassInfoMarker::Class();
        (void)SAXParserFactory::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::xml::parsers
