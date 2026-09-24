#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "com/google/gson/jxx.com.google.gson.JsonParseException.h"
namespace com::google::gson {
class JsonIOException : public ::jxx::lang::ClassBase<JsonIOException, JsonParseException> {
public:
    using JxxSuper = JsonParseException;
    using Super = ::jxx::lang::ClassBase<JsonIOException, JxxSuper>;

    JsonIOException();
    explicit JsonIOException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit JsonIOException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    JsonIOException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};
} // namespace com::google::gson
