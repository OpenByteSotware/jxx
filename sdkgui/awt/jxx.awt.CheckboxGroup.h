#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt
{
    class Checkbox;

    class CheckboxGroup :
        public ::jxx::lang::ClassBase<CheckboxGroup, ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<CheckboxGroup, JxxSuper>;

        CheckboxGroup() = default;

        ::jxx::Ptr<Checkbox> getSelectedCheckbox() const;
        void setSelectedCheckbox(const ::jxx::Ptr<Checkbox>& checkbox);

        ::jxx::Ptr<Checkbox> getCurrent() const;
        void setCurrent(const ::jxx::Ptr<Checkbox>& checkbox);

        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    private:
        ::jxx::Ptr<Checkbox> selectedCheckbox_;
    };
}
