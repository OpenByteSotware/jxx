#pragma once
#include "com/google/gson/jxx.com.google.gson.ToNumberStrategy.h"
namespace com::google::gson {
enum class ToNumberPolicy { DOUBLE, LONG_OR_DOUBLE, BIG_DECIMAL };
class ToNumberPolicies final {
    public: static ::jxx::Ptr<ToNumberStrategy> strategy(ToNumberPolicy policy);
};
} // namespace com::google::gson
