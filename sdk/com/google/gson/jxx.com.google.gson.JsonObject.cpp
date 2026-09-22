#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson {
JsonObject::JsonObject():members_(::jxx::NEW<::com::google::gson::internal::LinkedTreeMap<::jxx::lang::String,JsonElement>>()){}
void JsonObject::add(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<JsonElement>& value){if(!property)throw ::jxx::lang::NullPointerException();members_->put(property,value==nullptr?JsonNull::INSTANCE():value);}
void JsonObject::addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<::jxx::lang::String>& value){::jxx::Ptr<JsonElement> element=value==nullptr?::jxx::CAST<JsonElement>(JsonNull::INSTANCE()) : ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(value));add(property,element);}
::jxx::Ptr<JsonElement> JsonObject::get(const ::jxx::Ptr<::jxx::lang::String>& property) const{return members_->get(::jxx::CAST<::jxx::lang::Object>(property));}
::jxx::lang::jbool JsonObject::has(const ::jxx::Ptr<::jxx::lang::String>& property) const{return members_->containsKey(::jxx::CAST<::jxx::lang::Object>(property));}
::jxx::Ptr<::jxx::lang::String> JsonObject::toString() const{return ::jxx::NEW<::jxx::lang::String>("{}");}
} // namespace com::google::gson
