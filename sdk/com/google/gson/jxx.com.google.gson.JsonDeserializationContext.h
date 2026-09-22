#pragma once
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace com::google::gson {
class JsonElement;
class JsonDeserializationContext
    : public ::jxx::lang::InterfaceBase<JsonDeserializationContext> {
public:
    ~JsonDeserializationContext() override = default;
    virtual ::jxx::Ptr<::jxx::lang::Object> deserialize(
        const ::jxx::Ptr<JsonElement>& json,
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) = 0;
};
} // namespace com::google::gson
