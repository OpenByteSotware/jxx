#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslTrustManagerFactorySpi.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultTrustManager.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
namespace jxx::ext::net::ssl::internal {
::jxx::Ptr<OpenSslTrustManagerFactorySpi::TrustManagerArray> OpenSslTrustManagerFactorySpi::engineGetTrustManagers(){if(!initialized_)throw ::jxx::lang::IllegalStateException();auto result=::jxx::NEW<TrustManagerArray>(1);(*result)[0]=::jxx::NEW<OpenSslDefaultTrustManager>();return result;}
void OpenSslTrustManagerFactorySpi::engineInit(const ::jxx::Ptr<::jxx::security::KeyStore>& keyStore){if(keyStore!=nullptr)throw ::jxx::lang::IllegalArgumentException("KeyStore-backed trust managers require JXX KeyStore support");initialized_=true;}
void OpenSslTrustManagerFactorySpi::engineInit(const ::jxx::Ptr<::jxx::ext::net::ssl::ManagerFactoryParameters>& parameters){if(parameters!=nullptr)throw ::jxx::lang::IllegalArgumentException("manager parameters are not supported");initialized_=true;}
}
