#include "awt/jxx.awt.Button.h"
#include <algorithm>
#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "gui/internal/jxx.gui.internal.NativeComponent.h"
namespace jxx::awt {
namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){return ::jxx::NEW<::jxx::lang::String>("");} }
Button::Button():Button(emptyText()){} Button::Button(const ::jxx::Ptr<::jxx::lang::String>& label):label_(label ? label : emptyText()){}
::jxx::Ptr<::jxx::lang::String> Button::getLabel() const{return label_;} void Button::setLabel(const ::jxx::Ptr<::jxx::lang::String>& value){label_=value ? value : emptyText();if(nativeComponent_)nativeComponent_->setText(label_);invalidate();}
void Button::setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& value){actionCommand_=value;} ::jxx::Ptr<::jxx::lang::String> Button::getActionCommand() const{return actionCommand_ ? actionCommand_ : label_;}
void Button::addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value){if(value&&std::find(listeners_.begin(),listeners_.end(),value)==listeners_.end())listeners_.push_back(value);} void Button::removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value){listeners_.erase(std::remove(listeners_.begin(),listeners_.end(),value),listeners_.end());}
void Button::fireActionPerformed(){processActionEvent(::jxx::NEW<::jxx::awt::event::ActionEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr()),::jxx::awt::event::ActionEvent::ACTION_PERFORMED,getActionCommand()));} void Button::processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event){auto copy=listeners_;for(const auto& listener:copy)if(listener)listener->actionPerformed(event);}
}
