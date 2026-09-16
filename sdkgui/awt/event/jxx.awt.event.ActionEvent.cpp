#include "awt/event/jxx.awt.event.ActionEvent.h"
#include <sstream>
namespace jxx::awt::event {
ActionEvent::ActionEvent(const ::jxx::Ptr<::jxx::lang::Object>& s,::jxx::lang::jint i,const ::jxx::Ptr<::jxx::lang::String>& c):ActionEvent(s,i,c,0,0){}
ActionEvent::ActionEvent(const ::jxx::Ptr<::jxx::lang::Object>& s,::jxx::lang::jint i,const ::jxx::Ptr<::jxx::lang::String>& c,::jxx::lang::jint m):ActionEvent(s,i,c,0,m){}
ActionEvent::ActionEvent(const ::jxx::Ptr<::jxx::lang::Object>& s,::jxx::lang::jint i,const ::jxx::Ptr<::jxx::lang::String>& c,::jxx::lang::jlong w,::jxx::lang::jint m):Super(s,i),command_(c),when_(w),modifiers_(m){}
::jxx::Ptr<::jxx::lang::String> ActionEvent::getActionCommand() const{return command_;} ::jxx::lang::jlong ActionEvent::getWhen() const{return when_;} ::jxx::lang::jint ActionEvent::getModifiers() const{return modifiers_;}
::jxx::Ptr<::jxx::lang::String> ActionEvent::paramString() const{std::ostringstream out;out<<"ACTION_PERFORMED,cmd="<<(command_?command_->utf8():"")<<",when="<<when_<<",modifiers="<<modifiers_;return ::jxx::NEW<::jxx::lang::String>(out.str());}
}
