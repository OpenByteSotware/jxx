#include "util/jxx.util.OptionalDouble.h"
#include <string>
#include <cstdint>
#include <cstring>
namespace jxx::util { namespace { static ::jxx::lang::jint hashDouble(double v){std::uint64_t b;std::memcpy(&b,&v,sizeof b);return static_cast<::jxx::lang::jint>(b^(b>>32));}
 }
::jxx::Ptr<OptionalDouble> OptionalDouble::empty(){return ::jxx::NEW<OptionalDouble>();} ::jxx::Ptr<OptionalDouble> OptionalDouble::of(::jxx::lang::jdouble v){return ::jxx::NEW<OptionalDouble>(v);}
OptionalDouble::OptionalDouble()=default; OptionalDouble::OptionalDouble(::jxx::lang::jdouble v):present_(true),value_(v){}
::jxx::lang::jdouble OptionalDouble::getAsDouble()const{if(!present_)throw ::jxx::util::NoSuchElementException(::jxx::NEW<::jxx::lang::String>("No value present"));return value_;}
::jxx::lang::jbool OptionalDouble::isPresent()const noexcept{return present_;}
void OptionalDouble::ifPresent(const ::jxx::Ptr<::jxx::util::function::DoubleConsumer>& c)const{if(!c)throw ::jxx::lang::NullPointerException();if(present_)c->accept(value_);}
::jxx::lang::jdouble OptionalDouble::orElse(::jxx::lang::jdouble o)const noexcept{return present_?value_:o;}
::jxx::lang::jdouble OptionalDouble::orElseGet(const ::jxx::Ptr<::jxx::util::function::DoubleSupplier>& s)const{if(present_)return value_;if(!s)throw ::jxx::lang::NullPointerException();return s->getAsDouble();}
::jxx::lang::jbool OptionalDouble::equals(const ::jxx::Ptr<::jxx::lang::Object>& o)const{if(o.get()==this)return true;auto x=::jxx::CAST<OptionalDouble>(o);return x&&present_==x->present_&&(!present_||value_==x->value_);}
::jxx::lang::jint OptionalDouble::hashCode()const{return present_?hashDouble(value_):0;}
::jxx::Ptr<::jxx::lang::String> OptionalDouble::toString()const{return ::jxx::NEW<::jxx::lang::String>(present_?std::string("OptionalDouble[")+std::to_string(value_)+"]":"OptionalDouble.empty");}
} 
