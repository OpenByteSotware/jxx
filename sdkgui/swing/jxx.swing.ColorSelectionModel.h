#pragma once
#include "lang/jxx.lang.ClassInfo.h"
namespace jxx::awt { class Color; }
namespace jxx::swing::event { class ChangeListener; }
namespace jxx::swing
{
    class ColorSelectionModel : public ::jxx::lang::InterfaceBase<ColorSelectionModel>
    {
    public:
        ~ColorSelectionModel() override = default;
        virtual ::jxx::Ptr<::jxx::awt::Color> getSelectedColor() const = 0;
        virtual void setSelectedColor(const ::jxx::Ptr<::jxx::awt::Color>& color) = 0;
        virtual void addChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) = 0;
        virtual void removeChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) = 0;
    };
}
