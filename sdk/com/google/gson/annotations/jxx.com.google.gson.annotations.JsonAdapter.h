#pragma once
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace com::google::gson::annotations {
class JsonAdapter final : public ::jxx::lang::ClassBase<JsonAdapter, ::jxx::lang::Object> {
public:
    JsonAdapter(const ::jxx::Ptr<::jxx::lang::ClassAny>& value,
                ::jxx::lang::jbool nullSafe = true)
        : value_(value), nullSafe_(nullSafe) {}
    ::jxx::Ptr<::jxx::lang::ClassAny> value() const { return value_; }
    ::jxx::lang::jbool nullSafe() const noexcept { return nullSafe_; }
private:
    ::jxx::Ptr<::jxx::lang::ClassAny> value_;
    ::jxx::lang::jbool nullSafe_;
};
} // namespace com::google::gson::annotations
