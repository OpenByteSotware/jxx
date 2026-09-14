#pragma once
#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.EventImpl.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.UIEvent.h"
namespace jxx::org::w3c::dom::events::internal {
class UIEventImpl
    : public EventImpl
    , public virtual ::jxx::org::w3c::dom::events::UIEvent {
public:
    using JxxSuper = EventImpl;
    UIEventImpl();
    ~UIEventImpl() override;
    ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView> getView() const override;
    ::jxx::lang::jint getDetail() const override;
    void initUIEvent(const ::jxx::Ptr<::jxx::lang::String>& typeArg,
        ::jxx::lang::jbool canBubbleArg, ::jxx::lang::jbool cancelableArg,
        const ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView>& viewArg,
        ::jxx::lang::jint detailArg) override;
protected:
    ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView> view_;
    ::jxx::lang::jint detail_ = 0;
};
} // namespace jxx::org::w3c::dom::events::internal
