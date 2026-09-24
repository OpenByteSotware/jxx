#include "com/google/gson/stream/jxx.com.google.gson.stream.MalformedJsonException.h"
namespace com::google::gson::stream {
MalformedJsonException::MalformedJsonException() = default;
MalformedJsonException::MalformedJsonException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {}
} // namespace com::google::gson::stream
