#pragma once

#include <vector>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "security/jxx.security.PrivateKey.h"
#include "util/jxx.util.Date.h"
#include "ext/crypto/jxx.ext.crypto.SecretKey.h"

namespace jxx::io { class InputStream; }
namespace jxx::security::cert { class Certificate; class X509Certificate; }

namespace jxx::security {

class KeyStore final
    : public ::jxx::lang::ClassBase<KeyStore, ::jxx::lang::Object> {
public:
    using CharArray = ::jxx::lang::JxxArray<::jxx::lang::jchar, 1U>;
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;
    using CertificateArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::security::cert::Certificate>, 1U>;
    using X509CertificateArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::security::cert::X509Certificate>, 1U>;

    class Entry : public ::jxx::lang::InterfaceBase<Entry> {
    public:
        ~Entry() override = default;
    };

    class ProtectionParameter
        : public ::jxx::lang::InterfaceBase<ProtectionParameter> {
    public:
        ~ProtectionParameter() override = default;
    };

    class PasswordProtection final
        : public ::jxx::lang::ClassBase<
              PasswordProtection,
              ::jxx::lang::Object,
              ProtectionParameter> {
    public:
        explicit PasswordProtection(const ::jxx::Ptr<CharArray>& password);
        ::jxx::Ptr<CharArray> getPassword() const;
        void destroy();
        ::jxx::lang::jbool isDestroyed() const noexcept;
    private:
        ::jxx::Ptr<CharArray> password_;
        ::jxx::lang::jbool destroyed_ = false;
    };

    class PrivateKeyEntry final
        : public ::jxx::lang::ClassBase<
              PrivateKeyEntry,
              ::jxx::lang::Object,
              Entry> {
    public:
        PrivateKeyEntry(
            const ::jxx::Ptr<::jxx::security::PrivateKey>& privateKey,
            const ::jxx::Ptr<CertificateArray>& chain);
        ::jxx::Ptr<::jxx::security::PrivateKey> getPrivateKey() const;
        ::jxx::Ptr<CertificateArray> getCertificateChain() const;
        ::jxx::Ptr<::jxx::security::cert::Certificate> getCertificate() const;
    private:
        ::jxx::Ptr<::jxx::security::PrivateKey> privateKey_;
        ::jxx::Ptr<CertificateArray> chain_;
    };

    class SecretKeyEntry final
        : public ::jxx::lang::ClassBase<
              SecretKeyEntry,
              ::jxx::lang::Object,
              Entry> {
    public:
        explicit SecretKeyEntry(
            const ::jxx::Ptr<::jxx::ext::crypto::SecretKey>& secretKey);
        ::jxx::Ptr<::jxx::ext::crypto::SecretKey> getSecretKey() const;
    private:
        ::jxx::Ptr<::jxx::ext::crypto::SecretKey> secretKey_;
    };

    class TrustedCertificateEntry final
        : public ::jxx::lang::ClassBase<
              TrustedCertificateEntry,
              ::jxx::lang::Object,
              Entry> {
    public:
        explicit TrustedCertificateEntry(
            const ::jxx::Ptr<::jxx::security::cert::Certificate>& certificate);
        ::jxx::Ptr<::jxx::security::cert::Certificate>
        getTrustedCertificate() const;
    private:
        ::jxx::Ptr<::jxx::security::cert::Certificate> certificate_;
    };

    class Builder final : public ::jxx::lang::ClassBase<Builder, ::jxx::lang::Object> {
    public:
        static ::jxx::Ptr<Builder> newInstance(
            const ::jxx::Ptr<KeyStore>& keyStore,
            const ::jxx::Ptr<ProtectionParameter>& protectionParameter);
        ::jxx::Ptr<KeyStore> getKeyStore() const;
        ::jxx::Ptr<ProtectionParameter> getProtectionParameter(
            const ::jxx::Ptr<::jxx::lang::String>& alias) const;
    private:
        Builder(const ::jxx::Ptr<KeyStore>& keyStore,
                const ::jxx::Ptr<ProtectionParameter>& protectionParameter);
        ::jxx::Ptr<KeyStore> keyStore_;
        ::jxx::Ptr<ProtectionParameter> protectionParameter_;
    };

    static ::jxx::Ptr<KeyStore> getInstance(
        const ::jxx::Ptr<::jxx::lang::String>& type);
    static ::jxx::Ptr<::jxx::lang::String> getDefaultType();

    void load(
        const ::jxx::Ptr<::jxx::io::InputStream>& stream,
        const ::jxx::Ptr<CharArray>& password);

    ::jxx::Ptr<::jxx::lang::String> getType() const;
    ::jxx::lang::jbool isKeyEntry(
        const ::jxx::Ptr<::jxx::lang::String>& alias) const;
    ::jxx::lang::jbool isCertificateEntry(
        const ::jxx::Ptr<::jxx::lang::String>& alias) const;
    ::jxx::lang::jbool containsAlias(
        const ::jxx::Ptr<::jxx::lang::String>& alias) const;
    ::jxx::lang::jint size() const noexcept;
    ::jxx::Ptr<StringArray> aliases() const;
    ::jxx::Ptr<::jxx::security::PrivateKey> getKey(
        const ::jxx::Ptr<::jxx::lang::String>& alias,
        const ::jxx::Ptr<CharArray>& password) const;
    ::jxx::Ptr<CertificateArray> getCertificateChain(
        const ::jxx::Ptr<::jxx::lang::String>& alias) const;
    ::jxx::Ptr<::jxx::security::cert::Certificate> getCertificate(
        const ::jxx::Ptr<::jxx::lang::String>& alias) const;
    ::jxx::Ptr<::jxx::util::Date> getCreationDate(
        const ::jxx::Ptr<::jxx::lang::String>& alias) const;
    ::jxx::Ptr<::jxx::lang::String> getCertificateAlias(
        const ::jxx::Ptr<::jxx::security::cert::Certificate>& certificate) const;

    ::jxx::Ptr<Entry> getEntry(
        const ::jxx::Ptr<::jxx::lang::String>& alias,
        const ::jxx::Ptr<ProtectionParameter>& protection) const;
    void setEntry(
        const ::jxx::Ptr<::jxx::lang::String>& alias,
        const ::jxx::Ptr<Entry>& entry,
        const ::jxx::Ptr<ProtectionParameter>& protection);
    void deleteEntry(
        const ::jxx::Ptr<::jxx::lang::String>& alias);
    void setCertificateEntry(
        const ::jxx::Ptr<::jxx::lang::String>& alias,
        const ::jxx::Ptr<::jxx::security::cert::Certificate>& certificate);
    void setKeyEntry(
        const ::jxx::Ptr<::jxx::lang::String>& alias,
        const ::jxx::Ptr<::jxx::security::PrivateKey>& key,
        const ::jxx::Ptr<CharArray>& password,
        const ::jxx::Ptr<CertificateArray>& chain);

    ::jxx::Ptr<X509CertificateArray> trustedCertificates() const;

private:
    explicit KeyStore(const ::jxx::Ptr<::jxx::lang::String>& type);
    void ensureLoaded() const;

    ::jxx::Ptr<::jxx::lang::String> type_;
    ::jxx::Ptr<::jxx::lang::String> keyAlias_;
    ::jxx::Ptr<::jxx::security::PrivateKey> privateKey_;
    ::jxx::Ptr<CertificateArray> chain_;
    std::vector<::jxx::Ptr<::jxx::lang::String>> certificateAliases_;
    std::vector<::jxx::Ptr<::jxx::security::cert::X509Certificate>> certificates_;
    ::jxx::lang::jlong creationTime_ = 0;
    ::jxx::lang::jbool loaded_ = false;
};

} // namespace jxx::security
