#include "com/google/gson/jxx.com.google.gson.LongSerializationPolicy.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
::jxx::Ptr<JsonElement> LongSerializationPolicies::serialize(
    LongSerializationPolicy policy,
    ::jxx::lang::jlong value) {
    return policy == LongSerializationPolicy::STRING
        ? ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(
              ::jxx::NEW<::jxx::lang::String>(std::to_string(value))))
        : ::jxx::CAST<JsonElement>(::jxx::NEW<JsonPrimitive>(value));
}
} // namespace com::google::gson
