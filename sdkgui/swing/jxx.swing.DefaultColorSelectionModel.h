#pragma once
#include <vector>
#include "awt/jxx.awt.Color.h"
#include "lang/jxx.lang.Object.h"
#include "swing/jxx.swing.ColorSelectionModel.h"
namespace jxx::swing
{
    class DefaultColorSelectionModel : public ::jxx::lang::ClassBase<
        DefaultColorSelectionModel, ::jxx::lang::Object, ColorSelectionModel>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<DefaultColorSelectionModel,
            JxxSuper, ColorSelectionModel>;
        DefaultColorSelectionModel();
        explicit DefaultColorSelectionModel(
            const ::jxx::Ptr<::jxx::awt::Color>& color);
        ::jxx::Ptr<::jxx::awt::Color> getSelectedColor() const override;
        void setSelectedColor(
            const ::jxx::Ptr<::jxx::awt::Color>& color) override;
        void addChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) override;
        void removeChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) override;
    private:
        void fireStateChanged();
        ::jxx::Ptr<::jxx::awt::Color> color_;
        std::vector<::jxx::Ptr<::jxx::swing::event::ChangeListener>> listeners_;
    };
}
