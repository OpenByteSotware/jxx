#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.ClassInitialization.h"

#include <mutex>

#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.DomUnmarshaller.h"
#include "ext/xml/bind/internal/jxx.ext.xml.bind.internal.XmlMarshaller.h"

namespace jxx::ext::xml::bind::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<DomUnmarshaller, ::jxx::lang::Object, ::jxx::ext::xml::bind::Unmarshaller>::Class();
        (void)::jxx::lang::ClassBase<XmlMarshaller, ::jxx::lang::Object, ::jxx::ext::xml::bind::Marshaller>::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::xml::bind::internal
