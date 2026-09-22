#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace com::google::gson {
class InstanceCreator
    : public ::jxx::lang::InterfaceBase<InstanceCreator> {
public:
    ~InstanceCreator() override = default;
    virtual ::jxx::Ptr<::jxx::lang::Object> createInstance() = 0;
};
} // namespace com::google::gson
