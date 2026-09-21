#pragma once

#include <vector>

#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"

namespace jxx::awt::event { class ActionEvent; class ActionListener; }
namespace jxx::gui::internal { class WxFramePeer; class WxDialogPeer; }
namespace jxx::swing
{
    class ButtonModel;

    class AbstractButton : public ::jxx::lang::ClassBase<AbstractButton,
        JComponent, SwingConstants>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<AbstractButton,
            JxxSuper, SwingConstants>;

        ~AbstractButton() override = default;

        ::jxx::Ptr<::jxx::lang::String> getText() const;
        void setText(const ::jxx::Ptr<::jxx::lang::String>& text);
        void setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& command);
        ::jxx::Ptr<::jxx::lang::String> getActionCommand() const;
        void addActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
        void removeActionListener(
            const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);

        ::jxx::Ptr<ButtonModel> getModel() const;
        virtual void setModel(const ::jxx::Ptr<ButtonModel>& model);
        ::jxx::lang::jbool isSelected() const;
        virtual void setSelected(::jxx::lang::jbool selected);
        ::jxx::lang::jbool isArmed() const;
        ::jxx::lang::jbool isPressed() const;
        ::jxx::lang::jbool isRollover() const;
        void setRolloverEnabled(::jxx::lang::jbool enabled);
        ::jxx::lang::jbool isRolloverEnabled() const;
        void setMnemonic(::jxx::lang::jint mnemonic);
        ::jxx::lang::jint getMnemonic() const;
        void setEnabled(::jxx::lang::jbool enabled) override;
        virtual void doClick();

    protected:
        AbstractButton();
        explicit AbstractButton(const ::jxx::Ptr<::jxx::lang::String>& text);
        virtual void processActionEvent(
            const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event);

    private:
        friend class ::jxx::gui::internal::WxFramePeer;
        friend class ::jxx::gui::internal::WxDialogPeer;
        void fireActionPerformed();
        ::jxx::Ptr<::jxx::lang::String> text_;
        ::jxx::Ptr<::jxx::lang::String> actionCommand_;
        ::jxx::Ptr<ButtonModel> model_;
        ::jxx::lang::jbool rolloverEnabled_ = false;
        std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> listeners_;
    };
}
