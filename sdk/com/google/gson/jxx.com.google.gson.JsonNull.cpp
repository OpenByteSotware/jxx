#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
::jxx::Ptr<JsonNull> JsonNull::INSTANCE(){ static const auto value=::jxx::NEW<JsonNull>(); return value; }
::jxx::Ptr<::jxx::lang::String> JsonNull::toString() const { return ::jxx::NEW<::jxx::lang::String>("null"); }
} // namespace com::google::gson
