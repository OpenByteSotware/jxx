#pragma once
#include <vector>
#include "awt/jxx.awt.MenuItem.h"
namespace jxx::awt {
class Menu : public ::jxx::lang::ClassBase<Menu,MenuItem> {
public:
 using JxxSuper=MenuItem; using Super=::jxx::lang::ClassBase<Menu,JxxSuper>;
 Menu(); explicit Menu(const ::jxx::Ptr<::jxx::lang::String>& label); Menu(const ::jxx::Ptr<::jxx::lang::String>& label,::jxx::lang::jbool tearOff);
 ::jxx::Ptr<MenuItem> add(const ::jxx::Ptr<MenuItem>& item); void add(const ::jxx::Ptr<::jxx::lang::String>& label); void addSeparator();
 void insert(const ::jxx::Ptr<MenuItem>& item,::jxx::lang::jint index); void insert(const ::jxx::Ptr<::jxx::lang::String>& label,::jxx::lang::jint index); void insertSeparator(::jxx::lang::jint index);
 void remove(::jxx::lang::jint index); void remove(const ::jxx::Ptr<MenuComponent>& item); void removeAll();
 ::jxx::lang::jint getItemCount() const; ::jxx::Ptr<MenuItem> getItem(::jxx::lang::jint index) const; ::jxx::lang::jbool isTearOff() const;
private: ::jxx::lang::jbool tearOff_; std::vector<::jxx::Ptr<MenuItem>> items_;
}; }
