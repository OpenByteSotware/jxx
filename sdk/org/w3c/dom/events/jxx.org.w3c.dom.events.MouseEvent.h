#pragma once

#include "org/w3c/dom/events/jxx.org.w3c.dom.events.UIEvent.h"

namespace jxx::org::w3c::dom::events {

class EventTarget;

class MouseEvent : public virtual UIEvent {
public:
    ~MouseEvent() override = default;

    virtual ::jxx::lang::jint getScreenX() const = 0;
    virtual ::jxx::lang::jint getScreenY() const = 0;
    virtual ::jxx::lang::jint getClientX() const = 0;
    virtual ::jxx::lang::jint getClientY() const = 0;
    virtual ::jxx::lang::jbool getCtrlKey() const = 0;
    virtual ::jxx::lang::jbool getShiftKey() const = 0;
    virtual ::jxx::lang::jbool getAltKey() const = 0;
    virtual ::jxx::lang::jbool getMetaKey() const = 0;
    virtual ::jxx::lang::jshort getButton() const = 0;
    virtual ::jxx::Ptr<EventTarget> getRelatedTarget() const = 0;

    virtual void initMouseEvent(
        const ::jxx::Ptr<::jxx::lang::String>& typeArg,
        ::jxx::lang::jbool canBubbleArg,
        ::jxx::lang::jbool cancelableArg,
        const ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView>& viewArg,
        ::jxx::lang::jint detailArg,
        ::jxx::lang::jint screenXArg,
        ::jxx::lang::jint screenYArg,
        ::jxx::lang::jint clientXArg,
        ::jxx::lang::jint clientYArg,
        ::jxx::lang::jbool ctrlKeyArg,
        ::jxx::lang::jbool altKeyArg,
        ::jxx::lang::jbool shiftKeyArg,
        ::jxx::lang::jbool metaKeyArg,
        ::jxx::lang::jshort buttonArg,
        const ::jxx::Ptr<EventTarget>& relatedTargetArg) = 0;
};

} // namespace jxx::org::w3c::dom::events
