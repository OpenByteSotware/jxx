#pragma once
#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.UIEventImpl.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.MouseEvent.h"
namespace jxx::org::w3c::dom::events::internal {
class MouseEventImpl final
    : public UIEventImpl
    , public virtual ::jxx::org::w3c::dom::events::MouseEvent {
public:
    using JxxSuper = UIEventImpl;
    MouseEventImpl();
    ~MouseEventImpl() override;
    ::jxx::lang::jint getScreenX() const override;
    ::jxx::lang::jint getScreenY() const override;
    ::jxx::lang::jint getClientX() const override;
    ::jxx::lang::jint getClientY() const override;
    ::jxx::lang::jbool getCtrlKey() const override;
    ::jxx::lang::jbool getShiftKey() const override;
    ::jxx::lang::jbool getAltKey() const override;
    ::jxx::lang::jbool getMetaKey() const override;
    ::jxx::lang::jshort getButton() const override;
    ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> getRelatedTarget() const override;
    void initMouseEvent(const ::jxx::Ptr<::jxx::lang::String>& typeArg,
        ::jxx::lang::jbool canBubbleArg, ::jxx::lang::jbool cancelableArg,
        const ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView>& viewArg,
        ::jxx::lang::jint detailArg, ::jxx::lang::jint screenXArg,
        ::jxx::lang::jint screenYArg, ::jxx::lang::jint clientXArg,
        ::jxx::lang::jint clientYArg, ::jxx::lang::jbool ctrlKeyArg,
        ::jxx::lang::jbool altKeyArg, ::jxx::lang::jbool shiftKeyArg,
        ::jxx::lang::jbool metaKeyArg, ::jxx::lang::jshort buttonArg,
        const ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget>& relatedTargetArg) override;
private:
    ::jxx::lang::jint screenX_ = 0, screenY_ = 0, clientX_ = 0, clientY_ = 0;
    ::jxx::lang::jbool ctrlKey_ = false, shiftKey_ = false, altKey_ = false, metaKey_ = false;
    ::jxx::lang::jshort button_ = 0;
    ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> relatedTarget_;
};
} // namespace jxx::org::w3c::dom::events::internal
