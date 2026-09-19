#include "swing/jxx.swing.JColorChooser.h"
#include "awt/jxx.awt.Color.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "swing/jxx.swing.ColorSelectionModel.h"
#include "swing/jxx.swing.DefaultColorSelectionModel.h"
namespace jxx::swing
{
    JColorChooser::JColorChooser()
        : JColorChooser(::jxx::awt::Color::white) {}
    JColorChooser::JColorChooser(const ::jxx::Ptr<::jxx::awt::Color>& color)
        : JColorChooser(::jxx::NEW<DefaultColorSelectionModel>(color)) {}
    JColorChooser::JColorChooser(const ::jxx::Ptr<ColorSelectionModel>& model)
        : Super() { setSelectionModel(model); }
    ::jxx::Ptr<::jxx::awt::Color> JColorChooser::getColor() const
    { return model_->getSelectedColor(); }
    void JColorChooser::setColor(const ::jxx::Ptr<::jxx::awt::Color>& color)
    { model_->setSelectedColor(color); }
    void JColorChooser::setColor(::jxx::lang::jint red,
        ::jxx::lang::jint green, ::jxx::lang::jint blue)
    { setColor(::jxx::NEW<::jxx::awt::Color>(red, green, blue)); }
    void JColorChooser::setColor(::jxx::lang::jint rgb)
    { setColor(::jxx::NEW<::jxx::awt::Color>(rgb)); }
    ::jxx::Ptr<ColorSelectionModel> JColorChooser::getSelectionModel() const
    { return model_; }
    void JColorChooser::setSelectionModel(
        const ::jxx::Ptr<ColorSelectionModel>& model)
    {
        if (!model) throw ::jxx::lang::NullPointerException("model");
        model_ = model;
    }
    void JColorChooser::setPreviewPanel(const ::jxx::Ptr<JComponent>& preview)
    { preview_ = preview; }
    ::jxx::Ptr<JComponent> JColorChooser::getPreviewPanel() const
    { return preview_; }
    ::jxx::Ptr<::jxx::awt::Color> JColorChooser::showDialog(
        const ::jxx::Ptr<::jxx::awt::Component>&,
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::awt::Color>& initialColor)
    { return initialColor; }
}
