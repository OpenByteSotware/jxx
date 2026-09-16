#include "awt/jxx.awt.MenuItem.h"
#include <algorithm>
#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
namespace jxx::awt {
namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){return ::jxx::NEW<::jxx::lang::String>("");} }
MenuItem::MenuItem():MenuItem(emptyText(),nullptr){} MenuItem::MenuItem(const ::jxx::Ptr<::jxx::lang::String>& label):MenuItem(label,nullptr){}
MenuItem::MenuItem(const ::jxx::Ptr<::jxx::lang::String>& label,const ::jxx::Ptr<MenuShortcut>& shortcut):label_(label ? label : emptyText()),shortcut_(shortcut),enabled_(true){}
::jxx::Ptr<::jxx::lang::String> MenuItem::getLabel() const{return label_;} void MenuItem::setLabel(const ::jxx::Ptr<::jxx::lang::String>& value){label_=value ? value : emptyText();}
::jxx::lang::jbool MenuItem::isEnabled() const{return enabled_;} void MenuItem::setEnabled(::jxx::lang::jbool value){enabled_=value;}
::jxx::Ptr<MenuShortcut> MenuItem::getShortcut() const{return shortcut_;} void MenuItem::setShortcut(const ::jxx::Ptr<MenuShortcut>& value){shortcut_=value;} void MenuItem::deleteShortcut(){shortcut_.reset();}
void MenuItem::setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& value){actionCommand_=value;} ::jxx::Ptr<::jxx::lang::String> MenuItem::getActionCommand() const{return actionCommand_ ? actionCommand_ : label_;}
void MenuItem::addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value){if(value&&std::find(listeners_.begin(),listeners_.end(),value)==listeners_.end())listeners_.push_back(value);}
void MenuItem::removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value){listeners_.erase(std::remove(listeners_.begin(),listeners_.end(),value),listeners_.end());}
void MenuItem::fireActionPerformed(){if(enabled_)processActionEvent(::jxx::NEW<::jxx::awt::event::ActionEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr()),::jxx::awt::event::ActionEvent::ACTION_PERFORMED,getActionCommand()));}
void MenuItem::processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event){auto copy=listeners_;for(const auto& listener:copy)if(listener)listener->actionPerformed(event);}
}
