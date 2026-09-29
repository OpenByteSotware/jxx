#include "security/jxx.security.KeyStore.h"

#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"
#include "security/internal/jxx.security.internal.EncodedPrivateKey.h"

namespace jxx::security {
namespace {

::jxx::lang::ByteArray bytesOf(const unsigned char* data, int length) {
    const auto result = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
    for (int i = 0; i < length; ++i) (*result)[i] = static_cast<::jxx::lang::jbyte>(data[i]);
    return result;
}

std::string passwordOf(const ::jxx::Ptr<KeyStore::CharArray>& password) {
    std::string result;
    if (password == nullptr) return result;
    result.reserve(static_cast<std::size_t>(password->length));
    for (::jxx::lang::jint i = 0; i < password->length; ++i)
        result.push_back(static_cast<char>((*password)[i] & 0xFF));
    return result;
}

} // namespace

KeyStore::KeyStore(const ::jxx::Ptr<::jxx::lang::String>& type) : type_(type) {}

::jxx::Ptr<KeyStore> KeyStore::getInstance(const ::jxx::Ptr<::jxx::lang::String>& type) {
    if (type == nullptr) throw ::jxx::lang::NullPointerException();
    if (type->utf8() != "PKCS12" && type->utf8() != "PKCS#12")
        throw ::jxx::lang::IllegalArgumentException("unsupported KeyStore type");
    return ::jxx::Ptr<KeyStore>(new KeyStore(::jxx::NEW<::jxx::lang::String>("PKCS12")));
}

::jxx::Ptr<::jxx::lang::String> KeyStore::getDefaultType() {
    return ::jxx::NEW<::jxx::lang::String>("PKCS12");
}

void KeyStore::load(const ::jxx::Ptr<::jxx::io::InputStream>& stream,
                    const ::jxx::Ptr<CharArray>& password) {
    if (stream == nullptr) throw ::jxx::lang::NullPointerException();
    std::vector<unsigned char> bytes;
    const auto buffer = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(4096);
    for (;;) {
        const auto count = stream->read(buffer, 0, buffer->length);
        if (count < 0) break;
        for (::jxx::lang::jint i = 0; i < count; ++i)
            bytes.push_back(static_cast<unsigned char>((*buffer)[i]));
    }
    if (bytes.empty()) throw ::jxx::lang::IllegalArgumentException("empty PKCS12 KeyStore");
    const unsigned char* cursor = bytes.data();
    PKCS12* container = d2i_PKCS12(nullptr, &cursor, static_cast<long>(bytes.size()));
    if (container == nullptr || cursor != bytes.data() + bytes.size()) {
        if (container != nullptr) PKCS12_free(container);
        throw ::jxx::lang::IllegalArgumentException("invalid PKCS12 KeyStore");
    }
    EVP_PKEY* key = nullptr; X509* leaf = nullptr; STACK_OF(X509)* extras = nullptr;
    const auto passwordText = passwordOf(password);
    const int parsed = PKCS12_parse(container, passwordText.c_str(), &key, &leaf, &extras);
    PKCS12_free(container);
    if (parsed != 1) throw ::jxx::lang::IllegalArgumentException("cannot unlock PKCS12 KeyStore");

    certificateAliases_.clear(); certificates_.clear(); chain_ = nullptr; privateKey_ = nullptr;
    if (leaf != nullptr) {
        const int length = i2d_X509(leaf, nullptr); std::vector<unsigned char> der(length);
        unsigned char* out = der.data(); i2d_X509(leaf, &out);
        certificates_.push_back(::jxx::NEW<::jxx::ext::net::ssl::internal::OpenSslX509Certificate>(bytesOf(der.data(), length)));
        certificateAliases_.push_back(::jxx::NEW<::jxx::lang::String>("key"));
    }
    const int extraCount = extras == nullptr ? 0 : sk_X509_num(extras);
    for (int index = 0; index < extraCount; ++index) {
        X509* cert = sk_X509_value(extras, index); const int length = i2d_X509(cert, nullptr);
        if (length <= 0) continue; std::vector<unsigned char> der(length); unsigned char* out = der.data(); i2d_X509(cert, &out);
        certificates_.push_back(::jxx::NEW<::jxx::ext::net::ssl::internal::OpenSslX509Certificate>(bytesOf(der.data(), length)));
        certificateAliases_.push_back(::jxx::NEW<::jxx::lang::String>("cert-" + std::to_string(index + 1)));
    }
    if (key != nullptr && leaf != nullptr) {
        const int length = i2d_PrivateKey(key, nullptr); std::vector<unsigned char> der(length);
        unsigned char* out = der.data(); i2d_PrivateKey(key, &out);
        const int id = EVP_PKEY_base_id(key);
        privateKey_ = ::jxx::NEW<::jxx::security::internal::EncodedPrivateKey>(::jxx::NEW<::jxx::lang::String>(id == EVP_PKEY_EC ? "EC" : id == EVP_PKEY_RSA ? "RSA" : "UNKNOWN"), bytesOf(der.data(), length));
        keyAlias_ = ::jxx::NEW<::jxx::lang::String>("key");
        chain_ = ::jxx::NEW<CertificateArray>(static_cast<::jxx::lang::jint>(certificates_.size()));
        for (std::size_t i = 0; i < certificates_.size(); ++i) (*chain_)[static_cast<::jxx::lang::jint>(i)] = certificates_[i];
    }
    if (key != nullptr) EVP_PKEY_free(key); if (leaf != nullptr) X509_free(leaf);
    if (extras != nullptr) sk_X509_pop_free(extras, X509_free);
    loaded_ = true;
}

::jxx::Ptr<::jxx::lang::String> KeyStore::getType() const { return type_; }
::jxx::lang::jbool KeyStore::isKeyEntry(const ::jxx::Ptr<::jxx::lang::String>& alias) const { return alias != nullptr && keyAlias_ != nullptr && alias->utf8() == keyAlias_->utf8(); }
::jxx::lang::jbool KeyStore::isCertificateEntry(const ::jxx::Ptr<::jxx::lang::String>& alias) const { if (alias == nullptr) return false; for (std::size_t i=0;i<certificateAliases_.size();++i) if (alias->utf8()==certificateAliases_[i]->utf8() && !isKeyEntry(alias)) return true; return false; }
::jxx::lang::jbool KeyStore::containsAlias(const ::jxx::Ptr<::jxx::lang::String>& alias) const { return isKeyEntry(alias) || isCertificateEntry(alias); }
::jxx::lang::jint KeyStore::size() const noexcept { return static_cast<::jxx::lang::jint>(certificateAliases_.size()); }
::jxx::Ptr<KeyStore::StringArray> KeyStore::aliases() const { const auto a=::jxx::NEW<StringArray>(size()); for(::jxx::lang::jint i=0;i<a->length;++i)(*a)[i]=certificateAliases_[i]; return a; }
::jxx::Ptr<::jxx::security::PrivateKey> KeyStore::getKey(const ::jxx::Ptr<::jxx::lang::String>& alias,const ::jxx::Ptr<CharArray>&) const { return isKeyEntry(alias)?privateKey_:nullptr; }
::jxx::Ptr<KeyStore::CertificateArray> KeyStore::getCertificateChain(const ::jxx::Ptr<::jxx::lang::String>& alias) const { if(!isKeyEntry(alias)||chain_==nullptr)return nullptr; const auto c=::jxx::NEW<CertificateArray>(chain_->length);for(::jxx::lang::jint i=0;i<c->length;++i)(*c)[i]=(*chain_)[i];return c; }
::jxx::Ptr<::jxx::security::cert::Certificate> KeyStore::getCertificate(const ::jxx::Ptr<::jxx::lang::String>& alias) const { if(alias==nullptr)return nullptr;for(std::size_t i=0;i<certificateAliases_.size();++i)if(alias->utf8()==certificateAliases_[i]->utf8())return certificates_[i];return nullptr; }
::jxx::Ptr<KeyStore::X509CertificateArray> KeyStore::trustedCertificates() const { const auto r=::jxx::NEW<X509CertificateArray>(static_cast<::jxx::lang::jint>(certificates_.size()));for(std::size_t i=0;i<certificates_.size();++i)(*r)[static_cast<::jxx::lang::jint>(i)]=certificates_[i];return r; }

} // namespace jxx::security
