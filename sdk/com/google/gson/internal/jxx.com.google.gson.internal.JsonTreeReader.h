#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
namespace com::google::gson::internal {
class JsonTreeReader final : public ::jxx::lang::ClassBase<JsonTreeReader, ::jxx::lang::Object> {
public:
 explicit JsonTreeReader(const ::jxx::Ptr<::com::google::gson::JsonElement>& element);
 ::jxx::Ptr<::com::google::gson::JsonElement> get() const;
private: ::jxx::Ptr<::com::google::gson::JsonElement> element_;
};
}
