#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonParseException.h"
namespace com::google::gson {
class JsonIOException : public JsonParseException {
public:
    JsonIOException();
    explicit JsonIOException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit JsonIOException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    JsonIOException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};
} // namespace com::google::gson
