#include "org/w3c/dom/views/internal/jxx.org.w3c.dom.views.internal.AbstractViewImpl.h"

#include "org/w3c/dom/views/jxx.org.w3c.dom.views.DocumentView.h"

namespace jxx::org::w3c::dom::views::internal {

AbstractViewImpl::AbstractViewImpl(
    const ::jxx::Ptr<::jxx::org::w3c::dom::views::DocumentView>& document)
    : document_(document) {
}

AbstractViewImpl::~AbstractViewImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::views::DocumentView>
AbstractViewImpl::getDocument() const {
    return document_;
}

} // namespace jxx::org::w3c::dom::views::internal
