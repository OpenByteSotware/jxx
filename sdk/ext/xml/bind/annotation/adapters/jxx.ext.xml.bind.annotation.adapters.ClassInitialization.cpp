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
        (void)::jxx::lang::InterfaceBase<XmlAdapter>::Class();
        (void)::jxx::lang::ClassBase<XmlJavaTypeAdapter, ::jxx::lang::Object>::Class();
        (void)::jxx::lang::ClassBase<XmlJavaTypeAdapters, ::jxx::lang::Object>::Class();
    });
}

} // namespace jxx::ext::xml::bind::annotation::adapters
