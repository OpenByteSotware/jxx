#include "awt/jxx.awt.CheckboxMenuItem.h"
namespace jxx::awt {
CheckboxMenuItem::CheckboxMenuItem():CheckboxMenuItem(::jxx::NEW<::jxx::lang::String>(""),false){} CheckboxMenuItem::CheckboxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& label):CheckboxMenuItem(label,false){} CheckboxMenuItem::CheckboxMenuItem(const ::jxx::Ptr<::jxx::lang::String>& label,::jxx::lang::jbool state):Super(label),state_(state){}
::jxx::lang::jbool CheckboxMenuItem::getState() const{return state_;} void CheckboxMenuItem::setState(::jxx::lang::jbool value){state_=value;}
}
