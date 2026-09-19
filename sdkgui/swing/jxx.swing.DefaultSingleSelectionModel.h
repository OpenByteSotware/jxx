#pragma once
#include <vector>
#include "lang/jxx.lang.Object.h"
#include "swing/jxx.swing.SingleSelectionModel.h"
namespace jxx::swing
{
    class DefaultSingleSelectionModel : public ::jxx::lang::ClassBase<
        DefaultSingleSelectionModel, ::jxx::lang::Object, SingleSelectionModel>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<DefaultSingleSelectionModel, JxxSuper, SingleSelectionModel>;
        DefaultSingleSelectionModel();
        ::jxx::lang::jint getSelectedIndex() const override;
        void setSelectedIndex(::jxx::lang::jint index) override;
        void clearSelection() override;
        ::jxx::lang::jbool isSelected() const override;
        void addChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) override;
        void removeChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) override;
    private:
        void fireStateChanged();
        ::jxx::lang::jint index_;
        std::vector<::jxx::Ptr<::jxx::swing::event::ChangeListener>> listeners_;
    };
}
