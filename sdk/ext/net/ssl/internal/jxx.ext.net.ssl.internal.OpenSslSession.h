#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
namespace jxx::ext::net::ssl::internal
{
	class OpenSslSession final :public ::jxx::lang::ClassBase<OpenSslSession, ::jxx::lang::Object, ::jxx::ext::net::ssl::SSLSession>
	{
	public:OpenSslSession(const ::jxx::Ptr<::jxx::lang::String>& cipher, 
		const ::jxx::Ptr<::jxx::lang::String>& protocol, 
		const ::jxx::Ptr<::jxx::lang::String>& host, ::jxx::lang::jint port); 
		  ::jxx::lang::ByteArray getId()const override; 
		  ::jxx::Ptr<::jxx::lang::String> getCipherSuite()const override;
		  ::jxx::Ptr<::jxx::lang::String> getProtocol()const override;
		  ::jxx::Ptr<::jxx::lang::String> getPeerHost()const override;
		  ::jxx::lang::jint getPeerPort()const override; 
		  ::jxx::Ptr<::jxx::security::Principal> getPeerPrincipal()const override;
		  ::jxx::Ptr<::jxx::security::Principal> getLocalPrincipal()const override;
		  ::jxx::lang::jbool isValid()const override; void invalidate()override;
		  private: 
			  ::jxx::Ptr<::jxx::lang::String> cipher_, protocol_, host_; 
		  ::jxx::lang::jint port_; ::jxx::lang::jbool valid_ = true;
	};
}
