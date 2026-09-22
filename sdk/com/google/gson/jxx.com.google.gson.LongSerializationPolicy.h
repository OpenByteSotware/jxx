#pragma once
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
namespace com::google::gson {
enum class LongSerializationPolicy { DEFAULT, STRING };
class LongSerializationPolicies final {
public:
    static ::jxx::Ptr<JsonElement> serialize(
        LongSerializationPolicy policy,
        ::jxx::lang::jlong value);
};
} // namespace com::google::gson
