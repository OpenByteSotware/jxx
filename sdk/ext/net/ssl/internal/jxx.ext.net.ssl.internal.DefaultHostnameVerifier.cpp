#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DefaultHostnameVerifier.h"
#include <openssl/x509.h>
#include <openssl/x509v3.h>
namespace jxx::ext::net::ssl::internal {
::jxx::lang::jbool DefaultHostnameVerifier::verify(const ::jxx::Ptr<::jxx::lang::String>&h,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>&s){if(h==nullptr||s==nullptr)return false;const auto c=s->getPeerCertificates();if(c==nullptr||c->length==0||(*c)[0]==nullptr)return false;const auto e=(*c)[0]->getEncoded();const unsigned char*p=reinterpret_cast<const unsigned char*>(&(*e)[0]);X509*x=d2i_X509(nullptr,&p,e->length);if(x==nullptr)return false;const auto n=h->utf8();const int ip=X509_check_ip_asc(x,n.c_str(),0);const int r=ip==0?X509_check_host(x,n.c_str(),n.size(),X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS,nullptr):ip;X509_free(x);return r==1;}}
