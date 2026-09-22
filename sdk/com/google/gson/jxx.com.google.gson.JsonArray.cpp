#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
JsonArray::JsonArray():elements_(::jxx::NEW<::jxx::util::ArrayList<JsonElement>>()){}
void JsonArray::add(const ::jxx::Ptr<JsonElement>& element){elements_->add(element==nullptr?JsonNull::INSTANCE():element);}
void JsonArray::add(const ::jxx::Ptr<::jxx::lang::String>& value){add(value==nullptr?::jxx::CAST<JsonElement>(JsonNull::INSTANCE()) : ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(value)));}
void JsonArray::add(::jxx::lang::jbool value){add(::jxx::NEW<JsonPrimitive>(value));}
void JsonArray::add(::jxx::lang::jlong value){add(::jxx::NEW<JsonPrimitive>(value));}
void JsonArray::add(::jxx::lang::jdouble value){add(::jxx::NEW<JsonPrimitive>(value));}
::jxx::Ptr<JsonElement> JsonArray::get(::jxx::lang::jint index) const{return elements_->get(index);}
void JsonArray::set(::jxx::lang::jint index,const ::jxx::Ptr<JsonElement>& element){elements_->set(index,element==nullptr?JsonNull::INSTANCE():element);}
::jxx::Ptr<JsonElement> JsonArray::remove(::jxx::lang::jint index){return elements_->remove(index);}
::jxx::lang::jbool JsonArray::remove(const ::jxx::Ptr<JsonElement>& element){return elements_->remove(element);}
::jxx::lang::jbool JsonArray::contains(const ::jxx::Ptr<JsonElement>& element) const{return elements_->contains(element);}
::jxx::lang::jint JsonArray::size() const{return elements_->size();}
::jxx::Ptr<JsonElement> JsonArray::deepCopy(){const auto result=::jxx::NEW<JsonArray>();for(::jxx::lang::jint i=0;i<size();++i){const auto value=get(i);result->add(value==nullptr?nullptr:value->deepCopy());}return result;}
::jxx::Ptr<::jxx::lang::String> JsonArray::toString() const{std::string out="[";for(::jxx::lang::jint i=0;i<size();++i){if(i)out+=',';out+=get(i)->toString()->utf8();}return ::jxx::NEW<::jxx::lang::String>(out+"]");}
} // namespace com::google::gson
