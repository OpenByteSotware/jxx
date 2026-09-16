#include "awt/jxx.awt.MenuComponent.h"
namespace jxx::awt {
MenuComponent::MenuComponent()=default; MenuComponent::~MenuComponent()=default;
::jxx::Ptr<Font> MenuComponent::getFont() const{return font_;} void MenuComponent::setFont(const ::jxx::Ptr<Font>& value){font_=value;}
::jxx::Ptr<::jxx::lang::String> MenuComponent::getName() const{return name_;} void MenuComponent::setName(const ::jxx::Ptr<::jxx::lang::String>& value){name_=value;}
}
