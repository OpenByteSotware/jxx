#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
namespace com::google::gson {
JsonObject::JsonObject():members_(::jxx::NEW<::com::google::gson::internal::LinkedTreeMap<::jxx::lang::String,JsonElement>>()){}
void JsonObject::add(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<JsonElement>& value){if(!property)throw ::jxx::lang::NullPointerException();members_->put(property,value==nullptr?JsonNull::INSTANCE():value);}
void JsonObject::addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<::jxx::lang::String>& value){::jxx::Ptr<JsonElement> element=value==nullptr?::jxx::CAST<JsonElement>(JsonNull::INSTANCE()) : ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(value));add(property,element);}
::jxx::Ptr<JsonElement> JsonObject::get(const ::jxx::Ptr<::jxx::lang::String>& property) const{return members_->get(::jxx::CAST<::jxx::lang::Object>(property));}
::jxx::lang::jbool JsonObject::has(const ::jxx::Ptr<::jxx::lang::String>& property) const{return members_->containsKey(::jxx::CAST<::jxx::lang::Object>(property));}
::jxx::Ptr<::jxx::lang::String> JsonObject::toString() const {
    std::string output = "{";
    auto iterator = members_->entrySet()->iterator();
    ::jxx::lang::jbool first = true;
    while (iterator->hasNext()) {
        const auto entry = iterator->next();
        if (!first) output += ',';
        first = false;
        const auto key = entry->getKey();
        output += ::jxx::NEW<JsonPrimitive>(key)->toString()->utf8();
        output += ':';
        const auto value = entry->getValue();
        output += (value == nullptr ? JsonNull::INSTANCE() : value)->toString()->utf8();
    }
    output += '}';
    return ::jxx::NEW<::jxx::lang::String>(output);
}
} // namespace com::google::gson
