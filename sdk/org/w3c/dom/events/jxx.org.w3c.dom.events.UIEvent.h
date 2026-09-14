#pragma once

#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"

namespace jxx::org::w3c::dom::views {
class AbstractView;
}

namespace jxx::org::w3c::dom::events {

class UIEvent : public virtual Event {
public:
    ~UIEvent() override = default;

    virtual ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView>
    getView() const = 0;

    virtual ::jxx::lang::jint getDetail() const = 0;

    virtual void initUIEvent(
        const ::jxx::Ptr<::jxx::lang::String>& typeArg,
        ::jxx::lang::jbool canBubbleArg,
        ::jxx::lang::jbool cancelableArg,
        const ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView>& viewArg,
        ::jxx::lang::jint detailArg) = 0;
};

} // namespace jxx::org::w3c::dom::events
