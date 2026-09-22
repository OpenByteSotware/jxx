#pragma once
#include "com/google/gson/jxx.com.google.gson.FieldNamingStrategy.h"
namespace com::google::gson {
enum class FieldNamingPolicy {
    IDENTITY,
    UPPER_CAMEL_CASE,
    UPPER_CAMEL_CASE_WITH_SPACES,
    LOWER_CASE_WITH_UNDERSCORES,
    LOWER_CASE_WITH_DASHES,
    LOWER_CASE_WITH_DOTS
};
class FieldNamingPolicies final {
public:
    static ::jxx::Ptr<FieldNamingStrategy> strategy(FieldNamingPolicy policy);
};
} // namespace com::google::gson
