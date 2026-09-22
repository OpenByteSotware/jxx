#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace com::google::gson {
class FieldNamingStrategy
    : public ::jxx::lang::InterfaceBase<FieldNamingStrategy> {
public:
    ~FieldNamingStrategy() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> translateName(
        const ::jxx::Ptr<::jxx::lang::String>& fieldName) = 0;
};
} // namespace com::google::gson
