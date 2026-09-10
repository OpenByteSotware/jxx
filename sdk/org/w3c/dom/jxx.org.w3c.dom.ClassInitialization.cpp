#include "org/w3c/dom/jxx.org.w3c.dom.ClassInitialization.h"

#include <mutex>

#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"

namespace jxx::org::w3c::dom {

void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)DOMException::Class();
    });
}

} // namespace jxx::org::w3c::dom
