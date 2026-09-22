#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
class JsonPrimitive;
class ToNumberStrategy : public ::jxx::lang::InterfaceBase<ToNumberStrategy> {
public:
    ~ToNumberStrategy() override = default;
    virtual ::jxx::Ptr<JsonPrimitive> readNumber(
        const ::jxx::Ptr<::jxx::lang::String>& lexicalValue) = 0;
};
} // namespace com::google::gson
