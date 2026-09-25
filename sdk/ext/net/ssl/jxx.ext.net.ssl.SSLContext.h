#pragma once
#include <memory>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::security
{
	class SecureRandom; class Provider;
}
namespace jxx::ext::net::ssl::internal
{
	class OpenSslContextConfig;
}
namespace jxx::ext::net::ssl
{
	class KeyManager; class TrustManager; class SSLSocketFactory; class SSLServerSocketFactory; class SSLSessionContext; class SSLEngine; class SSLParameters;
	class SSLContext final :public ::jxx::lang::ClassBase<SSLContext, ::jxx::lang::Object>
	{
	public:
		using KeyManagerArray = ::jxx::lang::JxxArray<::jxx::Ptr<KeyManager>, 1U>; using TrustManagerArray = ::jxx::lang::JxxArray<::jxx::Ptr<TrustManager>, 1U>;
		explicit SSLContext(const ::jxx::Ptr<::jxx::lang::String>& protocol); ~SSLContext()override;
		static ::jxx::Ptr<SSLContext> getInstance(const ::jxx::Ptr<::jxx::lang::String>& protocol);
		static ::jxx::Ptr<SSLContext> getInstance(const ::jxx::Ptr<::jxx::lang::String>& protocol, const ::jxx::Ptr<::jxx::lang::String>& provider);
		static ::jxx::Ptr<SSLContext> getInstance(const ::jxx::Ptr<::jxx::lang::String>& protocol, const ::jxx::Ptr<::jxx::security::Provider>& provider);
		static ::jxx::Ptr<SSLContext> getDefault(); static void setDefault(const ::jxx::Ptr<SSLContext>& context);
		void init(const ::jxx::Ptr<KeyManagerArray>& keyManagers, const ::jxx::Ptr<TrustManagerArray>& trustManagers, const ::jxx::Ptr<::jxx::security::SecureRandom>& secureRandom);
		::jxx::Ptr<SSLSocketFactory> getSocketFactory(); ::jxx::Ptr<SSLServerSocketFactory> getServerSocketFactory();
		::jxx::Ptr<SSLSessionContext> getClientSessionContext(); ::jxx::Ptr<SSLSessionContext> getServerSessionContext();
		::jxx::Ptr<SSLEngine> createSSLEngine(); ::jxx::Ptr<SSLEngine> createSSLEngine(const ::jxx::Ptr<::jxx::lang::String>& peerHost, ::jxx::lang::jint peerPort);
		::jxx::Ptr<SSLParameters> getDefaultSSLParameters(); ::jxx::Ptr<SSLParameters> getSupportedSSLParameters();
		::jxx::Ptr<::jxx::lang::String> getProtocol()const; ::jxx::Ptr<::jxx::security::Provider> getProvider()const;
		private: 
			::jxx::Ptr<::jxx::lang::String> protocol_; ::jxx::Ptr<::jxx::security::Provider> provider_; std::shared_ptr<::jxx::ext::net::ssl::internal::OpenSslContextConfig> config_;
	};
}
