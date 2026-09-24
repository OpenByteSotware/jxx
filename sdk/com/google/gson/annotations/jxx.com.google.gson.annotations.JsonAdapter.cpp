#include "com/google/gson/annotations/jxx.com.google.gson.annotations.JsonAdapter.h"

namespace com::google::gson::annotations {
JsonAdapter::JsonAdapter(const ::jxx::Ptr<::jxx::lang::ClassAny>& value,
    ::jxx::lang::jbool nullSafe) : value_(value), nullSafe_(nullSafe) {}
::jxx::Ptr<::jxx::lang::ClassAny> JsonAdapter::value() const { return value_; }
::jxx::lang::jbool JsonAdapter::nullSafe() const noexcept { return nullSafe_; }
} // namespace com::google::gson::annotations
