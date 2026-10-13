#include <mutex>
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.ClassInitialization.h"
#include "lang/jxx.lang.ClassInfo.h"

#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"

namespace jxx::org::w3c::dom::ls {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::org::w3c::dom::ls::LSException::JxxClassInfoMarker::Class();
    });
}

} // namespace jxx::org::w3c::dom::ls
