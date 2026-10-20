#include "ext/xml/bind/jxx.ext.xml.bind.ClassInitialization.h"

#include <mutex>

#include "ext/xml/bind/jxx.ext.xml.bind.DatatypeConverter.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
#include "ext/xml/bind/jxx.ext.xml.bind.UnmarshalException.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBContext.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElementI.h"
#include "ext/xml/bind/jxx.ext.xml.bind.JAXBElement.h"
#include "lang/jxx.lang.Object.h"
#include "ext/xml/bind/jxx.ext.xml.bind.Marshaller.h"
#include "ext/xml/bind/jxx.ext.xml.bind.Unmarshaller.h"

namespace jxx::ext::xml::bind {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)DatatypeConverter::JxxClassInfoMarker::Class();
        (void)JAXBException::JxxClassInfoMarker::Class();
        (void)UnmarshalException::JxxClassInfoMarker::Class();
        (void)JAXBContext::JxxClassInfoMarker::Class();
        (void)JAXBElementI::JxxClassInfoMarker::Class();
        (void)JAXBElement<::jxx::lang::Object>::Class();
        (void)Marshaller::JxxClassInfoMarker::Class();
        (void)Unmarshaller::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::ext::xml::bind
