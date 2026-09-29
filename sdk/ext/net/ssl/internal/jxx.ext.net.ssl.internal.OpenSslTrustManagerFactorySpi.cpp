#include "ext/net/ssl/jxx.ext.net.ssl.CertPathTrustManagerParameters.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslTrustManagerFactorySpi.h"
#include <vector>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultTrustManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "security/jxx.security.KeyStore.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"
namespace jxx::ext::net::ssl::internal {
::jxx::Ptr<OpenSslTrustManagerFactorySpi::TrustManagerArray> OpenSslTrustManagerFactorySpi::engineGetTrustManagers(){if(!initialized_)throw ::jxx::lang::IllegalStateException();auto result=::jxx::NEW<TrustManagerArray>(1);(*result)[0]=manager_==nullptr?::jxx::CAST<::jxx::ext::net::ssl::TrustManager>(::jxx::NEW<OpenSslDefaultTrustManager>()):manager_;return result;}
void OpenSslTrustManagerFactorySpi::engineInit(const ::jxx::Ptr<::jxx::security::KeyStore>& keyStore){manager_=nullptr;if(keyStore!=nullptr){const auto certs=keyStore->trustedCertificates();std::vector<::jxx::lang::ByteArray> encoded;for(::jxx::lang::jint i=0;i<certs->length;++i)if((*certs)[i]!=nullptr)encoded.push_back((*certs)[i]->getEncoded());manager_=::jxx::CAST<::jxx::ext::net::ssl::TrustManager>(::jxx::NEW<OpenSslPropertyTrustManager>(encoded));}initialized_=true;}
void OpenSslTrustManagerFactorySpi::engineInit(const ::jxx::Ptr<::jxx::ext::net::ssl::ManagerFactoryParameters>& parameters){manager_=nullptr;if(parameters!=nullptr){const auto certPath=::jxx::CAST<::jxx::ext::net::ssl::CertPathTrustManagerParameters>(parameters);if(certPath==nullptr)throw ::jxx::lang::IllegalArgumentException("unsupported manager parameters");(void)certPath->getParameters();manager_=::jxx::CAST<::jxx::ext::net::ssl::TrustManager>(::jxx::NEW<OpenSslDefaultTrustManager>());}initialized_=true;}
}
