#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
namespace com::google::gson {
JsonObject::JsonObject():members_(::jxx::NEW<::com::google::gson::internal::LinkedTreeMap<::jxx::lang::String,JsonElement>>()){}
JsonObject::JsonObject(const ::jxx::Ptr<JsonObject>& source):JsonObject(){if(source==nullptr)throw ::jxx::lang::NullPointerException();const auto iterator=source->entrySet()->iterator();while(iterator->hasNext()){const auto entry=iterator->next();add(entry->getKey(),entry->getValue());}}
void JsonObject::add(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<JsonElement>& value){if(!property)throw ::jxx::lang::NullPointerException();members_->put(property,value==nullptr?JsonNull::INSTANCE():value);}
void JsonObject::addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,const ::jxx::Ptr<::jxx::lang::String>& value){::jxx::Ptr<JsonElement> element=value==nullptr?::jxx::CAST<JsonElement>(JsonNull::INSTANCE()) : ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(value));add(property,element);}
void JsonObject::addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,::jxx::lang::jbool value){add(property,::jxx::NEW<JsonPrimitive>(value));}
void JsonObject::addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,::jxx::lang::jlong value){add(property,::jxx::NEW<JsonPrimitive>(value));}
void JsonObject::addProperty(const ::jxx::Ptr<::jxx::lang::String>& property,::jxx::lang::jdouble value){add(property,::jxx::NEW<JsonPrimitive>(value));}
::jxx::Ptr<JsonElement> JsonObject::get(const ::jxx::Ptr<::jxx::lang::String>& property) const{return members_->get(::jxx::CAST<::jxx::lang::Object>(property));}
::jxx::Ptr<JsonElement> JsonObject::remove(const ::jxx::Ptr<::jxx::lang::String>& property){return members_->remove(::jxx::CAST<::jxx::lang::Object>(property));}
::jxx::Ptr<JsonArray> JsonObject::getAsJsonArray(const ::jxx::Ptr<::jxx::lang::String>& property) const{const auto value=get(property);return value==nullptr?nullptr:value->getAsJsonArray();}
::jxx::Ptr<JsonObject> JsonObject::getAsJsonObject(const ::jxx::Ptr<::jxx::lang::String>& property) const{const auto value=get(property);return value==nullptr?nullptr:value->getAsJsonObject();}
::jxx::Ptr<JsonPrimitive> JsonObject::getAsJsonPrimitive(const ::jxx::Ptr<::jxx::lang::String>& property) const{const auto value=get(property);return value==nullptr?nullptr:value->getAsJsonPrimitive();}
::jxx::lang::jint JsonObject::size() const{return members_->size();}
::jxx::lang::jbool JsonObject::isEmpty() const{return members_->isEmpty();}
::jxx::lang::jbool JsonObject::has(const ::jxx::Ptr<::jxx::lang::String>& property) const{return members_->containsKey(::jxx::CAST<::jxx::lang::Object>(property));}
::jxx::Ptr<::jxx::util::Set<
    ::jxx::util::MapEntry<::jxx::lang::String, JsonElement>>>
JsonObject::entrySet() const {
    return members_->entrySet();
}

::jxx::Ptr<::jxx::util::Set<::jxx::lang::String>> JsonObject::keySet() const { return members_->keySet(); }
::jxx::Ptr<::jxx::util::Collection<JsonElement>> JsonObject::values() const { return members_->values(); }

::jxx::Ptr<JsonElement> JsonObject::deepCopy() {
    const auto result = ::jxx::NEW<JsonObject>();
    const auto iterator = members_->entrySet()->iterator();
    while (iterator->hasNext()) { const auto entry = iterator->next(); const auto value = entry->getValue(); result->add(entry->getKey(), value == nullptr ? nullptr : value->deepCopy()); }
    return result;
}

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
