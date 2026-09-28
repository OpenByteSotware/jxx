#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyManagerFactorySpi.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultKeyManager.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
namespace jxx::ext::net::ssl::internal {
::jxx::Ptr<OpenSslKeyManagerFactorySpi::KeyManagerArray> OpenSslKeyManagerFactorySpi::engineGetKeyManagers(){if(!initialized_)throw ::jxx::lang::IllegalStateException();auto result=::jxx::NEW<KeyManagerArray>(1);(*result)[0]=::jxx::NEW<OpenSslDefaultKeyManager>();return result;}
void OpenSslKeyManagerFactorySpi::engineInit(const ::jxx::Ptr<::jxx::security::KeyStore>& keyStore,const ::jxx::Ptr<CharArray>&){if(keyStore!=nullptr)throw ::jxx::lang::IllegalArgumentException("KeyStore-backed key managers require JXX KeyStore support");initialized_=true;}
void OpenSslKeyManagerFactorySpi::engineInit(const ::jxx::Ptr<::jxx::ext::net::ssl::ManagerFactoryParameters>& parameters){if(parameters!=nullptr)throw ::jxx::lang::IllegalArgumentException("manager parameters are not supported");initialized_=true;}
}
