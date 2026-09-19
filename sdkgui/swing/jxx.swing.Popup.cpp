#include "swing/jxx.swing.Popup.h"
namespace jxx::swing
{
    Popup::Popup() = default;
    Popup::Popup(const ::jxx::Ptr<::jxx::awt::Component>& owner,
                 const ::jxx::Ptr<::jxx::awt::Component>& contents,
                 ::jxx::lang::jint x, ::jxx::lang::jint y)
        : owner_(owner), contents_(contents), x_(x), y_(y) {}
    void Popup::show() { visible_ = true; if (contents_) { contents_->setLocation(x_, y_); contents_->setVisible(true); } }
    void Popup::hide() { visible_ = false; if (contents_) contents_->setVisible(false); }
    ::jxx::lang::jbool Popup::isVisible() const noexcept { return visible_; }
}
