#include "awt/jxx.awt.Menu.h"
#include <algorithm>
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::awt {
Menu::Menu():Menu(::jxx::NEW<::jxx::lang::String>(""),false){} Menu::Menu(const ::jxx::Ptr<::jxx::lang::String>& label):Menu(label,false){} Menu::Menu(const ::jxx::Ptr<::jxx::lang::String>& label,::jxx::lang::jbool tearOff):Super(label),tearOff_(tearOff){}
::jxx::Ptr<MenuItem> Menu::add(const ::jxx::Ptr<MenuItem>& item){if(!item)throw ::jxx::lang::NullPointerException("item");items_.push_back(item);return item;} void Menu::add(const ::jxx::Ptr<::jxx::lang::String>& label){add(::jxx::NEW<MenuItem>(label));} void Menu::addSeparator(){add(::jxx::NEW<MenuItem>(::jxx::NEW<::jxx::lang::String>("-")));}
void Menu::insert(const ::jxx::Ptr<MenuItem>& item,::jxx::lang::jint index){if(!item)throw ::jxx::lang::NullPointerException("item");if(index<0||index>getItemCount())throw ::jxx::lang::IndexOutOfBoundsException("index");items_.insert(items_.begin()+index,item);} void Menu::insert(const ::jxx::Ptr<::jxx::lang::String>& label,::jxx::lang::jint index){insert(::jxx::NEW<MenuItem>(label),index);} void Menu::insertSeparator(::jxx::lang::jint index){insert(::jxx::NEW<MenuItem>(::jxx::NEW<::jxx::lang::String>("-")),index);}
void Menu::remove(::jxx::lang::jint index){if(index<0||index>=getItemCount())throw ::jxx::lang::IndexOutOfBoundsException("index");items_.erase(items_.begin()+index);} void Menu::remove(const ::jxx::Ptr<MenuComponent>& item){auto target=::jxx::CAST<MenuItem>(item);auto it=std::find(items_.begin(),items_.end(),target);if(it!=items_.end())items_.erase(it);} void Menu::removeAll(){items_.clear();}
::jxx::lang::jint Menu::getItemCount() const{return static_cast<::jxx::lang::jint>(items_.size());} ::jxx::Ptr<MenuItem> Menu::getItem(::jxx::lang::jint index) const{if(index<0||index>=getItemCount())throw ::jxx::lang::IndexOutOfBoundsException("index");return items_[static_cast<std::size_t>(index)];} ::jxx::lang::jbool Menu::isTearOff() const{return tearOff_;}
}
