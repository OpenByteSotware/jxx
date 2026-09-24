#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
namespace com::google::gson {
enum class LongSerializationPolicy { DEFAULT, STRING };
class LongSerializationPolicies final : public ::jxx::lang::ClassBase<LongSerializationPolicies, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<LongSerializationPolicies, JxxSuper>;
    static ::jxx::Ptr<JsonElement> serialize(
        LongSerializationPolicy policy,
        ::jxx::lang::jlong value);
};
} // namespace com::google::gson
