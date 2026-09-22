#pragma once
#include "com/google/gson/jxx.com.google.gson.FieldAttributes.h"
namespace com::google::gson {
class ExclusionStrategy
    : public ::jxx::lang::InterfaceBase<ExclusionStrategy> {
public:
    ~ExclusionStrategy() override = default;
    virtual ::jxx::lang::jbool shouldSkipField(
        const ::jxx::Ptr<FieldAttributes>& field) = 0;
    virtual ::jxx::lang::jbool shouldSkipClass(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) = 0;
};
} // namespace com::google::gson
