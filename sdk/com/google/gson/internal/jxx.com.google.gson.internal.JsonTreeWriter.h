#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
namespace com::google::gson::internal {
class JsonTreeWriter final : public ::jxx::lang::ClassBase<JsonTreeWriter, ::jxx::lang::Object> {
public:
 JsonTreeWriter();
 void set(const ::jxx::Ptr<::com::google::gson::JsonElement>& element);
 ::jxx::Ptr<::com::google::gson::JsonElement> get() const;
private: ::jxx::Ptr<::com::google::gson::JsonElement> element_;
};
}
