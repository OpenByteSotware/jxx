#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::transform {
class Result : public ::jxx::lang::InterfaceBase<Result> {
public:
    ~Result() override = default;
    virtual void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getSystemId() const = 0;
};
}
