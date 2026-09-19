#include "swing/border/jxx.swing.border.EmptyBorder.h"
#include "awt/jxx.awt.Insets.h"
namespace jxx::swing::border {
EmptyBorder::EmptyBorder(::jxx::lang::jint top, ::jxx::lang::jint left,
                         ::jxx::lang::jint bottom, ::jxx::lang::jint right)
    : top_(top), left_(left), bottom_(bottom), right_(right) {}
EmptyBorder::EmptyBorder(const ::jxx::Ptr<::jxx::awt::Insets>& value)
    : top_(value ? value->top : 0), left_(value ? value->left : 0),
      bottom_(value ? value->bottom : 0), right_(value ? value->right : 0) {}
::jxx::Ptr<::jxx::awt::Insets> EmptyBorder::getBorderInsets(
    const ::jxx::Ptr<::jxx::awt::Component>&) const {
    return ::jxx::NEW<::jxx::awt::Insets>(top_, left_, bottom_, right_);
}
::jxx::Ptr<::jxx::awt::Insets> EmptyBorder::getBorderInsets(
    const ::jxx::Ptr<::jxx::awt::Component>&,
    const ::jxx::Ptr<::jxx::awt::Insets>& insets) const {
    auto result = insets ? insets : ::jxx::NEW<::jxx::awt::Insets>(0, 0, 0, 0);
    result->set(top_, left_, bottom_, right_);
    return result;
}
::jxx::lang::jbool EmptyBorder::isBorderOpaque() const { return false; }
}
