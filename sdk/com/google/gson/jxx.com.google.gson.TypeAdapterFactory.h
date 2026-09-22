#pragma once
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "lang/jxx.lang.Class.h"
namespace com::google::gson {
class TypeAdapterFactory
    : public ::jxx::lang::InterfaceBase<TypeAdapterFactory> {
public:
    ~TypeAdapterFactory() override = default;
    virtual ::jxx::Ptr<TypeAdapter> create(
        const ::jxx::Ptr<::jxx::lang::ClassAny>& type) = 0;
};
} // namespace com::google::gson
