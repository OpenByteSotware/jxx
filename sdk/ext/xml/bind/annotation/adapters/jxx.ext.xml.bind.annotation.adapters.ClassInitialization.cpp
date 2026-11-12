#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.ClassInitialization.h"

#include <mutex>

#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlAdapter.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapter.h"
#include "ext/xml/bind/annotation/adapters/jxx.ext.xml.bind.annotation.adapters.XmlJavaTypeAdapters.h"

namespace jxx::ext::xml::bind::annotation::adapters {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
            ::jxx::lang::class_info_detail::registerClassInfo<XmlAdapter>();
            ::jxx::lang::class_info_detail::registerClassInfo<XmlJavaTypeAdapter>();
            ::jxx::lang::class_info_detail::registerClassInfo<XmlJavaTypeAdapters>();
        (void)::jxx::lang::InterfaceBase<XmlAdapter>::Class();
        (void)::jxx::lang::ClassBase<XmlJavaTypeAdapter, ::jxx::lang::Object>::Class();
        (void)::jxx::lang::ClassBase<XmlJavaTypeAdapters, ::jxx::lang::Object>::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::xml::bind::annotation::adapters
