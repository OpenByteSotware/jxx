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
::jxx::lang::jint JsonElement::getAsInt(){ throw ::jxx::lang::IllegalStateException(); }
::jxx::lang::jlong JsonElement::getAsLong(){ throw ::jxx::lang::IllegalStateException(); }
::jxx::lang::jdouble JsonElement::getAsDouble(){ throw ::jxx::lang::IllegalStateException(); }
} // namespace com::google::gson
