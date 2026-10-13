#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ClassInitialization.h"

#include <mutex>

#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributeListImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributesImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.DefaultHandler.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.LocatorImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.NamespaceSupport.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ParserAdapter.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.ParserFactory.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLFilterImpl.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderAdapter.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.XMLReaderFactory.h"

namespace jxx::org::xml::sax::helpers {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)AttributeListImpl::JxxClassInfoMarker::Class();
        (void)AttributesImpl::JxxClassInfoMarker::Class();
        (void)DefaultHandler::JxxClassInfoMarker::Class();
        (void)LocatorImpl::JxxClassInfoMarker::Class();
        (void)NamespaceSupport::JxxClassInfoMarker::Class();
        (void)ParserAdapter::JxxClassInfoMarker::Class();
        (void)ParserFactory::JxxClassInfoMarker::Class();
        (void)XMLFilterImpl::JxxClassInfoMarker::Class();
        (void)XMLReaderAdapter::JxxClassInfoMarker::Class();
        (void)XMLReaderFactory::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::org::xml::sax::helpers
