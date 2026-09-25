#pragma once
#include "util/jxx.util.EventObject.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
namespace jxx::ext::net::ssl
{
	class SSLSocket; class SSLSession;
	class HandshakeCompletedEvent final :public ::jxx::lang::ClassBase<HandshakeCompletedEvent, ::jxx::util::EventObject>
	{
	public:using JxxSuper = ::jxx::util::EventObject; using Super = ::jxx::lang::ClassBase<HandshakeCompletedEvent, JxxSuper>; using CertificateArray = HttpsURLConnection::CertificateArray; HandshakeCompletedEvent(const ::jxx::Ptr<SSLSocket>&, const ::jxx::Ptr<SSLSession>&); ::jxx::Ptr<SSLSession> getSession()const; ::jxx::Ptr<SSLSocket> getSocket()const; ::jxx::Ptr<::jxx::lang::String> getCipherSuite()const; ::jxx::Ptr<::jxx::security::Principal> getLocalPrincipal()const; ::jxx::Ptr<::jxx::security::Principal> getPeerPrincipal()const; private: ::jxx::Ptr<SSLSession> session_;
	};
}
