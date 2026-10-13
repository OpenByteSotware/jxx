#include "org/xml/sax/jxx.org.xml.sax.ClassInitialization.h"

#include <mutex>

#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"
#include "org/xml/sax/jxx.org.xml.sax.AttributeList.h"
#include "org/xml/sax/jxx.org.xml.sax.Attributes.h"
#include "org/xml/sax/jxx.org.xml.sax.ContentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DocumentHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.DTDHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.EntityResolver.h"
#include "org/xml/sax/jxx.org.xml.sax.ErrorHandler.h"
#include "org/xml/sax/jxx.org.xml.sax.HandlerBase.h"
#include "org/xml/sax/jxx.org.xml.sax.Locator.h"
#include "org/xml/sax/jxx.org.xml.sax.Parser.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXNotRecognizedException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXNotSupportedException.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLFilter.h"
#include "org/xml/sax/jxx.org.xml.sax.XMLReader.h"

namespace jxx::org::xml::sax {

void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)InputSource::Class();
        (void)SAXException::Class();
        (void)SAXParseException::Class();
        (void)AttributeList::JxxClassInfoMarker::Class();
        (void)Attributes::JxxClassInfoMarker::Class();
        (void)ContentHandler::JxxClassInfoMarker::Class();
        (void)DocumentHandler::JxxClassInfoMarker::Class();
        (void)DTDHandler::JxxClassInfoMarker::Class();
        (void)EntityResolver::JxxClassInfoMarker::Class();
        (void)ErrorHandler::JxxClassInfoMarker::Class();
        (void)HandlerBase::JxxClassInfoMarker::Class();
        (void)Locator::JxxClassInfoMarker::Class();
        (void)Parser::JxxClassInfoMarker::Class();
        (void)SAXNotRecognizedException::JxxClassInfoMarker::Class();
        (void)SAXNotSupportedException::JxxClassInfoMarker::Class();
        (void)XMLFilter::JxxClassInfoMarker::Class();
        (void)XMLReader::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::org::xml::sax
