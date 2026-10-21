#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.ClassInitialization.h"

#include <mutex>

#include "ext/xml/transform/dom/jxx.ext.xml.transform.dom.DOMSource.h"

namespace jxx::ext::xml::transform::dom {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            DOMSource,
            ::jxx::lang::Object,
            ::jxx::ext::xml::transform::Source>::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::xml::transform::dom
