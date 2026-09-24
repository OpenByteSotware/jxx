#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
::jxx::Ptr<JsonNull> JsonNull::INSTANCE(){ static const auto value=::jxx::NEW<JsonNull>(); return value; }
::jxx::Ptr<JsonElement> JsonNull::deepCopy(){ return INSTANCE(); }
::jxx::lang::jbool JsonNull::equals(const ::jxx::Ptr<::jxx::lang::Object>& other) const{return std::dynamic_pointer_cast<JsonNull>(other)!=nullptr;}
::jxx::lang::jint JsonNull::hashCode() const{return 31;}
::jxx::Ptr<::jxx::lang::String> JsonNull::toString() const { return ::jxx::NEW<::jxx::lang::String>("null"); }
::jxx::lang::jbool JsonNull::isJsonNull() const noexcept { return true; }
} // namespace com::google::gson
