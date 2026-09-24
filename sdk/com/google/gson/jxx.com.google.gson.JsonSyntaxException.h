#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "com/google/gson/jxx.com.google.gson.JsonParseException.h"

namespace com::google::gson {

class JsonSyntaxException : public ::jxx::lang::ClassBase<JsonSyntaxException, JsonParseException> {
public:
    using JxxSuper = JsonParseException;
    using Super = ::jxx::lang::ClassBase<JsonSyntaxException, JxxSuper>;

    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<JsonSyntaxException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    JsonSyntaxException();
    explicit JsonSyntaxException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit JsonSyntaxException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    JsonSyntaxException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
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
