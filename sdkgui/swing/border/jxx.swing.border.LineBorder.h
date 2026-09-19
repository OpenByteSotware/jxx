#pragma once
#include "swing/border/jxx.swing.border.AbstractBorder.h"
namespace jxx::awt { class Color; }
namespace jxx::swing::border {
class LineBorder : public ::jxx::lang::ClassBase<LineBorder, AbstractBorder> {
public:
    using JxxSuper = AbstractBorder;
    using Super = ::jxx::lang::ClassBase<LineBorder, JxxSuper>;
    explicit LineBorder(const ::jxx::Ptr<::jxx::awt::Color>& color);
    LineBorder(const ::jxx::Ptr<::jxx::awt::Color>& color, ::jxx::lang::jint thickness);
    LineBorder(const ::jxx::Ptr<::jxx::awt::Color>& color, ::jxx::lang::jint thickness,
               ::jxx::lang::jbool roundedCorners);
    static ::jxx::Ptr<Border> createBlackLineBorder();
    static ::jxx::Ptr<Border> createGrayLineBorder();
    ::jxx::Ptr<::jxx::awt::Insets> getBorderInsets(
        const ::jxx::Ptr<::jxx::awt::Component>& component) const override;
    ::jxx::lang::jbool isBorderOpaque() const override;
    ::jxx::Ptr<::jxx::awt::Color> getLineColor() const;
    ::jxx::lang::jint getThickness() const;
    ::jxx::lang::jbool getRoundedCorners() const;
private:
    ::jxx::Ptr<::jxx::awt::Color> lineColor_;
    ::jxx::lang::jint thickness_;
    ::jxx::lang::jbool roundedCorners_;
};
}
