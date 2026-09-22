#pragma once
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
namespace com::google::gson::internal {
class NullSafeTypeAdapter final
    : public ::jxx::lang::ClassBase<
          NullSafeTypeAdapter,
          ::jxx::lang::Object,
          ::com::google::gson::TypeAdapter> {
public:
    explicit NullSafeTypeAdapter(
        const ::jxx::Ptr<::com::google::gson::TypeAdapter>& delegate);
    ::jxx::Ptr<::jxx::lang::Object> fromJsonTree(
        const ::jxx::Ptr<::com::google::gson::JsonElement>& element) override;
    ::jxx::Ptr<::com::google::gson::JsonElement> toJsonTree(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
private:
    ::jxx::Ptr<::com::google::gson::TypeAdapter> delegate_;
};
} // namespace com::google::gson::internal
