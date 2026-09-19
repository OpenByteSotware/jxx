#include "swing/border/jxx.swing.border.LineBorder.h"
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Insets.h"
namespace jxx::swing::border {
LineBorder::LineBorder(const ::jxx::Ptr<::jxx::awt::Color>& color)
    : LineBorder(color, 1, false) {}
LineBorder::LineBorder(const ::jxx::Ptr<::jxx::awt::Color>& color, ::jxx::lang::jint thickness)
    : LineBorder(color, thickness, false) {}
LineBorder::LineBorder(const ::jxx::Ptr<::jxx::awt::Color>& color,
                       ::jxx::lang::jint thickness, ::jxx::lang::jbool roundedCorners)
    : lineColor_(color), thickness_(thickness), roundedCorners_(roundedCorners) {}
::jxx::Ptr<Border> LineBorder::createBlackLineBorder() {
    return ::jxx::NEW<LineBorder>(::jxx::awt::Color::black);
}
::jxx::Ptr<Border> LineBorder::createGrayLineBorder() {
    return ::jxx::NEW<LineBorder>(::jxx::awt::Color::gray);
}
::jxx::Ptr<::jxx::awt::Insets> LineBorder::getBorderInsets(
    const ::jxx::Ptr<::jxx::awt::Component>&) const {
    return ::jxx::NEW<::jxx::awt::Insets>(thickness_, thickness_, thickness_, thickness_);
}
::jxx::lang::jbool LineBorder::isBorderOpaque() const { return !roundedCorners_; }
::jxx::Ptr<::jxx::awt::Color> LineBorder::getLineColor() const { return lineColor_; }
::jxx::lang::jint LineBorder::getThickness() const { return thickness_; }
::jxx::lang::jbool LineBorder::getRoundedCorners() const { return roundedCorners_; }
}
