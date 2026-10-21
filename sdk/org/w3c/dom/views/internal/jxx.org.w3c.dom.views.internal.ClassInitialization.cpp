#include "org/w3c/dom/views/internal/jxx.org.w3c.dom.views.internal.ClassInitialization.h"
#include <mutex>
#include "org/w3c/dom/views/internal/jxx.org.w3c.dom.views.internal.AbstractViewImpl.h"
namespace jxx::org::w3c::dom::views::internal {
void initializeClasses() { static std::once_flag flag; std::call_once(flag, [] {
 (void)::jxx::lang::ClassBase<AbstractViewImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::views::AbstractView>::Class();
}); }

void initialize()
{
    initializeClasses();
}

}
