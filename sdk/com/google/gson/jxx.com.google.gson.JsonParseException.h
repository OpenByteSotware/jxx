#pragma once
#include "lang/jxx.lang.RuntimeException.h"
namespace com::google::gson {
class JsonParseException : public ::jxx::lang::RuntimeException {
public:
    JsonParseException();
    explicit JsonParseException(const ::jxx::Ptr<::jxx::lang::String>& message);
protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};
} // namespace com::google::gson
