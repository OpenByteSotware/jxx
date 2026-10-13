#include "ext/xml/namespace/jxx.ext.xml.namespace.ClassInitialization.h"

#include <mutex>

#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"

namespace jxx::ext::xml::namespace_ {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            QName,
            ::jxx::lang::Object>::Class();
    });
}

} // namespace jxx::ext::xml::namespace_
