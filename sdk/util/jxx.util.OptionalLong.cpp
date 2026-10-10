#include "util/jxx.util.OptionalLong.h"
#include <string>
#include <cstdint>
#include <cstring>
namespace jxx::util { namespace {  }
::jxx::Ptr<OptionalLong> OptionalLong::empty(){return ::jxx::NEW<OptionalLong>();} ::jxx::Ptr<OptionalLong> OptionalLong::of(::jxx::lang::jlong v){return ::jxx::NEW<OptionalLong>(v);}
OptionalLong::OptionalLong()=default; OptionalLong::OptionalLong(::jxx::lang::jlong v):present_(true),value_(v){}
::jxx::lang::jlong OptionalLong::getAsLong()const{if(!present_)throw ::jxx::util::NoSuchElementException(::jxx::NEW<::jxx::lang::String>("No value present"));return value_;}
::jxx::lang::jbool OptionalLong::isPresent()const noexcept{return present_;}
void OptionalLong::ifPresent(const ::jxx::Ptr<::jxx::util::function::LongConsumer>& c)const{if(!c)throw ::jxx::lang::NullPointerException();if(present_)c->accept(value_);}
::jxx::lang::jlong OptionalLong::orElse(::jxx::lang::jlong o)const noexcept{return present_?value_:o;}
::jxx::lang::jlong OptionalLong::orElseGet(const ::jxx::Ptr<::jxx::util::function::LongSupplier>& s)const{if(present_)return value_;if(!s)throw ::jxx::lang::NullPointerException();return s->getAsLong();}
::jxx::lang::jbool OptionalLong::equals(const ::jxx::Ptr<::jxx::lang::Object>& o)const{if(o.get()==this)return true;auto x=::jxx::CAST<OptionalLong>(o);return x&&present_==x->present_&&(!present_||value_==x->value_);}
::jxx::lang::jint OptionalLong::hashCode()const{return present_?static_cast<::jxx::lang::jint>(static_cast<std::uint64_t>(value_)^(static_cast<std::uint64_t>(value_)>>32)):0;}
::jxx::Ptr<::jxx::lang::String> OptionalLong::toString()const{return ::jxx::NEW<::jxx::lang::String>(present_?std::string("OptionalLong[")+std::to_string(value_)+"]":"OptionalLong.empty");}
} 
