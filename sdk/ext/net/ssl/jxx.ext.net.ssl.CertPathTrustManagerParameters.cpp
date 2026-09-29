#include "ext/net/ssl/jxx.ext.net.ssl.CertPathTrustManagerParameters.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::net::ssl {
CertPathTrustManagerParameters::CertPathTrustManagerParameters(const ::jxx::Ptr<::jxx::security::cert::CertPathParameters>&p){if(p==nullptr)throw ::jxx::lang::NullPointerException();parameters_=::jxx::CAST<::jxx::security::cert::CertPathParameters>(p->clone());if(parameters_==nullptr)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<::jxx::security::cert::CertPathParameters> CertPathTrustManagerParameters::getParameters()const{return ::jxx::CAST<::jxx::security::cert::CertPathParameters>(parameters_->clone());}
}
