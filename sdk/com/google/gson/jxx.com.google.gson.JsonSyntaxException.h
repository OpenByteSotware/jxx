#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"

namespace com::google::gson {

class JsonSyntaxException
    : public ::jxx::lang::RuntimeException {
public:
    using JxxSuper = ::jxx::lang::RuntimeException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<JsonSyntaxException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    JsonSyntaxException();
    explicit JsonSyntaxException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    JsonSyntaxException(const JsonSyntaxException&) = default;
    JsonSyntaxException(JsonSyntaxException&&) noexcept = default;
    JsonSyntaxException& operator=(const JsonSyntaxException&) = default;
    JsonSyntaxException& operator=(JsonSyntaxException&&) noexcept = default;
    ~JsonSyntaxException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace com::google::gson
