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

#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.ClassInitialization.h"
#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.ClassInitialization.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ClassInitialization.h"
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


void initialize()
{
    initializeClasses();
    ::jxx::ext::xml::bind::annotation::initialize();
    ::jxx::ext::xml::bind::internal::initialize();
    ::jxx::ext::xml::bind::metadata::initialize();
}

} // namespace jxx::ext::xml::bind
