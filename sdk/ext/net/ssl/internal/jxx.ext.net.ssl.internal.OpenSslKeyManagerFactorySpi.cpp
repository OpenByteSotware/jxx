#include <vector>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyManagerFactorySpi.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCompositeKeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.KeyStoreBuilderParameters.h"
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
	void OpenSslKeyManagerFactorySpi::engineInit(
		const ::jxx::Ptr<::jxx::ext::net::ssl::ManagerFactoryParameters>& parameters)
	{
		manager_ = nullptr;
		if (parameters != nullptr) {
			const auto builderParameters =
				::jxx::CAST<::jxx::ext::net::ssl::KeyStoreBuilderParameters>(parameters);
			if (builderParameters == nullptr) {
				throw ::jxx::lang::IllegalArgumentException(
					"unsupported manager parameters");
			}
			const auto builders = builderParameters->getParameters();
			std::vector<OpenSslCompositeKeyManager::Manager> managers;
			for (::jxx::lang::jint index = 0; index < builders->length; ++index) {
				const auto builder = (*builders)[index];
				const auto keyStore = builder->getKeyStore();
				const auto protection = builder->getProtectionParameter(
					::jxx::NEW<::jxx::lang::String>("key"));
				const auto passwordProtection =
					::jxx::CAST<::jxx::security::KeyStore::PasswordProtection>(protection);
				managers.push_back(::jxx::NEW<OpenSslKeyStoreKeyManager>(
					keyStore, passwordProtection == nullptr ? nullptr : passwordProtection->getPassword()));
			}
			manager_ = ::jxx::CAST<::jxx::ext::net::ssl::KeyManager>(
				::jxx::NEW<OpenSslCompositeKeyManager>(managers));
		}
		initialized_ = true;
	}
}
