#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.UIEventImpl.h"
#include "org/w3c/dom/views/jxx.org.w3c.dom.views.AbstractView.h"
namespace jxx::org::w3c::dom::events::internal {
UIEventImpl::UIEventImpl() = default;
UIEventImpl::~UIEventImpl() = default;
::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView> UIEventImpl::getView() const { return view_; }
::jxx::lang::jint UIEventImpl::getDetail() const { return detail_; }
void UIEventImpl::initUIEvent(const ::jxx::Ptr<::jxx::lang::String>& typeArg,
    ::jxx::lang::jbool canBubbleArg, ::jxx::lang::jbool cancelableArg,
    const ::jxx::Ptr<::jxx::org::w3c::dom::views::AbstractView>& viewArg,
    ::jxx::lang::jint detailArg) {
    initEvent(typeArg, canBubbleArg, cancelableArg);
    view_ = viewArg;
    detail_ = detailArg;
}
} // namespace jxx::org::w3c::dom::events::internal
