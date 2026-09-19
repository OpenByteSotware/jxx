#pragma once
#include "swing/jxx.swing.JComponent.h"
namespace jxx::awt { class Color; }
namespace jxx::swing
{
    class ColorSelectionModel;
    class JColorChooser : public ::jxx::lang::ClassBase<JColorChooser, JComponent>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<JColorChooser, JxxSuper>;
        JColorChooser();
        explicit JColorChooser(const ::jxx::Ptr<::jxx::awt::Color>& color);
        explicit JColorChooser(const ::jxx::Ptr<ColorSelectionModel>& model);
        ::jxx::Ptr<::jxx::awt::Color> getColor() const;
        void setColor(const ::jxx::Ptr<::jxx::awt::Color>& color);
        void setColor(::jxx::lang::jint red, ::jxx::lang::jint green,
            ::jxx::lang::jint blue);
        void setColor(::jxx::lang::jint rgb);
        ::jxx::Ptr<ColorSelectionModel> getSelectionModel() const;
        void setSelectionModel(const ::jxx::Ptr<ColorSelectionModel>& model);
        void setPreviewPanel(const ::jxx::Ptr<JComponent>& preview);
        ::jxx::Ptr<JComponent> getPreviewPanel() const;
        static ::jxx::Ptr<::jxx::awt::Color> showDialog(
            const ::jxx::Ptr<::jxx::awt::Component>& parent,
            const ::jxx::Ptr<::jxx::lang::String>& title,
            const ::jxx::Ptr<::jxx::awt::Color>& initialColor);
    private:
        ::jxx::Ptr<ColorSelectionModel> model_;
        ::jxx::Ptr<JComponent> preview_;
    };
}
