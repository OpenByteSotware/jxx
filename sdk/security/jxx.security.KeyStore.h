#pragma once

#include <vector>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "security/jxx.security.PrivateKey.h"

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
    ::jxx::Ptr<X509CertificateArray> trustedCertificates() const;

private:
    explicit KeyStore(const ::jxx::Ptr<::jxx::lang::String>& type);

    ::jxx::Ptr<::jxx::lang::String> type_;
    ::jxx::Ptr<::jxx::lang::String> keyAlias_;
    ::jxx::Ptr<::jxx::security::PrivateKey> privateKey_;
    ::jxx::Ptr<CertificateArray> chain_;
    std::vector<::jxx::Ptr<::jxx::lang::String>> certificateAliases_;
    std::vector<::jxx::Ptr<::jxx::security::cert::X509Certificate>> certificates_;
    ::jxx::lang::jbool loaded_ = false;
};

} // namespace jxx::security
