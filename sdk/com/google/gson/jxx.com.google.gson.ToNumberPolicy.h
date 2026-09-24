#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "com/google/gson/jxx.com.google.gson.ToNumberStrategy.h"
namespace com::google::gson {
enum class ToNumberPolicy { DOUBLE, LONG_OR_DOUBLE, BIG_DECIMAL };
class ToNumberPolicies final : public ::jxx::lang::ClassBase<ToNumberPolicies, ::jxx::lang::Object> {
    public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ToNumberPolicies, JxxSuper>; static ::jxx::Ptr<ToNumberStrategy> strategy(ToNumberPolicy policy);
};
} // namespace com::google::gson
