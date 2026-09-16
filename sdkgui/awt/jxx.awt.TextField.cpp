#include "awt/jxx.awt.TextField.h"
#include <algorithm>
#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::awt {
namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){return ::jxx::NEW<::jxx::lang::String>("");} }
TextField::TextField():TextField(emptyText(),0){} TextField::TextField(const ::jxx::Ptr<::jxx::lang::String>& text):TextField(text,0){} TextField::TextField(::jxx::lang::jint columns):TextField(emptyText(),columns){}
TextField::TextField(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint columns):Super(text),columns_(0),echoChar_(0){setColumns(columns);}
::jxx::lang::jint TextField::getColumns() const{return columns_;} void TextField::setColumns(::jxx::lang::jint value){if(value<0)throw ::jxx::lang::IllegalArgumentException("columns");columns_=value;invalidate();}
::jxx::lang::jchar TextField::getEchoChar() const{return echoChar_;} void TextField::setEchoChar(::jxx::lang::jchar value){echoChar_=value;} ::jxx::lang::jbool TextField::echoCharIsSet() const{return echoChar_!=0;}
void TextField::setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& value){actionCommand_=value;} ::jxx::Ptr<::jxx::lang::String> TextField::getActionCommand() const{return actionCommand_ ? actionCommand_ : getText();}
void TextField::addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value){if(value&&std::find(listeners_.begin(),listeners_.end(),value)==listeners_.end())listeners_.push_back(value);}
void TextField::removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value){listeners_.erase(std::remove(listeners_.begin(),listeners_.end(),value),listeners_.end());}
void TextField::fireActionPerformed(){processActionEvent(::jxx::NEW<::jxx::awt::event::ActionEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr()),::jxx::awt::event::ActionEvent::ACTION_PERFORMED,getActionCommand()));}
void TextField::processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event){auto copy=listeners_;for(const auto& listener:copy)if(listener)listener->actionPerformed(event);}
}
