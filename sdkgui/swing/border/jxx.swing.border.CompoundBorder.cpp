#include "swing/border/jxx.swing.border.CompoundBorder.h"
#include "awt/jxx.awt.Insets.h"
namespace jxx::swing::border {
CompoundBorder::CompoundBorder() = default;
CompoundBorder::CompoundBorder(const ::jxx::Ptr<Border>& outsideBorder,
                               const ::jxx::Ptr<Border>& insideBorder)
    : outsideBorder_(outsideBorder), insideBorder_(insideBorder) {}
::jxx::lang::jbool CompoundBorder::isBorderOpaque() const {
    return (!outsideBorder_ || outsideBorder_->isBorderOpaque()) &&
           (!insideBorder_ || insideBorder_->isBorderOpaque());
}
::jxx::Ptr<::jxx::awt::Insets> CompoundBorder::getBorderInsets(
    const ::jxx::Ptr<::jxx::awt::Component>& component) const {
    auto outside = outsideBorder_ ? outsideBorder_->getBorderInsets(component)
                                  : ::jxx::NEW<::jxx::awt::Insets>(0, 0, 0, 0);
    auto inside = insideBorder_ ? insideBorder_->getBorderInsets(component)
                                : ::jxx::NEW<::jxx::awt::Insets>(0, 0, 0, 0);
    return ::jxx::NEW<::jxx::awt::Insets>(outside->top + inside->top,
                                          outside->left + inside->left,
                                          outside->bottom + inside->bottom,
                                          outside->right + inside->right);
}
::jxx::Ptr<Border> CompoundBorder::getOutsideBorder() const { return outsideBorder_; }
::jxx::Ptr<Border> CompoundBorder::getInsideBorder() const { return insideBorder_; }
}
