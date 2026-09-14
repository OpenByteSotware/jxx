#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.MouseEventImpl.h"
namespace jxx::org::w3c::dom::events::internal {
MouseEventImpl::MouseEventImpl() = default;
MouseEventImpl::~MouseEventImpl() = default;
::jxx::lang::jint MouseEventImpl::getScreenX() const { return screenX_; }
::jxx::lang::jint MouseEventImpl::getScreenY() const { return screenY_; }
::jxx::lang::jint MouseEventImpl::getClientX() const { return clientX_; }
::jxx::lang::jint MouseEventImpl::getClientY() const { return clientY_; }
::jxx::lang::jbool MouseEventImpl::getCtrlKey() const { return ctrlKey_; }
::jxx::lang::jbool MouseEventImpl::getShiftKey() const { return shiftKey_; }
::jxx::lang::jbool MouseEventImpl::getAltKey() const { return altKey_; }
::jxx::lang::jbool MouseEventImpl::getMetaKey() const { return metaKey_; }
::jxx::lang::jshort MouseEventImpl::getButton() const { return button_; }
::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget> MouseEventImpl::getRelatedTarget() const { return relatedTarget_; }
void MouseEventImpl::initMouseEvent(const ::jxx::Ptr<::jxx::lang::String>& typeArg,
    ::jxx::lang::jbool canBubbleArg, ::jxx::lang::jbool cancelableArg,
    const ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView>& viewArg,
    ::jxx::lang::jint detailArg, ::jxx::lang::jint screenXArg,
    ::jxx::lang::jint screenYArg, ::jxx::lang::jint clientXArg,
    ::jxx::lang::jint clientYArg, ::jxx::lang::jbool ctrlKeyArg,
    ::jxx::lang::jbool altKeyArg, ::jxx::lang::jbool shiftKeyArg,
    ::jxx::lang::jbool metaKeyArg, ::jxx::lang::jshort buttonArg,
    const ::jxx::Ptr<::jxx::org::w3c::dom::events::EventTarget>& relatedTargetArg) {
    initUIEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg);
    screenX_ = screenXArg; screenY_ = screenYArg; clientX_ = clientXArg; clientY_ = clientYArg;
    ctrlKey_ = ctrlKeyArg; altKey_ = altKeyArg; shiftKey_ = shiftKeyArg; metaKey_ = metaKeyArg;
    button_ = buttonArg; relatedTarget_ = relatedTargetArg;
}
} // namespace jxx::org::w3c::dom::events::internal
