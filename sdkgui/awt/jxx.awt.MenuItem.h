#pragma once
#include <vector>
#include "awt/jxx.awt.MenuComponent.h"
#include "awt/jxx.awt.MenuShortcut.h"
namespace jxx::awt::event { class ActionEvent; class ActionListener; }
namespace jxx::gui::internal { class WxFramePeer; }
namespace jxx::awt {
class MenuItem : public ::jxx::lang::ClassBase<MenuItem,MenuComponent> {
public:
 using JxxSuper=MenuComponent; using Super=::jxx::lang::ClassBase<MenuItem,JxxSuper>;
 MenuItem(); explicit MenuItem(const ::jxx::Ptr<::jxx::lang::String>& label); MenuItem(const ::jxx::Ptr<::jxx::lang::String>& label,const ::jxx::Ptr<MenuShortcut>& shortcut); ~MenuItem() override=default;
 ::jxx::Ptr<::jxx::lang::String> getLabel() const; virtual void setLabel(const ::jxx::Ptr<::jxx::lang::String>& label);
 ::jxx::lang::jbool isEnabled() const; void setEnabled(::jxx::lang::jbool enabled);
 ::jxx::Ptr<MenuShortcut> getShortcut() const; void setShortcut(const ::jxx::Ptr<MenuShortcut>& shortcut); void deleteShortcut();
 void setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& command); ::jxx::Ptr<::jxx::lang::String> getActionCommand() const;
 void addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener); void removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
protected: virtual void processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event); void fireActionPerformed();
private:
 friend class ::jxx::gui::internal::WxFramePeer;
 ::jxx::Ptr<::jxx::lang::String> label_; ::jxx::Ptr<::jxx::lang::String> actionCommand_; ::jxx::Ptr<MenuShortcut> shortcut_; ::jxx::lang::jbool enabled_; std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> listeners_;
}; }
