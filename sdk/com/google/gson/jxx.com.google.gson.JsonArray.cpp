#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
JsonArray::JsonArray():elements_(::jxx::NEW<::jxx::util::ArrayList<JsonElement>>()){}
void JsonArray::add(const ::jxx::Ptr<JsonElement>& element){elements_->add(element==nullptr?JsonNull::INSTANCE():element);}
void JsonArray::add(const ::jxx::Ptr<::jxx::lang::String>& value){add(value==nullptr?::jxx::CAST<JsonElement>(JsonNull::INSTANCE()) : ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(value)));}
void JsonArray::add(::jxx::lang::jbool value){add(::jxx::NEW<JsonPrimitive>(value));}
void JsonArray::add(::jxx::lang::jlong value){add(::jxx::NEW<JsonPrimitive>(value));}
void JsonArray::add(::jxx::lang::jdouble value){add(::jxx::NEW<JsonPrimitive>(value));}
void JsonArray::addAll(const ::jxx::Ptr<JsonArray>& array){if(array==nullptr)throw ::jxx::lang::NullPointerException();for(::jxx::lang::jint i=0;i<array->size();++i)add(array->get(i));}
::jxx::Ptr<JsonElement> JsonArray::get(::jxx::lang::jint index) const{return elements_->get(index);}
void JsonArray::set(::jxx::lang::jint index,const ::jxx::Ptr<JsonElement>& element){elements_->set(index,element==nullptr?JsonNull::INSTANCE():element);}
::jxx::Ptr<JsonElement> JsonArray::remove(::jxx::lang::jint index){return elements_->remove(index);}
::jxx::lang::jbool JsonArray::remove(const ::jxx::Ptr<JsonElement>& element){return elements_->remove(element);}
::jxx::lang::jbool JsonArray::contains(const ::jxx::Ptr<JsonElement>& element) const{return elements_->contains(element);}
::jxx::lang::jint JsonArray::size() const{return elements_->size();}
::jxx::lang::jbool JsonArray::isEmpty() const{return elements_->isEmpty();}
::jxx::Ptr<::jxx::util::Iterator<JsonElement>> JsonArray::iterator() const{return elements_->iterator();}
::jxx::Ptr<JsonElement> JsonArray::deepCopy(){const auto result=::jxx::NEW<JsonArray>();for(::jxx::lang::jint i=0;i<size();++i){const auto value=get(i);result->add(value==nullptr?nullptr:value->deepCopy());}return result;}
::jxx::Ptr<::jxx::lang::String> JsonArray::getAsString(){if(size()!=1)throw ::jxx::lang::IllegalStateException();return get(0)->getAsString();}
::jxx::lang::jbool JsonArray::getAsBoolean(){if(size()!=1)throw ::jxx::lang::IllegalStateException();return get(0)->getAsBoolean();}
::jxx::lang::jbyte JsonArray::getAsByte(){if(size()!=1)throw ::jxx::lang::IllegalStateException();return get(0)->getAsByte();}
::jxx::lang::jshort JsonArray::getAsShort(){if(size()!=1)throw ::jxx::lang::IllegalStateException();return get(0)->getAsShort();}
::jxx::lang::jint JsonArray::getAsInt(){if(size()!=1)throw ::jxx::lang::IllegalStateException();return get(0)->getAsInt();}
::jxx::lang::jfloat JsonArray::getAsFloat(){if(size()!=1)throw ::jxx::lang::IllegalStateException();return get(0)->getAsFloat();}
::jxx::lang::jlong JsonArray::getAsLong(){if(size()!=1)throw ::jxx::lang::IllegalStateException();return get(0)->getAsLong();}
::jxx::lang::jdouble JsonArray::getAsDouble(){if(size()!=1)throw ::jxx::lang::IllegalStateException();return get(0)->getAsDouble();}
::jxx::lang::jbool JsonArray::equals(const ::jxx::Ptr<::jxx::lang::Object>& other) const{const auto array=std::dynamic_pointer_cast<JsonArray>(other);if(array==nullptr||array->size()!=size())return false;for(::jxx::lang::jint i=0;i<size();++i){const auto a=get(i);const auto b=array->get(i);if(a==nullptr?b!=nullptr:!a->equals(b))return false;}return true;}
::jxx::lang::jint JsonArray::hashCode() const{::jxx::lang::jint result=1;for(::jxx::lang::jint i=0;i<size();++i){const auto value=get(i);result=31*result+(value==nullptr?0:value->hashCode());}return result;}
::jxx::Ptr<::jxx::lang::String> JsonArray::toString() const{std::string out="[";for(::jxx::lang::jint i=0;i<size();++i){if(i)out+=',';out+=get(i)->toString()->utf8();}return ::jxx::NEW<::jxx::lang::String>(out+"]");}
} // namespace com::google::gson
