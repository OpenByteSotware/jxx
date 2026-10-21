#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.ClassInitialization.h"

#include <mutex>

#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlAccessorType.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlAttribute.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElement.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementDecl.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementRef.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementRefs.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElements.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementWrapper.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlEnum.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlEnumValue.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlList.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlRegistry.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlRootElement.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlTransient.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlType.h"
#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlValue.h"

#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.ClassInitialization.h"
namespace jxx::ext::xml::bind::annotation {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)XmlAccessorType::JxxClassInfoMarker::Class();
        (void)XmlAttribute::JxxClassInfoMarker::Class();
        (void)XmlElement::JxxClassInfoMarker::Class();
        (void)XmlElementDecl::JxxClassInfoMarker::Class();
        (void)XmlElementRef::JxxClassInfoMarker::Class();
        (void)XmlElementRefs::JxxClassInfoMarker::Class();
        (void)XmlElements::JxxClassInfoMarker::Class();
        (void)XmlElementWrapper::JxxClassInfoMarker::Class();
        (void)XmlEnum::JxxClassInfoMarker::Class();
        (void)XmlEnumValue::JxxClassInfoMarker::Class();
        (void)XmlList::JxxClassInfoMarker::Class();
        (void)XmlRegistry::JxxClassInfoMarker::Class();
        (void)XmlRootElement::JxxClassInfoMarker::Class();
        (void)XmlTransient::JxxClassInfoMarker::Class();
        (void)XmlType::JxxClassInfoMarker::Class();
        (void)XmlValue::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
    ::jxx::ext::xml::bind::annotation::adapters::initialize();
}

} // namespace jxx::ext::xml::bind::annotation
