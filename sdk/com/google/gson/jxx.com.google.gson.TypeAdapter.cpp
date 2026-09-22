#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.NullSafeTypeAdapter.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace com::google::gson {
::jxx::Ptr<TypeAdapter> TypeAdapter::nullSafe(
    const ::jxx::Ptr<TypeAdapter>& delegate) {
    if (delegate == nullptr) throw ::jxx::lang::NullPointerException();
    return std::make_shared<::com::google::gson::internal::NullSafeTypeAdapter>(delegate);
}
} // namespace com::google::gson
