#pragma once
#include <memory>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
namespace jxx::ext::net::ssl::internal {
class OpenSslContextConfig;
class OpenSslSocketFactory final:public ::jxx::lang::ClassBase<OpenSslSocketFactory,::jxx::ext::net::ssl::SSLSocketFactory>{public:explicit OpenSslSocketFactory(const std::shared_ptr<OpenSslContextConfig>& config);
::jxx::Ptr<StringArray> getDefaultCipherSuites()const override;::jxx::Ptr<StringArray> getSupportedCipherSuites()const override;
::jxx::Ptr<::jxx::net::Socket> createSocket()override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::lang::String>&,::jxx::lang::jint)override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::lang::String>&,::jxx::lang::jint,const ::jxx::Ptr<::jxx::net::InetAddress>&,::jxx::lang::jint)override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::InetAddress>&,::jxx::lang::jint)override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::InetAddress>&,::jxx::lang::jint,const ::jxx::Ptr<::jxx::net::InetAddress>&,::jxx::lang::jint)override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::Socket>&,const ::jxx::Ptr<::jxx::lang::String>&,::jxx::lang::jint,::jxx::lang::jbool)override;private:std::shared_ptr<OpenSslContextConfig> config_;};}
