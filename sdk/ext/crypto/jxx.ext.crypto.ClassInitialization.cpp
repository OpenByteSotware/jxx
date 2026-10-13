#include "ext/crypto/jxx.ext.crypto.ClassInitialization.h"
#include <mutex>
#include "ext/crypto/jxx.ext.crypto.SecretKey.h"
namespace jxx::ext::crypto {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::InterfaceBase<
            SecretKey,
            ::jxx::security::Key,
            ::jxx::ext::security::auth::Destroyable>::Class();
    });
}
} // namespace jxx::ext::crypto
