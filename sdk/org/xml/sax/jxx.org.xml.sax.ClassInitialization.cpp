#include "org/xml/sax/jxx.org.xml.sax.ClassInitialization.h"

#include <mutex>

#include "org/xml/sax/jxx.org.xml.sax.InputSource.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"
#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

namespace jxx::org::xml::sax {

void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)InputSource::Class();
        (void)SAXException::Class();
        (void)SAXParseException::Class();
    });
}

} // namespace jxx::org::xml::sax
