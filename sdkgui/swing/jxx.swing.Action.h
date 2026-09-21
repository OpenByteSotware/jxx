#pragma once

#include "awt/event/jxx.awt.event.ActionListener.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::beans
{
    class PropertyChangeListener;
}

namespace jxx::swing
{
    class Action : public ::jxx::lang::InterfaceBase<
        Action, ::jxx::awt::event::ActionListener>
    {
    public:
        static ::jxx::Ptr<::jxx::lang::String> NAME;
        static ::jxx::Ptr<::jxx::lang::String> SMALL_ICON;
        static ::jxx::Ptr<::jxx::lang::String> LARGE_ICON_KEY;
        static ::jxx::Ptr<::jxx::lang::String> SHORT_DESCRIPTION;
        static ::jxx::Ptr<::jxx::lang::String> LONG_DESCRIPTION;
        static ::jxx::Ptr<::jxx::lang::String> MNEMONIC_KEY;
        static ::jxx::Ptr<::jxx::lang::String> ACTION_COMMAND_KEY;
        static ::jxx::Ptr<::jxx::lang::String> ACCELERATOR_KEY;
        static ::jxx::Ptr<::jxx::lang::String> SELECTED_KEY;
        static ::jxx::Ptr<::jxx::lang::String> DISPLAYED_MNEMONIC_INDEX_KEY;

        ~Action() override = default;
        virtual ::jxx::Ptr<::jxx::lang::Object> getValue(
            const ::jxx::Ptr<::jxx::lang::String>& key) const = 0;
        virtual void putValue(
            const ::jxx::Ptr<::jxx::lang::String>& key,
            const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;
        virtual void setEnabled(::jxx::lang::jbool enabled) = 0;
        virtual ::jxx::lang::jbool isEnabled() const = 0;
        virtual void addPropertyChangeListener(
            const ::jxx::Ptr<::jxx::beans::PropertyChangeListener>& listener) = 0;
        virtual void removePropertyChangeListener(
            const ::jxx::Ptr<::jxx::beans::PropertyChangeListener>& listener) = 0;
    };
}
