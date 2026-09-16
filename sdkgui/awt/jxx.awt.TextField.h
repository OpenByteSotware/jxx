#pragma once
#include <vector>
#include "awt/jxx.awt.TextComponent.h"
namespace jxx::awt::event { class ActionEvent; class ActionListener; }
namespace jxx::awt {
class TextField : public ::jxx::lang::ClassBase<TextField,TextComponent> {
public:
 using JxxSuper=TextComponent; using Super=::jxx::lang::ClassBase<TextField,JxxSuper>;
 TextField(); explicit TextField(const ::jxx::Ptr<::jxx::lang::String>& text); explicit TextField(::jxx::lang::jint columns); TextField(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint columns);
 ~TextField() override=default;
 ::jxx::lang::jint getColumns() const; void setColumns(::jxx::lang::jint columns);
 ::jxx::lang::jchar getEchoChar() const; void setEchoChar(::jxx::lang::jchar character); ::jxx::lang::jbool echoCharIsSet() const;
 void setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& command); ::jxx::Ptr<::jxx::lang::String> getActionCommand() const;
 void addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener); void removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& listener);
protected: virtual void processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event); void fireActionPerformed();
private: ::jxx::lang::jint columns_; ::jxx::lang::jchar echoChar_; ::jxx::Ptr<::jxx::lang::String> actionCommand_; std::vector<::jxx::Ptr<::jxx::awt::event::ActionListener>> listeners_;
}; }
