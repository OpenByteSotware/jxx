#include "awt/event/jxx.awt.event.FocusEvent.h"
#include <sstream>
namespace jxx::awt::event {
FocusEvent::FocusEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,::jxx::lang::jint id):FocusEvent(source,id,false,nullptr){}
FocusEvent::FocusEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,::jxx::lang::jint id,::jxx::lang::jbool temporary):FocusEvent(source,id,temporary,nullptr){}
FocusEvent::FocusEvent(const ::jxx::Ptr<::jxx::awt::Component>& source,::jxx::lang::jint id,::jxx::lang::jbool temporary,const ::jxx::Ptr<::jxx::awt::Component>& opposite):Super(source,id),temporary_(temporary),opposite_(opposite){}
::jxx::lang::jbool FocusEvent::isTemporary() const{return temporary_;}
::jxx::Ptr<::jxx::awt::Component> FocusEvent::getOppositeComponent() const{return opposite_.lock();}
::jxx::Ptr<::jxx::lang::String> FocusEvent::paramString() const{std::ostringstream out;out<<(id==FOCUS_GAINED?"FOCUS_GAINED":id==FOCUS_LOST?"FOCUS_LOST":"unknown type")<<",temporary="<<(temporary_?"true":"false");return ::jxx::NEW<::jxx::lang::String>(out.str());}
}
