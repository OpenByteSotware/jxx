#pragma once

#include <vector>

#include "awt/jxx.awt.Component.h"
#include "awt/jxx.awt.ItemSelectable.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.String.h"

namespace jxx::awt::event
{
    class ItemEvent;
    class ItemListener;
}

namespace jxx::awt
{
    class CheckboxGroup;

    class Checkbox :
        public ::jxx::lang::ClassBase<Checkbox, Component, ItemSelectable>
    {
    public:
        using JxxSuper = Component;
        using Super = ::jxx::lang::ClassBase<Checkbox,
            JxxSuper, ItemSelectable>;

        Checkbox();
        explicit Checkbox(const ::jxx::Ptr<::jxx::lang::String>& label);
        Checkbox(const ::jxx::Ptr<::jxx::lang::String>& label,
            ::jxx::lang::jbool state);
        Checkbox(const ::jxx::Ptr<::jxx::lang::String>& label,
            ::jxx::lang::jbool state,
            const ::jxx::Ptr<CheckboxGroup>& group);
        Checkbox(const ::jxx::Ptr<::jxx::lang::String>& label,
            const ::jxx::Ptr<CheckboxGroup>& group,
            ::jxx::lang::jbool state);

        ::jxx::Ptr<::jxx::lang::String> getLabel() const;
        void setLabel(const ::jxx::Ptr<::jxx::lang::String>& label);
        ::jxx::lang::jbool getState() const;
        void setState(::jxx::lang::jbool state);
        ::jxx::Ptr<CheckboxGroup> getCheckboxGroup() const;
        void setCheckboxGroup(const ::jxx::Ptr<CheckboxGroup>& group);

        ::jxx::lang::ObjectArray getSelectedObjects() const override;
        void addItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;
        void removeItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;

    protected:
        virtual void processItemEvent(
            const ::jxx::Ptr<::jxx::awt::event::ItemEvent>& event);

    private:
        friend class CheckboxGroup;
        void setStateFromGroup(::jxx::lang::jbool state);
        void fireItemEvent(::jxx::lang::jint stateChange);

        ::jxx::Ptr<::jxx::lang::String> label_;
        ::jxx::lang::jbool state_;
        ::jxx::Ptr<CheckboxGroup> group_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ItemListener>> listeners_;
    };
}
