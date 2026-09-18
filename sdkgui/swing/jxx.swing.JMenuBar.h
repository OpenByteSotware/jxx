#pragma once
#include <vector>
#include "swing/jxx.swing.JComponent.h"
namespace jxx::swing { class JMenu; class JMenuBar : public ::jxx::lang::ClassBase<JMenuBar,JComponent> { public: using JxxSuper=JComponent; using Super=::jxx::lang::ClassBase<JMenuBar,JxxSuper>; JMenuBar(); ::jxx::Ptr<JMenu> add(const ::jxx::Ptr<JMenu>& menu); ::jxx::lang::jint getMenuCount() const; ::jxx::Ptr<JMenu> getMenu(::jxx::lang::jint index) const; void setHelpMenu(const ::jxx::Ptr<JMenu>& menu); ::jxx::Ptr<JMenu> getHelpMenu() const; private: std::vector<::jxx::Ptr<JMenu>> menus_; ::jxx::Ptr<JMenu> helpMenu_; }; }
