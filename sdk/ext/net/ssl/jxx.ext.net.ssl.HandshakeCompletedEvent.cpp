#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::net::ssl
{
	HandshakeCompletedEvent::HandshakeCompletedEvent(const ::jxx::Ptr<SSLSocket>& s, const ::jxx::Ptr<SSLSession>& x) :Super(::jxx::CAST<::jxx::lang::Object>(s)), session_(x)
	{
		if (s == nullptr || x == nullptr)throw ::jxx::lang::NullPointerException();
	}::jxx::Ptr<SSLSession> HandshakeCompletedEvent::getSession()const
	{
		return session_;
	}::jxx::Ptr<SSLSocket> HandshakeCompletedEvent::getSocket()const
	{
		return ::jxx::CAST<SSLSocket>(getSource());
	}::jxx::Ptr<::jxx::lang::String> HandshakeCompletedEvent::getCipherSuite()const
	{
		return session_->getCipherSuite();
	}::jxx::Ptr<::jxx::security::Principal> HandshakeCompletedEvent::getLocalPrincipal()const
	{
		return session_->getLocalPrincipal();
	}::jxx::Ptr<::jxx::security::Principal> HandshakeCompletedEvent::getPeerPrincipal()const
	{
		return session_->getPeerPrincipal();
	}
}
