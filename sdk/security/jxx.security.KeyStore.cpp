#include "security/jxx.security.KeyStore.h"

#include <chrono>
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
    return ::jxx::NEW<KeyStore>(::jxx::NEW<::jxx::lang::String>("PKCS12"));
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

    certificateAliases_.clear(); certificates_.clear(); keyAliases_.clear(); privateKeys_.clear(); chains_.clear();
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
        keyAliases_.push_back(::jxx::NEW<::jxx::lang::String>("key"));
        privateKeys_.push_back(::jxx::NEW<::jxx::security::internal::EncodedPrivateKey>(::jxx::NEW<::jxx::lang::String>(id == EVP_PKEY_EC ? "EC" : id == EVP_PKEY_RSA ? "RSA" : "UNKNOWN"), bytesOf(der.data(), length)));
        const auto chain = ::jxx::NEW<CertificateArray>(static_cast<::jxx::lang::jint>(certificates_.size()));
        for (std::size_t i = 0; i < certificates_.size(); ++i) (*chain)[static_cast<::jxx::lang::jint>(i)] = certificates_[i];
        chains_.push_back(chain);
    }
    if (key != nullptr) EVP_PKEY_free(key); if (leaf != nullptr) X509_free(leaf);
    if (extras != nullptr) sk_X509_pop_free(extras, X509_free);
    creationTime_ = static_cast<::jxx::lang::jlong>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    loaded_ = true;
}

::jxx::Ptr<::jxx::lang::String> KeyStore::getType() const { return type_; }
::jxx::lang::jbool KeyStore::isKeyEntry(const ::jxx::Ptr<::jxx::lang::String>& alias) const { if(alias==nullptr)return false; for(const auto& candidate:keyAliases_)if(candidate!=nullptr&&candidate->utf8()==alias->utf8())return true; return false; }
::jxx::lang::jbool KeyStore::isCertificateEntry(const ::jxx::Ptr<::jxx::lang::String>& alias) const { if (alias == nullptr || isKeyEntry(alias)) return false; for (const auto& candidate:certificateAliases_) if (candidate!=nullptr&&alias->utf8()==candidate->utf8()) return true; return false; }
::jxx::lang::jbool KeyStore::containsAlias(const ::jxx::Ptr<::jxx::lang::String>& alias) const { return isKeyEntry(alias) || isCertificateEntry(alias); }
::jxx::lang::jint KeyStore::size() const noexcept { ::jxx::lang::jint count=static_cast<::jxx::lang::jint>(keyAliases_.size()); for(const auto& alias:certificateAliases_){::jxx::lang::jbool key=false;for(const auto& keyAlias:keyAliases_)if(alias!=nullptr&&keyAlias!=nullptr&&alias->utf8()==keyAlias->utf8()){key=true;break;}if(!key)++count;}return count; }
::jxx::Ptr<KeyStore::StringArray> KeyStore::aliases() const { const auto a=::jxx::NEW<StringArray>(size()); ::jxx::lang::jint i=0; for(const auto& alias:keyAliases_)(*a)[i++]=alias; for(const auto& alias:certificateAliases_)if(!isKeyEntry(alias))(*a)[i++]=alias; if(i==a->length)return a; const auto result=::jxx::NEW<StringArray>(i);for(::jxx::lang::jint j=0;j<i;++j)(*result)[j]=(*a)[j];return result; }
::jxx::Ptr<::jxx::security::PrivateKey> KeyStore::getKey(const ::jxx::Ptr<::jxx::lang::String>& alias,const ::jxx::Ptr<CharArray>&) const { if(alias==nullptr)return nullptr;for(std::size_t i=0;i<keyAliases_.size();++i)if(keyAliases_[i]->utf8()==alias->utf8())return privateKeys_[i];return nullptr; }
::jxx::Ptr<KeyStore::CertificateArray> KeyStore::getCertificateChain(const ::jxx::Ptr<::jxx::lang::String>& alias) const { if(alias==nullptr)return nullptr;for(std::size_t n=0;n<keyAliases_.size();++n)if(keyAliases_[n]->utf8()==alias->utf8()){const auto& chain=chains_[n];const auto c=::jxx::NEW<CertificateArray>(chain->length);for(::jxx::lang::jint i=0;i<c->length;++i)(*c)[i]=(*chain)[i];return c;}return nullptr; }
::jxx::Ptr<::jxx::security::cert::Certificate> KeyStore::getCertificate(const ::jxx::Ptr<::jxx::lang::String>& alias) const { const auto chain=getCertificateChain(alias);if(chain!=nullptr&&chain->length>0)return (*chain)[0];if(alias==nullptr)return nullptr;for(std::size_t i=0;i<certificateAliases_.size();++i)if(certificateAliases_[i]->utf8()==alias->utf8())return certificates_[i];return nullptr; }

::jxx::Ptr<KeyStore::X509CertificateArray> KeyStore::trustedCertificates() const {
    ensureLoaded();
    const auto result = ::jxx::NEW<X509CertificateArray>(
        static_cast<::jxx::lang::jint>(certificates_.size()));
    for (std::size_t index = 0; index < certificates_.size(); ++index) {
        (*result)[static_cast<::jxx::lang::jint>(index)] = certificates_[index];
    }
    return result;
}





KeyStore::PasswordProtection::PasswordProtection(
    const ::jxx::Ptr<CharArray>& password) {
    if (password == nullptr) throw ::jxx::lang::NullPointerException();
    password_ = ::jxx::NEW<CharArray>(password->length);
    for (::jxx::lang::jint index = 0; index < password->length; ++index)
        (*password_)[index] = (*password)[index];
}

::jxx::Ptr<KeyStore::CharArray>
KeyStore::PasswordProtection::getPassword() const {
    if (destroyed_) throw ::jxx::lang::IllegalStateException("password destroyed");
    const auto copy = ::jxx::NEW<CharArray>(password_->length);
    for (::jxx::lang::jint index = 0; index < password_->length; ++index)
        (*copy)[index] = (*password_)[index];
    return copy;
}

void KeyStore::PasswordProtection::destroy() {
    if (password_ != nullptr)
        for (::jxx::lang::jint index = 0; index < password_->length; ++index)
            (*password_)[index] = 0;
    password_ = nullptr;
    destroyed_ = true;
}

::jxx::lang::jbool KeyStore::PasswordProtection::isDestroyed() const noexcept {
    return destroyed_;
}

KeyStore::PrivateKeyEntry::PrivateKeyEntry(
    const ::jxx::Ptr<::jxx::security::PrivateKey>& privateKey,
    const ::jxx::Ptr<CertificateArray>& chain)
    : privateKey_(privateKey) {
    if (privateKey == nullptr || chain == nullptr || chain->length == 0)
        throw ::jxx::lang::NullPointerException();
    chain_ = ::jxx::NEW<CertificateArray>(chain->length);
    for (::jxx::lang::jint index = 0; index < chain->length; ++index) {
        if ((*chain)[index] == nullptr) throw ::jxx::lang::NullPointerException();
        (*chain_)[index] = (*chain)[index];
    }
}

::jxx::Ptr<::jxx::security::PrivateKey>
KeyStore::PrivateKeyEntry::getPrivateKey() const { return privateKey_; }

::jxx::Ptr<KeyStore::CertificateArray>
KeyStore::PrivateKeyEntry::getCertificateChain() const {
    const auto copy = ::jxx::NEW<CertificateArray>(chain_->length);
    for (::jxx::lang::jint index = 0; index < chain_->length; ++index)
        (*copy)[index] = (*chain_)[index];
    return copy;
}

::jxx::Ptr<::jxx::security::cert::Certificate>
KeyStore::PrivateKeyEntry::getCertificate() const { return (*chain_)[0]; }

KeyStore::TrustedCertificateEntry::TrustedCertificateEntry(
    const ::jxx::Ptr<::jxx::security::cert::Certificate>& certificate)
    : certificate_(certificate) {
    if (certificate == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::security::cert::Certificate>
KeyStore::TrustedCertificateEntry::getTrustedCertificate() const {
    return certificate_;
}

void KeyStore::ensureLoaded() const {
    if (!loaded_) throw ::jxx::lang::IllegalStateException("KeyStore not loaded");
}

::jxx::Ptr<KeyStore::Entry> KeyStore::getEntry(
    const ::jxx::Ptr<::jxx::lang::String>& alias,
    const ::jxx::Ptr<ProtectionParameter>& protection) const {
    ensureLoaded();
    if (alias == nullptr) throw ::jxx::lang::NullPointerException();
    if (isKeyEntry(alias)) {
        if (protection != nullptr &&
            ::jxx::CAST<PasswordProtection>(protection) == nullptr)
            throw ::jxx::lang::IllegalArgumentException("unsupported protection parameter");
        return ::jxx::NEW<PrivateKeyEntry>(getKey(alias, nullptr), getCertificateChain(alias));
    }
    const auto certificate = getCertificate(alias);
    return certificate == nullptr
        ? nullptr
        : ::jxx::CAST<Entry>(::jxx::NEW<TrustedCertificateEntry>(certificate));
}

void KeyStore::setEntry(
    const ::jxx::Ptr<::jxx::lang::String>& alias,
    const ::jxx::Ptr<Entry>& entry,
    const ::jxx::Ptr<ProtectionParameter>& protection) {
    ensureLoaded();
    if (alias == nullptr || entry == nullptr) throw ::jxx::lang::NullPointerException();
    const auto privateEntry = ::jxx::CAST<PrivateKeyEntry>(entry);
    if (privateEntry != nullptr) {
        const auto passwordProtection = ::jxx::CAST<PasswordProtection>(protection);
        setKeyEntry(alias, privateEntry->getPrivateKey(),
            passwordProtection == nullptr ? nullptr : passwordProtection->getPassword(),
            privateEntry->getCertificateChain());
        return;
    }
    const auto trustedEntry = ::jxx::CAST<TrustedCertificateEntry>(entry);
    if (trustedEntry != nullptr) {
        if (protection != nullptr)
            throw ::jxx::lang::IllegalArgumentException("trusted certificate does not use protection");
        setCertificateEntry(alias, trustedEntry->getTrustedCertificate());
        return;
    }
    throw ::jxx::lang::IllegalArgumentException("unsupported KeyStore entry");
}

void KeyStore::deleteEntry(const ::jxx::Ptr<::jxx::lang::String>& alias) {
    ensureLoaded();
    if (alias == nullptr) throw ::jxx::lang::NullPointerException();
    for (std::size_t index = 0; index < keyAliases_.size(); ++index) {
        if (keyAliases_[index]->utf8() == alias->utf8()) {
            keyAliases_.erase(keyAliases_.begin() + static_cast<std::ptrdiff_t>(index));
            privateKeys_.erase(privateKeys_.begin() + static_cast<std::ptrdiff_t>(index));
            chains_.erase(chains_.begin() + static_cast<std::ptrdiff_t>(index));
            break;
        }
    }
    for (std::size_t index = 0; index < certificateAliases_.size();) {
        if (certificateAliases_[index]->utf8() == alias->utf8()) {
            certificateAliases_.erase(certificateAliases_.begin() + index);
            certificates_.erase(certificates_.begin() + index);
        } else ++index;
    }
}

void KeyStore::setCertificateEntry(
    const ::jxx::Ptr<::jxx::lang::String>& alias,
    const ::jxx::Ptr<::jxx::security::cert::Certificate>& certificate) {
    ensureLoaded();
    if (alias == nullptr || certificate == nullptr) throw ::jxx::lang::NullPointerException();
    if (isKeyEntry(alias)) throw ::jxx::lang::IllegalArgumentException("alias is a key entry");
    const auto x509 = ::jxx::CAST<::jxx::security::cert::X509Certificate>(certificate);
    if (x509 == nullptr) throw ::jxx::lang::IllegalArgumentException("certificate is not X.509");
    for (std::size_t index = 0; index < certificateAliases_.size(); ++index) {
        if (certificateAliases_[index]->utf8() == alias->utf8()) {
            certificates_[index] = x509; return;
        }
    }
    certificateAliases_.push_back(alias);
    certificates_.push_back(x509);
}

void KeyStore::setKeyEntry(
    const ::jxx::Ptr<::jxx::lang::String>& alias,
    const ::jxx::Ptr<::jxx::security::PrivateKey>& key,
    const ::jxx::Ptr<CharArray>&,
    const ::jxx::Ptr<CertificateArray>& chain) {
    ensureLoaded();
    if (alias == nullptr || key == nullptr || chain == nullptr || chain->length == 0)
        throw ::jxx::lang::NullPointerException();
    const auto entry = ::jxx::NEW<PrivateKeyEntry>(key, chain);
    deleteEntry(alias);
    keyAliases_.push_back(alias);
    privateKeys_.push_back(entry->getPrivateKey());
    const auto entryChain = entry->getCertificateChain();
    chains_.push_back(entryChain);
    for (::jxx::lang::jint index = 0; index < entryChain->length; ++index) {
        const auto x509 = ::jxx::CAST<::jxx::security::cert::X509Certificate>((*entryChain)[index]);
        if (x509 == nullptr) throw ::jxx::lang::IllegalArgumentException("certificate is not X.509");
        certificateAliases_.push_back(index == 0 ? alias : ::jxx::NEW<::jxx::lang::String>(alias->utf8() + "-cert-" + std::to_string(index)));
        certificates_.push_back(x509);
    }
}


KeyStore::SecretKeyEntry::SecretKeyEntry(
    const ::jxx::Ptr<::jxx::ext::crypto::SecretKey>& secretKey)
    : secretKey_(secretKey) {
    if (secretKey == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::ext::crypto::SecretKey>
KeyStore::SecretKeyEntry::getSecretKey() const {
    return secretKey_;
}


KeyStore::Builder::Builder(const ::jxx::Ptr<KeyStore>& store,
    const ::jxx::Ptr<ProtectionParameter>& protection)
    : keyStore_(store), protectionParameter_(protection) {}
::jxx::Ptr<KeyStore::Builder> KeyStore::Builder::newInstance(
    const ::jxx::Ptr<KeyStore>& store,
    const ::jxx::Ptr<ProtectionParameter>& protection) {
    if (store == nullptr || protection == nullptr) throw ::jxx::lang::NullPointerException();
    store->ensureLoaded();
    return ::jxx::NEW<Builder>(store, protection);
}
::jxx::Ptr<KeyStore> KeyStore::Builder::getKeyStore() const { return keyStore_; }
::jxx::Ptr<KeyStore::ProtectionParameter> KeyStore::Builder::getProtectionParameter(
    const ::jxx::Ptr<::jxx::lang::String>& alias) const {
    if (alias == nullptr) throw ::jxx::lang::NullPointerException();
    return protectionParameter_;
}


::jxx::Ptr<::jxx::util::Date> KeyStore::getCreationDate(
    const ::jxx::Ptr<::jxx::lang::String>& alias) const {
    ensureLoaded();
    if (alias == nullptr) throw ::jxx::lang::NullPointerException();
    return containsAlias(alias) ? ::jxx::NEW<::jxx::util::Date>(creationTime_) : nullptr;
}

::jxx::Ptr<::jxx::lang::String> KeyStore::getCertificateAlias(
    const ::jxx::Ptr<::jxx::security::cert::Certificate>& certificate) const {
    ensureLoaded();
    if (certificate == nullptr) throw ::jxx::lang::NullPointerException();
    for (std::size_t index=0; index<certificates_.size(); ++index)
        if (certificates_[index]!=nullptr && certificates_[index]->equals(certificate))
            return certificateAliases_[index];
    return nullptr;
}


::jxx::lang::jbool KeyStore::entryInstanceOf(
    const ::jxx::Ptr<::jxx::lang::String>& alias,
    const ::jxx::Ptr<::jxx::lang::ClassAny>& entryClass) const {
    ensureLoaded();
    if (alias == nullptr || entryClass == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto entry = getEntry(alias, nullptr);
    if (entry == nullptr) {
        return false;
    }

    const auto object =
        ::jxx::CAST<::jxx::lang::Object>(entry);
    return object != nullptr &&
        object->instanceOf(entryClass);
}

} // namespace jxx::security
