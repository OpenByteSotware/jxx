#include "awt/jxx.awt.AWTEvent.h"
#include <sstream>
namespace jxx::awt {
AWTEvent::AWTEvent(const ::jxx::Ptr<::jxx::lang::Object>& source,::jxx::lang::jint value):Super(source),id(value),consumed(false){}
::jxx::lang::jint AWTEvent::getID() const{return id;} ::jxx::lang::jbool AWTEvent::isConsumed() const{return consumed;} void AWTEvent::consume(){consumed=true;}
::jxx::Ptr<::jxx::lang::String> AWTEvent::paramString() const{return ::jxx::NEW<::jxx::lang::String>("");}
::jxx::Ptr<::jxx::lang::String> AWTEvent::toString() const{std::ostringstream out;out<<"jxx.awt.AWTEvent["<<paramString()->utf8()<<"] on "<<source->toString()->utf8();return ::jxx::NEW<::jxx::lang::String>(out.str());}
}
