#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.ClassInitialization.h"

#include <mutex>

#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.NodeFilter.h"

namespace jxx::org::w3c::dom::traversal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::InterfaceBase<NodeFilter>::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::org::w3c::dom::traversal
