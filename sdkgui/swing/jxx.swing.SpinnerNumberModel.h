#pragma once

#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.Object.h"
#include "swing/jxx.swing.AbstractSpinnerModel.h"

namespace jxx::swing
{
    class SpinnerNumberModel : public ::jxx::lang::ClassBase<
        SpinnerNumberModel, AbstractSpinnerModel>
    {
    public:
        using JxxSuper = AbstractSpinnerModel;
        using Super = ::jxx::lang::ClassBase<SpinnerNumberModel, JxxSuper>;

        SpinnerNumberModel();
        SpinnerNumberModel(
            ::jxx::lang::jint value,
            ::jxx::lang::jint minimum,
            ::jxx::lang::jint maximum,
            ::jxx::lang::jint stepSize);
        SpinnerNumberModel(
            ::jxx::lang::jdouble value,
            ::jxx::lang::jdouble minimum,
            ::jxx::lang::jdouble maximum,
            ::jxx::lang::jdouble stepSize);
        SpinnerNumberModel(
            const ::jxx::Ptr<::jxx::lang::Number>& value,
            const ::jxx::Ptr<::jxx::lang::Object>& minimum,
            const ::jxx::Ptr<::jxx::lang::Object>& maximum,
            const ::jxx::Ptr<::jxx::lang::Number>& stepSize);

        ::jxx::Ptr<::jxx::lang::Object> getValue() const override;
        void setValue(
            const ::jxx::Ptr<::jxx::lang::Object>& value) override;
        ::jxx::Ptr<::jxx::lang::Object> getNextValue() const override;
        ::jxx::Ptr<::jxx::lang::Object> getPreviousValue() const override;

        ::jxx::Ptr<::jxx::lang::Number> getNumber() const;
        ::jxx::Ptr<::jxx::lang::Object> getMinimum() const;
        void setMinimum(
            const ::jxx::Ptr<::jxx::lang::Object>& minimum);
        ::jxx::Ptr<::jxx::lang::Object> getMaximum() const;
        void setMaximum(
            const ::jxx::Ptr<::jxx::lang::Object>& maximum);
        ::jxx::Ptr<::jxx::lang::Number> getStepSize() const;
        void setStepSize(
            const ::jxx::Ptr<::jxx::lang::Number>& stepSize);

    private:
        ::jxx::Ptr<::jxx::lang::Number> adjacent(
            ::jxx::lang::jint direction) const;
        static ::jxx::lang::jint compareNumbers(
            const ::jxx::Ptr<::jxx::lang::Number>& left,
            const ::jxx::Ptr<::jxx::lang::Object>& right);

        ::jxx::Ptr<::jxx::lang::Number> value_;
        ::jxx::Ptr<::jxx::lang::Object> minimum_;
        ::jxx::Ptr<::jxx::lang::Object> maximum_;
        ::jxx::Ptr<::jxx::lang::Number> stepSize_;
    };
}
