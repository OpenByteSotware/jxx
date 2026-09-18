#include "swing/jxx.swing.JMenuBar.h"
#include "swing/jxx.swing.JMenu.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::swing { JMenuBar::JMenuBar():Super(){} ::jxx::Ptr<JMenu> JMenuBar::add(const ::jxx::Ptr<JMenu>& menu){if(!menu)throw ::jxx::lang::NullPointerException("menu");menus_.push_back(menu);return menu;} ::jxx::lang::jint JMenuBar::getMenuCount()const{return static_cast<::jxx::lang::jint>(menus_.size());} ::jxx::Ptr<JMenu> JMenuBar::getMenu(::jxx::lang::jint index)const{if(index<0||index>=static_cast<::jxx::lang::jint>(menus_.size()))throw ::jxx::lang::IllegalArgumentException("index");return menus_[index];} void JMenuBar::setHelpMenu(const ::jxx::Ptr<JMenu>& menu){helpMenu_=menu;} ::jxx::Ptr<JMenu> JMenuBar::getHelpMenu()const{return helpMenu_;} }
