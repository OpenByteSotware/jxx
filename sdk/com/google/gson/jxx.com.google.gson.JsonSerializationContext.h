#pragma once
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace com::google::gson {
class JsonElement;
class JsonSerializationContext
    : public ::jxx::lang::InterfaceBase<JsonSerializationContext> {
public:
    ~JsonSerializationContext() override = default;
    virtual ::jxx::Ptr<JsonElement> serialize(
        const ::jxx::Ptr<::jxx::lang::Object>& source) = 0;
    virtual ::jxx::Ptr<JsonElement> serialize(
        const ::jxx::Ptr<::jxx::lang::Object>& source,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) = 0;
};
} // namespace com::google::gson
