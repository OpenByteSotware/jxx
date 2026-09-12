#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.EventTarget.h"

namespace jxx::org::w3c::dom::events {

class Event {
public:
    virtual ~Event() = default;

    static constexpr ::jxx::lang::jshort CAPTURING_PHASE = 1;
    static constexpr ::jxx::lang::jshort AT_TARGET = 2;
    static constexpr ::jxx::lang::jshort BUBBLING_PHASE = 3;

    virtual ::jxx::Ptr<::jxx::lang::String> getType() const = 0;
    virtual ::jxx::Ptr<EventTarget> getTarget() const = 0;
    virtual ::jxx::Ptr<EventTarget> getCurrentTarget() const = 0;
    virtual ::jxx::lang::jshort getEventPhase() const = 0;
    virtual ::jxx::lang::jbool getBubbles() const = 0;
    virtual ::jxx::lang::jbool getCancelable() const = 0;
    virtual ::jxx::lang::jlong getTimeStamp() const = 0;
    virtual void stopPropagation() = 0;
    virtual void preventDefault() = 0;
    virtual void initEvent(
        const ::jxx::Ptr<::jxx::lang::String>& eventTypeArg,
        ::jxx::lang::jbool canBubbleArg,
        ::jxx::lang::jbool cancelableArg) = 0;
};

} // namespace jxx::org::w3c::dom::events
