#pragma once
#include <vector>
#include "swing/text/jxx.swing.text.JTextComponent.h"
#include "swing/jxx.swing.SwingConstants.h"
namespace jxx::awt::event { class ActionEvent; class ActionListener; }
namespace jxx::gui::internal { class WxFramePeer; class WxDialogPeer; }
namespace jxx::swing
{
    class JTextField : public ::jxx::lang::ClassBase<JTextField, ::jxx::swing::text::JTextComponent, SwingConstants>
    {
    public:
        using JxxSuper=::jxx::swing::text::JTextComponent; using Super=::jxx::lang::ClassBase<JTextField,JxxSuper,SwingConstants>;
        JTextField(); explicit JTextField(const ::jxx::Ptr<::jxx::lang::String>& text); explicit JTextField(::jxx::lang::jint columns); JTextField(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint columns);
        ::jxx::lang::jint getColumns() const; void setColumns(::jxx::lang::jint columns);
        ::jxx::lang::jint getHorizontalAlignment() const; void setHorizontalAlignment(::jxx::lang::jint alignment);
        void setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& command); ::jxx::Ptr<::jxx::lang::String> getActionCommand() const;
        void addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener); void removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
    protected: virtual void processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event);
    private:
        friend class ::jxx::gui::internal::WxFramePeer; friend class ::jxx::gui::internal::WxDialogPeer;
        void fireActionPerformed();
        ::jxx::lang::jint columns_=0; ::jxx::lang::jint alignment_=LEADING; ::jxx::Ptr<::jxx::lang::String> actionCommand_; std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> listeners_;
    };
}
