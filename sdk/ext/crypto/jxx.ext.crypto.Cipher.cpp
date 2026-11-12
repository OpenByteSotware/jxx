#include "ext/crypto/jxx.ext.crypto.Cipher.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::ext::crypto {
Cipher::Cipher(const ::jxx::Ptr<::jxx::lang::String>& t, const ::jxx::Ptr<::jxx::security::Provider>& p) : transformation_(t), provider_(p) {}
::jxx::Ptr<Cipher> Cipher::getInstance(const ::jxx::Ptr<::jxx::lang::String>& t) { if (t == nullptr) throw ::jxx::lang::NullPointerException(); return ::jxx::NEW<Cipher>(t, nullptr); }
::jxx::Ptr<Cipher> Cipher::getInstance(const ::jxx::Ptr<::jxx::lang::String>& t, const ::jxx::Ptr<::jxx::lang::String>& p) { if (t == nullptr || p == nullptr) throw ::jxx::lang::NullPointerException(); return ::jxx::NEW<Cipher>(t, nullptr); }
::jxx::Ptr<Cipher> Cipher::getInstance(const ::jxx::Ptr<::jxx::lang::String>& t, const ::jxx::Ptr<::jxx::security::Provider>& p) { if (t == nullptr || p == nullptr) throw ::jxx::lang::NullPointerException(); return ::jxx::NEW<Cipher>(t, p); }
::jxx::Ptr<::jxx::security::Provider> Cipher::getProvider() const { return provider_; }
::jxx::Ptr<::jxx::lang::String> Cipher::getAlgorithm() const { return transformation_; }
::jxx::lang::jint Cipher::getBlockSize() const { return 0; }
::jxx::lang::jint Cipher::getOutputSize(::jxx::lang::jint n) const { if (opmode_ == 0) unsupported(); return n; }
::jxx::lang::ByteArray Cipher::getIV() const { return nullptr; }
void Cipher::init(::jxx::lang::jint mode, const ::jxx::Ptr<::jxx::security::Key>& key) { if (key == nullptr) throw ::jxx::lang::NullPointerException(); opmode_ = mode; }
void Cipher::init(::jxx::lang::jint mode, const ::jxx::Ptr<::jxx::security::Key>& key, const ::jxx::Ptr<::jxx::security::SecureRandom>&) { init(mode, key); }
::jxx::lang::ByteArray Cipher::update(const ::jxx::lang::ByteArray&) { unsupported(); }
::jxx::lang::ByteArray Cipher::update(const ::jxx::lang::ByteArray&, ::jxx::lang::jint, ::jxx::lang::jint) { unsupported(); }
::jxx::lang::ByteArray Cipher::doFinal() { unsupported(); }
::jxx::lang::ByteArray Cipher::doFinal(const ::jxx::lang::ByteArray&) { unsupported(); }
::jxx::lang::ByteArray Cipher::doFinal(const ::jxx::lang::ByteArray&, ::jxx::lang::jint, ::jxx::lang::jint) { unsupported(); }
::jxx::lang::jint Cipher::getMaxAllowedKeyLength(const ::jxx::Ptr<::jxx::lang::String>& t) { if (t == nullptr) throw ::jxx::lang::NullPointerException(); return 0x7fffffff; }
::jxx::Ptr<::jxx::lang::String> Cipher::toString() const { return transformation_; }
[[noreturn]] void Cipher::unsupported() { throw ::jxx::lang::UnsupportedOperationException(::jxx::NEW<::jxx::lang::String>("Cipher transformation backend is not implemented")); }
} // namespace jxx::ext::crypto
