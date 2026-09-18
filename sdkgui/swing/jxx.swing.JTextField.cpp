#include "swing/jxx.swing.JTextField.h"
#include <algorithm>
#include "awt/event/jxx.awt.event.ActionEvent.h"
#include "awt/event/jxx.awt.event.ActionListener.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::swing
{
    namespace { ::jxx::Ptr<::jxx::lang::String> emptyText(){return ::jxx::NEW<::jxx::lang::String>("");} }
    JTextField::JTextField():JTextField(emptyText(),0){} JTextField::JTextField(const ::jxx::Ptr<::jxx::lang::String>& text):JTextField(text,0){} JTextField::JTextField(::jxx::lang::jint columns):JTextField(emptyText(),columns){}
    JTextField::JTextField(const ::jxx::Ptr<::jxx::lang::String>& text,::jxx::lang::jint columns):Super(text){setColumns(columns);}
    ::jxx::lang::jint JTextField::getColumns() const{return columns_;} void JTextField::setColumns(::jxx::lang::jint value){if(value<0)throw ::jxx::lang::IllegalArgumentException("columns");columns_=value;invalidate();}
    ::jxx::lang::jint JTextField::getHorizontalAlignment() const{return alignment_;} void JTextField::setHorizontalAlignment(::jxx::lang::jint value){if(value!=LEFT&&value!=CENTER&&value!=RIGHT&&value!=LEADING&&value!=TRAILING)throw ::jxx::lang::IllegalArgumentException("alignment");alignment_=value;invalidate();}
    void JTextField::setActionCommand(const ::jxx::Ptr<::jxx::lang::String>& value){actionCommand_=value;} ::jxx::Ptr<::jxx::lang::String> JTextField::getActionCommand() const{return actionCommand_?actionCommand_:getText();}
    void JTextField::addActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value){if(value&&std::find(listeners_.begin(),listeners_.end(),value)==listeners_.end())listeners_.push_back(value);} void JTextField::removeActionListener(const ::jxx::Ptr<::jxx::awt::event::ActionListener>& value){listeners_.erase(std::remove(listeners_.begin(),listeners_.end(),value),listeners_.end());}
    void JTextField::fireActionPerformed(){processActionEvent(::jxx::NEW<::jxx::awt::event::ActionEvent>(::jxx::CAST<::jxx::lang::Object>(thisPtr()),::jxx::awt::event::ActionEvent::ACTION_PERFORMED,getActionCommand()));}
    void JTextField::processActionEvent(const ::jxx::Ptr<::jxx::awt::event::ActionEvent>& event){const auto copy=listeners_;for(const auto& listener:copy)if(listener)listener->actionPerformed(event);}
}
