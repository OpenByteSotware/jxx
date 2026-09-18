#pragma once
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SpinnerModel.h"
namespace jxx::swing
{
    class JSpinner : public ::jxx::lang::ClassBase<JSpinner,JComponent>
    {
    public:
        using JxxSuper=JComponent;using Super=::jxx::lang::ClassBase<JSpinner,JxxSuper>;
        explicit JSpinner(const ::jxx::Ptr<SpinnerModel>& model);
        void setModel(const ::jxx::Ptr<SpinnerModel>& model);::jxx::Ptr<SpinnerModel> getModel()const;
        ::jxx::Ptr<::jxx::lang::Object> getValue()const;void setValue(const ::jxx::Ptr<::jxx::lang::Object>& value);
        ::jxx::Ptr<::jxx::lang::Object> getNextValue()const;::jxx::Ptr<::jxx::lang::Object> getPreviousValue()const;
    private:
        ::jxx::Ptr<SpinnerModel> model_;
    };
}
