#pragma once
#include "swing/border/jxx.swing.border.AbstractBorder.h"
namespace jxx::swing::border {
class CompoundBorder : public ::jxx::lang::ClassBase<CompoundBorder, AbstractBorder> {
public:
    using JxxSuper = AbstractBorder;
    using Super = ::jxx::lang::ClassBase<CompoundBorder, JxxSuper>;
    CompoundBorder();
    CompoundBorder(const ::jxx::Ptr<Border>& outsideBorder,
                   const ::jxx::Ptr<Border>& insideBorder);
    ::jxx::lang::jbool isBorderOpaque() const override;
    ::jxx::Ptr<::jxx::awt::Insets> getBorderInsets(
        const ::jxx::Ptr<::jxx::awt::Component>& component) const override;
    ::jxx::Ptr<Border> getOutsideBorder() const;
    ::jxx::Ptr<Border> getInsideBorder() const;
private:
    ::jxx::Ptr<Border> outsideBorder_;
    ::jxx::Ptr<Border> insideBorder_;
};
}
