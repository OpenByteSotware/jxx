#include "awt/jxx.awt.MenuBar.h"
#include <algorithm>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::awt {
::jxx::Ptr<Menu> MenuBar::add(const ::jxx::Ptr<Menu>& menu){if(!menu)throw ::jxx::lang::NullPointerException("menu");menus_.push_back(menu);return menu;}
void MenuBar::remove(::jxx::lang::jint index){if(index<0||index>=getMenuCount())throw ::jxx::lang::IndexOutOfBoundsException("index");auto value=menus_[static_cast<std::size_t>(index)];menus_.erase(menus_.begin()+index);if(helpMenu_==value)helpMenu_.reset();}
void MenuBar::remove(const ::jxx::Ptr<MenuComponent>& component){auto menu=::jxx::CAST<Menu>(component);auto it=std::find(menus_.begin(),menus_.end(),menu);if(it!=menus_.end()){if(helpMenu_==*it)helpMenu_.reset();menus_.erase(it);}}
::jxx::lang::jint MenuBar::getMenuCount() const{return static_cast<::jxx::lang::jint>(menus_.size());} ::jxx::Ptr<Menu> MenuBar::getMenu(::jxx::lang::jint index) const{if(index<0||index>=getMenuCount())throw ::jxx::lang::IndexOutOfBoundsException("index");return menus_[static_cast<std::size_t>(index)];}
::jxx::Ptr<Menu> MenuBar::getHelpMenu() const{return helpMenu_;} void MenuBar::setHelpMenu(const ::jxx::Ptr<Menu>& menu){if(helpMenu_)remove(::jxx::CAST<MenuComponent>(helpMenu_));helpMenu_=menu;if(menu)add(menu);}
}
