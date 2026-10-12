#include "security/jxx.security.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.AlgorithmConstraints.h"
#include "security/jxx.security.AlgorithmParameters.h"
#include "security/jxx.security.BasicPermission.h"
#include "security/jxx.security.CryptoPrimitive.h"
#include "security/jxx.security.GeneralSecurityException.h"
#include "security/jxx.security.Guard.h"
#include "security/jxx.security.Key.h"
#include "security/jxx.security.KeyManagementException.h"
#include "security/jxx.security.KeyStore.h"
#include "security/jxx.security.KeyStoreException.h"
#include "security/jxx.security.NoSuchAlgorithmException.h"
#include "security/jxx.security.NoSuchProviderException.h"
#include "security/jxx.security.Permission.h"
#include "security/jxx.security.Principal.h"
#include "security/jxx.security.PrivateKey.h"
#include "security/jxx.security.Provider.h"
#include "security/jxx.security.PublicKey.h"
#include "security/jxx.security.SecureString.h"
#include "security/jxx.security.UnrecoverableKeyException.h"

namespace jxx::security {
namespace {

template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
            registerType<AlgorithmConstraints>();
            registerType<AlgorithmParameters>();
            registerType<BasicPermission>();
            registerType<CryptoPrimitive>();
            registerType<GeneralSecurityException>();
            registerType<Guard>();
            registerType<Key>();
            registerType<KeyManagementException>();
            registerType<KeyStore>();
            registerType<KeyStoreException>();
            registerType<NoSuchAlgorithmException>();
            registerType<NoSuchProviderException>();
            registerType<Permission>();
            registerType<Principal>();
            registerType<PrivateKey>();
            registerType<Provider>();
            registerType<PublicKey>();
            registerType<SecureString>();
            registerType<UnrecoverableKeyException>();
    });
}

} // namespace jxx::security
