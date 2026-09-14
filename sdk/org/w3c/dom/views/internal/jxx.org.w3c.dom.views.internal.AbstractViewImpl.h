#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/views/jxx.org.w3c.dom.views.AbstractView.h"

namespace jxx::org::w3c::dom::views {
class DocumentView;
}

namespace jxx::org::w3c::dom::views::internal {

class AbstractViewImpl final
    : public ::jxx::lang::ClassBase<
          AbstractViewImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::views::AbstractView> {
public:
    using JxxSuper = ::jxx::lang::Object;

    explicit AbstractViewImpl(
        const ::jxx::Ptr<::jxx::org::w3c::dom::views::DocumentView>& document);

    ~AbstractViewImpl() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::views::DocumentView>
    getDocument() const override;

private:
    ::jxx::Ptr<::jxx::org::w3c::dom::views::DocumentView> document_;
};

} // namespace jxx::org::w3c::dom::views::internal
