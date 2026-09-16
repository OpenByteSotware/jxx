#pragma once
#include "awt/jxx.awt.MenuItem.h"
namespace jxx::awt {
class CheckboxMenuItem : public ::jxx::lang::ClassBase<CheckboxMenuItem,MenuItem> {
public:
 using JxxSuper=MenuItem; using Super=::jxx::lang::ClassBase<CheckboxMenuItem,JxxSuper>;
 CheckboxMenuItem(); explicit CheckboxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& label); CheckboxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& label,::jxx::lang::jbool state);
 ::jxx::lang::jbool getState() const; void setState(::jxx::lang::jbool state);
private: ::jxx::lang::jbool state_;
}; }
