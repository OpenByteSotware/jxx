#include "com/google/gson/jxx.com.google.gson.JsonParseException.h"
namespace com::google::gson {
JsonParseException::JsonParseException() = default;
JsonParseException::JsonParseException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : ::jxx::lang::RuntimeException(message) {}
::jxx::Ptr<::jxx::lang::Object> JsonParseException::cloneImpl() const {
    return ::jxx::NEW<JsonParseException>(*this);
}
const char* JsonParseException::typeName() const noexcept {
    return "com.google.gson.JsonParseException";
}
} // namespace com::google::gson
