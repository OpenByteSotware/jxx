#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::transform {
class Source : public ::jxx::lang::InterfaceBase<Source> {
public:
    ~Source() override = default;
    virtual void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getSystemId() const = 0;
};
}
