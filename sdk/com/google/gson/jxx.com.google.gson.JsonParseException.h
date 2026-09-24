#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.RuntimeException.h"
namespace com::google::gson {
class JsonParseException : public ::jxx::lang::ClassBase<JsonParseException, ::jxx::lang::RuntimeException> {
public:
    using JxxSuper = ::jxx::lang::RuntimeException;
    using Super = ::jxx::lang::ClassBase<JsonParseException, JxxSuper>;

    JsonParseException();
    explicit JsonParseException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit JsonParseException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    JsonParseException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};
} // namespace com::google::gson
