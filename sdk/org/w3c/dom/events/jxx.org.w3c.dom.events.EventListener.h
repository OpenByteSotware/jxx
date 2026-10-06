#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx.lang.Object.h"

namespace jxx::org::w3c::dom::events {

class Event;

class EventListener : public ::jxx::lang::InterfaceBase<EventListener> {
public:
    virtual ~EventListener() = default;

    virtual void handleEvent(
        const ::jxx::Ptr<Event>& evt) = 0;
};

} // namespace jxx::org::w3c::dom::events
