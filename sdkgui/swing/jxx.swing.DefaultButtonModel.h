#pragma once

#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "swing/jxx.swing.ButtonModel.h"

namespace jxx::swing
{
    class DefaultButtonModel : public ::jxx::lang::ClassBase<
        DefaultButtonModel, ::jxx::lang::Object, ButtonModel,
        ::jxx::io::SerializableI>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<DefaultButtonModel,
            JxxSuper, ButtonModel, ::jxx::io::SerializableI>;

        DefaultButtonModel();
        ~DefaultButtonModel() override = default;

        ::jxx::lang::jbool isArmed() const override;
        void setArmed(::jxx::lang::jbool armed) override;
        ::jxx::lang::jbool isSelected() const override;
        void setSelected(::jxx::lang::jbool selected) override;
        ::jxx::lang::jbool isEnabled() const override;
        void setEnabled(::jxx::lang::jbool enabled) override;
        ::jxx::lang::jbool isPressed() const override;
        void setPressed(::jxx::lang::jbool pressed) override;
        ::jxx::lang::jbool isRollover() const override;
        void setRollover(::jxx::lang::jbool rollover) override;
        void setMnemonic(::jxx::lang::jint key) override;
        ::jxx::lang::jint getMnemonic() const override;
        void setActionCommand(
            const ::jxx::Ptr<::jxx::lang::String>& command) override;
        ::jxx::Ptr<::jxx::lang::String> getActionCommand() const override;
        void setGroup(const ::jxx::Ptr<ButtonGroup>& group) override;

        ::jxx::lang::ObjectArray getSelectedObjects() const override;
        void addActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener) override;
        void removeActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener) override;
        void addItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;
        void removeItemListener(
            const ::jxx::Ptr<::jxx::awt::event::ItemListener>& listener) override;
        void addChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) override;
        void removeChangeListener(
            const ::jxx::Ptr<::jxx::swing::event::ChangeListener>& listener) override;

        void writeObject(
            const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
        void readObject(
            const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
        void readObjectNoData() override;

    protected:
        void fireStateChanged();
        void fireActionPerformed();
        void fireItemStateChanged();

    private:
        ::jxx::lang::jbool armed_ = false;
        ::jxx::lang::jbool selected_ = false;
        ::jxx::lang::jbool enabled_ = true;
        ::jxx::lang::jbool pressed_ = false;
        ::jxx::lang::jbool rollover_ = false;
        ::jxx::lang::jint mnemonic_ = 0;
        ::jxx::Ptr<::jxx::lang::String> actionCommand_;
        std::weak_ptr<ButtonGroup> group_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> actionListeners_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ItemListener>> itemListeners_;
        std::vector<::jxx::Ptr<::jxx::swing::event::ChangeListener>> changeListeners_;
    };
}
