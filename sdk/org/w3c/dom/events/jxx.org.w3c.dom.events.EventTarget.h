#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::w3c::dom::events {

class Event;
class EventListener;

class EventTarget {
public:
    virtual ~EventTarget() = default;

    virtual void addEventListener(
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<EventListener>& listener,
        ::jxx::lang::jbool useCapture) = 0;

    virtual void removeEventListener(
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<EventListener>& listener,
        ::jxx::lang::jbool useCapture) = 0;

    virtual ::jxx::lang::jbool dispatchEvent(
        const ::jxx::Ptr<Event>& evt) = 0;
};

} // namespace jxx::org::w3c::dom::events
