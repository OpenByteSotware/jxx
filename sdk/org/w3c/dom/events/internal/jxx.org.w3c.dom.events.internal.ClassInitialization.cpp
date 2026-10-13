#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.ClassInitialization.h"
#include <mutex>
#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.EventImpl.h"
namespace jxx::org::w3c::dom::events::internal {
void initializeClasses() { static std::once_flag flag; std::call_once(flag, [] {
 (void)::jxx::lang::ClassBase<EventImpl, ::jxx::lang::Object, ::jxx::org::w3c::dom::events::Event>::Class();
}); }
}
