#pragma once
#include <vector>
#include "swing/jxx.swing.SpinnerModel.h"
namespace jxx::swing
{
    class AbstractSpinnerModel : public ::jxx::lang::ClassBase<AbstractSpinnerModel, ::jxx::lang::Object, SpinnerModel>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<AbstractSpinnerModel, JxxSuper, SpinnerModel>;
        void addChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) override;
        void removeChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) override;
    protected:
        void fireStateChanged();
    private:
        std::vector<::jxx::Ptr<::jxx::swing::event::ChangeListener>> listeners_;
    };
}
