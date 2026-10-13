#include "ext/xml/bind/jxx.ext.xml.bind.ClassInitialization.h"

#include <mutex>

#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
#include "ext/xml/bind/jxx.ext.xml.bind.UnmarshalException.h"

namespace jxx::ext::xml::bind {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)JAXBException::JxxClassInfoMarker::Class();
        (void)UnmarshalException::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::ext::xml::bind
