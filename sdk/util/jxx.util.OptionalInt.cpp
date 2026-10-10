#include "util/jxx.util.OptionalInt.h"
#include <string>
#include <cstdint>
#include <cstring>
namespace jxx::util { namespace {  }
::jxx::Ptr<OptionalInt> OptionalInt::empty(){return ::jxx::NEW<OptionalInt>();} ::jxx::Ptr<OptionalInt> OptionalInt::of(::jxx::lang::jint v){return ::jxx::NEW<OptionalInt>(v);}
OptionalInt::OptionalInt()=default; OptionalInt::OptionalInt(::jxx::lang::jint v):present_(true),value_(v){}
::jxx::lang::jint OptionalInt::getAsInt()const{if(!present_)throw ::jxx::util::NoSuchElementException(::jxx::NEW<::jxx::lang::String>("No value present"));return value_;}
::jxx::lang::jbool OptionalInt::isPresent()const noexcept{return present_;}
void OptionalInt::ifPresent(const ::jxx::Ptr<::jxx::util::function::IntConsumer>& c)const{if(!c)throw ::jxx::lang::NullPointerException();if(present_)c->accept(value_);}
::jxx::lang::jint OptionalInt::orElse(::jxx::lang::jint o)const noexcept{return present_?value_:o;}
::jxx::lang::jint OptionalInt::orElseGet(const ::jxx::Ptr<::jxx::util::function::IntSupplier>& s)const{if(present_)return value_;if(!s)throw ::jxx::lang::NullPointerException();return s->getAsInt();}
::jxx::lang::jbool OptionalInt::equals(const ::jxx::Ptr<::jxx::lang::Object>& o)const{if(o.get()==this)return true;auto x=::jxx::CAST<OptionalInt>(o);return x&&present_==x->present_&&(!present_||value_==x->value_);}
::jxx::lang::jint OptionalInt::hashCode()const{return present_?static_cast<::jxx::lang::jint>(value_):0;}
::jxx::Ptr<::jxx::lang::String> OptionalInt::toString()const{return ::jxx::NEW<::jxx::lang::String>(present_?std::string("OptionalInt[")+std::to_string(value_)+"]":"OptionalInt.empty");}
} 
