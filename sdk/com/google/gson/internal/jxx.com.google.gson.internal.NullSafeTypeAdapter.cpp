#include "com/google/gson/internal/jxx.com.google.gson.internal.NullSafeTypeAdapter.h"
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson::internal {
NullSafeTypeAdapter::NullSafeTypeAdapter(
    const ::jxx::Ptr<::com::google::gson::TypeAdapter>& delegate)
    : delegate_(delegate) {
    if (delegate_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::Object> NullSafeTypeAdapter::fromJsonTree(
    const ::jxx::Ptr<::com::google::gson::JsonElement>& element) {
    return element == nullptr || element->isJsonNull()
        ? nullptr
        : delegate_->fromJsonTree(element);
}
::jxx::Ptr<::com::google::gson::JsonElement> NullSafeTypeAdapter::toJsonTree(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    return value == nullptr
        ? ::com::google::gson::JsonNull::INSTANCE()
        : delegate_->toJsonTree(value);
}
} // namespace com::google::gson::internal
