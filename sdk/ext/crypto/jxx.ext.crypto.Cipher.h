#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.Key.h"
#include "security/jxx.security.Provider.h"
#include "security/jxx.security.SecureRandom.h"

namespace jxx::ext::crypto {

class Cipher final : public ::jxx::lang::ClassBase<Cipher, ::jxx::lang::Object> {
public:
    static constexpr ::jxx::lang::jint ENCRYPT_MODE = 1;
    static constexpr ::jxx::lang::jint DECRYPT_MODE = 2;
    static constexpr ::jxx::lang::jint WRAP_MODE = 3;
    static constexpr ::jxx::lang::jint UNWRAP_MODE = 4;
    static constexpr ::jxx::lang::jint PUBLIC_KEY = 1;
    static constexpr ::jxx::lang::jint PRIVATE_KEY = 2;
    static constexpr ::jxx::lang::jint SECRET_KEY = 3;

    static ::jxx::Ptr<Cipher> getInstance(const ::jxx::Ptr<::jxx::lang::String>& transformation);
    static ::jxx::Ptr<Cipher> getInstance(const ::jxx::Ptr<::jxx::lang::String>& transformation, const ::jxx::Ptr<::jxx::lang::String>& provider);
    static ::jxx::Ptr<Cipher> getInstance(const ::jxx::Ptr<::jxx::lang::String>& transformation, const ::jxx::Ptr<::jxx::security::Provider>& provider);

    ::jxx::Ptr<::jxx::security::Provider> getProvider() const;
    ::jxx::Ptr<::jxx::lang::String> getAlgorithm() const;
    ::jxx::lang::jint getBlockSize() const;
    ::jxx::lang::jint getOutputSize(::jxx::lang::jint inputLen) const;
    ::jxx::lang::ByteArray getIV() const;
    void init(::jxx::lang::jint opmode, const ::jxx::Ptr<::jxx::security::Key>& key);
    void init(::jxx::lang::jint opmode, const ::jxx::Ptr<::jxx::security::Key>& key, const ::jxx::Ptr<::jxx::security::SecureRandom>& random);
    ::jxx::lang::ByteArray update(const ::jxx::lang::ByteArray& input);
    ::jxx::lang::ByteArray update(const ::jxx::lang::ByteArray& input, ::jxx::lang::jint inputOffset, ::jxx::lang::jint inputLen);
    ::jxx::lang::ByteArray doFinal();
    ::jxx::lang::ByteArray doFinal(const ::jxx::lang::ByteArray& input);
    ::jxx::lang::ByteArray doFinal(const ::jxx::lang::ByteArray& input, ::jxx::lang::jint inputOffset, ::jxx::lang::jint inputLen);
    static ::jxx::lang::jint getMaxAllowedKeyLength(const ::jxx::Ptr<::jxx::lang::String>& transformation);
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

public:
    // Internal construction path used by getInstance and jxx::NEW.
    explicit Cipher(const ::jxx::Ptr<::jxx::lang::String>& transformation, const ::jxx::Ptr<::jxx::security::Provider>& provider);
    [[noreturn]] static void unsupported();
    ::jxx::Ptr<::jxx::lang::String> transformation_;
    ::jxx::Ptr<::jxx::security::Provider> provider_;
    ::jxx::lang::jint opmode_ = 0;
};

} // namespace jxx::ext::crypto
