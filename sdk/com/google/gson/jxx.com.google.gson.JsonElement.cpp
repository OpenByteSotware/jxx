#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.IllegalStateException.h"
namespace com::google::gson {
::jxx::lang::jbool JsonElement::isJsonNull() const noexcept { return false; }
::jxx::lang::jbool JsonElement::isJsonPrimitive() const noexcept { return false; }
::jxx::lang::jbool JsonElement::isJsonArray() const noexcept { return false; }
::jxx::lang::jbool JsonElement::isJsonObject() const noexcept { return false; }
::jxx::Ptr<JsonPrimitive> JsonElement::getAsJsonPrimitive(){ if(!isJsonPrimitive()) throw ::jxx::lang::IllegalStateException(); return std::dynamic_pointer_cast<JsonPrimitive>(shared_from_this()); }
::jxx::Ptr<JsonArray> JsonElement::getAsJsonArray(){ if(!isJsonArray()) throw ::jxx::lang::IllegalStateException(); return std::dynamic_pointer_cast<JsonArray>(shared_from_this()); }
::jxx::Ptr<JsonObject> JsonElement::getAsJsonObject(){ if(!isJsonObject()) throw ::jxx::lang::IllegalStateException(); return std::dynamic_pointer_cast<JsonObject>(shared_from_this()); }
::jxx::Ptr<::jxx::lang::String> JsonElement::getAsString(){ throw ::jxx::lang::IllegalStateException(); }
::jxx::lang::jbool JsonElement::getAsBoolean(){ throw ::jxx::lang::IllegalStateException(); }
::jxx::lang::jchar JsonElement::getAsCharacter(){const auto text=getAsString();if(text==nullptr||text->length()==0)throw ::jxx::lang::IllegalStateException();return text->charAt(0);}
::jxx::lang::jbyte JsonElement::getAsByte(){ return static_cast<::jxx::lang::jbyte>(getAsInt()); }
::jxx::lang::jshort JsonElement::getAsShort(){ return static_cast<::jxx::lang::jshort>(getAsInt()); }
::jxx::lang::jint JsonElement::getAsInt(){ throw ::jxx::lang::IllegalStateException(); }
::jxx::lang::jfloat JsonElement::getAsFloat(){ return static_cast<::jxx::lang::jfloat>(getAsDouble()); }
::jxx::lang::jlong JsonElement::getAsLong(){ throw ::jxx::lang::IllegalStateException(); }
::jxx::lang::jdouble JsonElement::getAsDouble(){ throw ::jxx::lang::IllegalStateException(); }
::jxx::Ptr<JsonElement> JsonElement::deepCopy(){return ::jxx::CAST<JsonElement>(this->thisPtr());}
} // namespace com::google::gson
