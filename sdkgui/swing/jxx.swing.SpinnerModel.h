#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "swing/event/jxx.swing.event.ChangeListener.h"
namespace jxx::swing
{
    class SpinnerModel : public ::jxx::lang::InterfaceBase<SpinnerModel>
    {
    public:
        ~SpinnerModel() override = default;
        virtual ::jxx::Ptr<::jxx::lang::Object> getValue() const = 0;
        virtual void setValue(const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
        virtual ::jxx::Ptr<::jxx::lang::Object> getNextValue() const = 0;
        virtual ::jxx::Ptr<::jxx::lang::Object> getPreviousValue() const = 0;
        virtual void addChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) = 0;
        virtual void removeChangeListener(const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) = 0;
    };
}
