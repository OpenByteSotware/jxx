#include "com/google/gson/jxx.com.google.gson.JsonIOException.h"
namespace com::google::gson {
JsonIOException::JsonIOException() = default;
JsonIOException::JsonIOException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {}
JsonIOException::JsonIOException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause)
    : Super(cause) {}
JsonIOException::JsonIOException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause)
    : Super(message, cause) {}
::jxx::Ptr<::jxx::lang::Object> JsonIOException::cloneImpl() const {
    return ::jxx::NEW<JsonIOException>(*this);
}
const char* JsonIOException::typeName() const noexcept {
    return "com.google.gson.JsonIOException";
}
} // namespace com::google::gson
