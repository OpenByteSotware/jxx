#pragma once
#include <vector>
#include "awt/jxx.awt.Menu.h"
namespace jxx::awt {
class MenuBar : public ::jxx::lang::ClassBase<MenuBar,MenuComponent> {
public:
 using JxxSuper=MenuComponent; using Super=::jxx::lang::ClassBase<MenuBar,JxxSuper>;
 MenuBar()=default;
 ::jxx::Ptr<Menu> add(const ::jxx::Ptr<Menu>& menu); void remove(::jxx::lang::jint index); void remove(const ::jxx::Ptr<MenuComponent>& component);
 ::jxx::lang::jint getMenuCount() const; ::jxx::Ptr<Menu> getMenu(::jxx::lang::jint index) const;
 ::jxx::Ptr<Menu> getHelpMenu() const; void setHelpMenu(const ::jxx::Ptr<Menu>& menu);
private: std::vector<::jxx::Ptr<Menu>> menus_; ::jxx::Ptr<Menu> helpMenu_;
}; }
