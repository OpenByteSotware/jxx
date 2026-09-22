#pragma once
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.JsonDeserializationContext.h"
#include "com/google/gson/jxx.com.google.gson.JsonSerializationContext.h"
namespace com::google::gson::internal {
class GsonContext final
    : public ::jxx::lang::ClassBase<
          GsonContext,
          ::jxx::lang::Object,
          ::com::google::gson::JsonSerializationContext,
          ::com::google::gson::JsonDeserializationContext> {
public:
 explicit GsonContext(const ::jxx::Ptr<::com::google::gson::Gson>& gson);
 ::jxx::Ptr<::com::google::gson::JsonElement> serialize(
     const ::jxx::Ptr<::jxx::lang::Object>& source) override;
 ::jxx::Ptr<::com::google::gson::JsonElement> serialize(
     const ::jxx::Ptr<::jxx::lang::Object>& source,
     const ::jxx::Ptr<::jxx::lang::ClassAny>& type) override;
 ::jxx::Ptr<::jxx::lang::Object> deserialize(
     const ::jxx::Ptr<::com::google::gson::JsonElement>& json,
     const ::jxx::Ptr<::jxx::lang::ClassAny>& type) override;
private:
 ::jxx::Ptr<::com::google::gson::Gson> gson_;
};
} // namespace com::google::gson::internal
