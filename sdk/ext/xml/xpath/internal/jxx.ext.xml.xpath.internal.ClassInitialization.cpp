#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.ClassInitialization.h"

#include <mutex>

#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.DomXPath.h"

namespace jxx::ext::xml::xpath::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            DomXPath,
            ::jxx::lang::Object,
            ::jxx::ext::xml::xpath::XPath>::Class();
    });
}

} // namespace jxx::ext::xml::xpath::internal
