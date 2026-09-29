#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyManagerFactorySpi.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyStoreKeyManager.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
namespace jxx::ext::net::ssl::internal
{
	::jxx::Ptr<OpenSslKeyManagerFactorySpi::KeyManagerArray> OpenSslKeyManagerFactorySpi::engineGetKeyManagers()
	{
		if (!initialized_)throw ::jxx::lang::IllegalStateException(); auto result = ::jxx::NEW<KeyManagerArray>(1); (*result)[0] = manager_ == nullptr ? 
			::jxx::CAST<::jxx::ext::net::ssl::KeyManager>(::jxx::NEW<OpenSslDefaultKeyManager>()) : manager_; return result;
	}
	void OpenSslKeyManagerFactorySpi::engineInit(const ::jxx::Ptr<::jxx::security::KeyStore>& keyStore, const ::jxx::Ptr<CharArray>& password)
	{
		manager_ = keyStore == nullptr ? nullptr: ::jxx::CAST<::jxx::ext::net::ssl::KeyManager>(::jxx::NEW<OpenSslKeyStoreKeyManager>(keyStore, password)); initialized_ = true;
	}
	void OpenSslKeyManagerFactorySpi::engineInit(const ::jxx::Ptr<::jxx::ext::net::ssl::ManagerFactoryParameters>& parameters)
	{
		if (parameters != nullptr)throw ::jxx::lang::IllegalArgumentException("manager parameters are not supported"); manager_ = nullptr; initialized_ = true;
	}
}
