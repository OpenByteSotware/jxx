#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.ClassInitialization.h"
#include <mutex>
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationImpl.h"
#include "org/w3c/dom/internal/jxx.org.w3c.dom.internal.DOMImplementationSourceImpl.h"
namespace jxx::org::w3c::dom::internal {
void initializeClasses() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<DOMImplementationImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::DOMImplementation>::Class();
        (void)::jxx::lang::ClassBase<DOMImplementationSourceImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::DOMImplementationSource>::Class();
    });
}

void initialize()
{
    initializeClasses();
}

}
