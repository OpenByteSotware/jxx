#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::security::cert {
class CertPathParameters : public ::jxx::lang::InterfaceBase<CertPathParameters> {
public:
 ~CertPathParameters() override=default;
 virtual ::jxx::Ptr<::jxx::lang::Object> clone() const=0;
};
}
