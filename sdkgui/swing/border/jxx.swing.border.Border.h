#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::awt { class Component; class Graphics; class Insets; }
namespace jxx::swing::border {
class Border : public ::jxx::lang::InterfaceBase<Border> {
public:
    ~Border() override = default;
    virtual void paintBorder(const ::jxx::Ptr<::jxx::awt::Component>& component,
                             const ::jxx::Ptr<::jxx::awt::Graphics>& graphics,
                             ::jxx::lang::jint x, ::jxx::lang::jint y,
                             ::jxx::lang::jint width, ::jxx::lang::jint height) = 0;
    virtual ::jxx::Ptr<::jxx::awt::Insets> getBorderInsets(
        const ::jxx::Ptr<::jxx::awt::Component>& component) const = 0;
    virtual ::jxx::lang::jbool isBorderOpaque() const = 0;
};
}
