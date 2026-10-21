#include "org/w3c/dom/events/jxx.org.w3c.dom.events.ClassInitialization.h"

#include <mutex>

#include "org/w3c/dom/events/jxx.org.w3c.dom.events.DocumentEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventException.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventListener.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventTarget.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.MouseEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.MutationEvent.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.UIEvent.h"

#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.ClassInitialization.h"
namespace jxx::org::w3c::dom::events {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)DocumentEvent::JxxClassInfoMarker::Class();
        (void)Event::JxxClassInfoMarker::Class();
        (void)EventException::JxxClassInfoMarker::Class();
        (void)EventListener::JxxClassInfoMarker::Class();
        (void)EventTarget::JxxClassInfoMarker::Class();
        (void)MouseEvent::JxxClassInfoMarker::Class();
        (void)MutationEvent::JxxClassInfoMarker::Class();
        (void)UIEvent::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
    ::jxx::org::w3c::dom::events::internal::initialize();
}

} // namespace jxx::org::w3c::dom::events
