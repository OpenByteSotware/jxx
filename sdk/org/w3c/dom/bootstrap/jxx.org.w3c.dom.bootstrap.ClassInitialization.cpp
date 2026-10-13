#include "org/w3c/dom/bootstrap/jxx.org.w3c.dom.bootstrap.ClassInitialization.h"
#include <mutex>
#include "org/w3c/dom/bootstrap/jxx.org.w3c.dom.bootstrap.DOMImplementationRegistry.h"
namespace jxx::org::w3c::dom::bootstrap {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<
            DOMImplementationRegistry,
            ::jxx::lang::Object>::Class();
    });
}
} // namespace jxx::org::w3c::dom::bootstrap
