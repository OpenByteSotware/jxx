#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::security {
class Provider : public ::jxx::lang::ClassBase<Provider,::jxx::lang::Object> {
public:
    explicit Provider(const ::jxx::Ptr<::jxx::lang::String>& name);
    ::jxx::Ptr<::jxx::lang::String> getName() const;
private:
    ::jxx::Ptr<::jxx::lang::String> name_;
};
}
