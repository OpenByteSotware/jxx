#pragma once
#include "swing/border/jxx.swing.border.AbstractBorder.h"
namespace jxx::swing::border {
class EmptyBorder : public ::jxx::lang::ClassBase<EmptyBorder, AbstractBorder> {
public:
    using JxxSuper = AbstractBorder;
    using Super = ::jxx::lang::ClassBase<EmptyBorder, JxxSuper>;
    EmptyBorder(::jxx::lang::jint top, ::jxx::lang::jint left,
                ::jxx::lang::jint bottom, ::jxx::lang::jint right);
    explicit EmptyBorder(const ::jxx::Ptr<::jxx::awt::Insets>& borderInsets);
    ::jxx::Ptr<::jxx::awt::Insets> getBorderInsets(
        const ::jxx::Ptr<::jxx::awt::Component>& component) const override;
    ::jxx::Ptr<::jxx::awt::Insets> getBorderInsets(
        const ::jxx::Ptr<::jxx::awt::Component>& component,
        const ::jxx::Ptr<::jxx::awt::Insets>& insets) const override;
    ::jxx::lang::jbool isBorderOpaque() const override;
protected:
    ::jxx::lang::jint top_;
    ::jxx::lang::jint left_;
    ::jxx::lang::jint bottom_;
    ::jxx::lang::jint right_;
};
}
