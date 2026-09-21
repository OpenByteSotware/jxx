#pragma once

#include "awt/jxx.awt.ItemSelectable.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "awt/event/jxx.awt.event.ItemListener.h"
#include "swing/event/jxx.swing.event.ChangeListener.h"

namespace jxx::swing
{
    class ButtonGroup;

    class ButtonModel : public ::jxx::lang::InterfaceBase<
        ButtonModel, ::jxx::awt::ItemSelectable>
    {
    public:
        ~ButtonModel() override = default;

        virtual ::jxx::lang::jbool isArmed() const = 0;
        virtual void setArmed(::jxx::lang::jbool armed) = 0;
        virtual ::jxx::lang::jbool isSelected() const = 0;
        virtual void setSelected(::jxx::lang::jbool selected) = 0;
        virtual ::jxx::lang::jbool isEnabled() const = 0;
        virtual void setEnabled(::jxx::lang::jbool enabled) = 0;
        virtual ::jxx::lang::jbool isPressed() const = 0;
        virtual void setPressed(::jxx::lang::jbool pressed) = 0;
        virtual ::jxx::lang::jbool isRollover() const = 0;
        virtual void setRollover(::jxx::lang::jbool rollover) = 0;
        virtual void setMnemonic(::jxx::lang::jint key) = 0;
        virtual ::jxx::lang::jint getMnemonic() const = 0;
        virtual void setActionCommand(
            const ::jxx::Ptr<::jxx::lang::String>& command) = 0;
        virtual ::jxx::Ptr<::jxx::lang::String> getActionCommand() const = 0;
        virtual void setGroup(const ::jxx::Ptr<ButtonGroup>& group) = 0;
        virtual void addActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener) = 0;
        virtual void removeActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener) = 0;
        virtual void addChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) = 0;
        virtual void removeChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) = 0;
    };
}
