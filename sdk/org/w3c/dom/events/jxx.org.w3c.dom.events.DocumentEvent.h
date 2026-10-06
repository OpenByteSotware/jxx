#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::w3c::dom::events {

class Event;

class DocumentEvent : public ::jxx::lang::InterfaceBase<DocumentEvent> {
public:
    virtual ~DocumentEvent() = default;

    virtual ::jxx::Ptr<Event> createEvent(
        const ::jxx::Ptr<::jxx::lang::String>& eventType) = 0;
};

} // namespace jxx::org::w3c::dom::events
