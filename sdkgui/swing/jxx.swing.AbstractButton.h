#pragma once
#include <vector>
#include "swing/jxx.swing.JComponent.h"
#include "swing/jxx.swing.SwingConstants.h"
namespace jxx::awt::event { class ActionEvent; class ActionListener; }
namespace jxx::gui::internal { class WxFramePeer; class WxDialogPeer; }
namespace jxx::swing
{
    class AbstractButton : public ::jxx::lang::ClassBase<AbstractButton, JComponent, SwingConstants>
    {
    public:
        using JxxSuper = JComponent;
        using Super = ::jxx::lang::ClassBase<AbstractButton, JxxSuper, SwingConstants>;
        ~AbstractButton() override = default;
        ::jxx::Ptr<::jxx::lang::String> getText() const;
        void setText(const ::jxx::Ptr<::jxx::lang::String>& text);
        void setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& command);
        ::jxx::Ptr<::jxx::lang::String> getActionCommand() const;
        void addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
        void removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
        void doClick();
    protected:
        AbstractButton();
        explicit AbstractButton(const ::jxx::Ptr<::jxx::lang::String>& text);
        virtual void processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event);
    private:
        friend class ::jxx::gui::internal::WxFramePeer;
        friend class ::jxx::gui::internal::WxDialogPeer;
        void fireActionPerformed();
        ::jxx::Ptr<::jxx::lang::String> text_;
        ::jxx::Ptr<::jxx::lang::String> actionCommand_;
        std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> listeners_;
    };
}
