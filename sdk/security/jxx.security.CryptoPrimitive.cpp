#include "security/jxx.security.CryptoPrimitive.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::security {
namespace {
::jxx::Ptr<CryptoPrimitive> primitive(const char* name) {
    return ::jxx::NEW<CryptoPrimitive>(
        ::jxx::NEW<::jxx::lang::String>(name));
}
} // namespace

CryptoPrimitive::CryptoPrimitive(
    const ::jxx::Ptr<::jxx::lang::String>& name)
    : name_(name) {
    if (name_ == nullptr)
        throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> CryptoPrimitive::name() const {
    return name_;
}
#define JXX_CRYPTO_PRIMITIVE(method) \
::jxx::Ptr<CryptoPrimitive> CryptoPrimitive::method() { \
    static const auto value = primitive(#method); \
    return value; \
}
JXX_CRYPTO_PRIMITIVE(MESSAGE_DIGEST)
JXX_CRYPTO_PRIMITIVE(SECURE_RANDOM)
JXX_CRYPTO_PRIMITIVE(BLOCK_CIPHER)
JXX_CRYPTO_PRIMITIVE(STREAM_CIPHER)
JXX_CRYPTO_PRIMITIVE(MAC)
JXX_CRYPTO_PRIMITIVE(KEY_WRAP)
JXX_CRYPTO_PRIMITIVE(PUBLIC_KEY_ENCRYPTION)
JXX_CRYPTO_PRIMITIVE(SIGNATURE)
JXX_CRYPTO_PRIMITIVE(KEY_ENCAPSULATION)
JXX_CRYPTO_PRIMITIVE(KEY_AGREEMENT)
#undef JXX_CRYPTO_PRIMITIVE

} // namespace jxx::security
