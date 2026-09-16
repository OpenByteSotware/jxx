#include "util/jxx.util.EventObject.h"
#include <sstream>
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::util {
EventObject::EventObject(const ::jxx::Ptr<::jxx::lang::Object>& value):source(value){if(!source)throw ::jxx::lang::IllegalArgumentException("null source");}
::jxx::Ptr<::jxx::lang::Object> EventObject::getSource() const{return source;}
::jxx::Ptr<::jxx::lang::String> EventObject::toString() const{std::ostringstream out;out<<"jxx.util.EventObject[source="<<source->toString()->utf8()<<"]";return ::jxx::NEW<::jxx::lang::String>(out.str());}
}
