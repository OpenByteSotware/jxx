#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.X509Principal.h"
#include <openssl/x509.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLPeerUnverifiedException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl::internal {

namespace {
::jxx::Ptr<::jxx::security::Principal> certificatePrincipal(
    const ::jxx::Ptr<OpenSslSession::CertificateArray>& certificates) {
    if (certificates == nullptr || certificates->length == 0 ||
        (*certificates)[0] == nullptr) return nullptr;
    const auto encoded = (*certificates)[0]->getEncoded();
    if (encoded == nullptr || encoded->length == 0) return nullptr;
    const unsigned char* cursor = reinterpret_cast<const unsigned char*>(&(*encoded)[0]);
    X509* certificate = d2i_X509(nullptr, &cursor, encoded->length);
    if (certificate == nullptr) return nullptr;
    char* text = X509_NAME_oneline(X509_get_subject_name(certificate), nullptr, 0);
    ::jxx::Ptr<::jxx::security::Principal> result;
    if (text != nullptr) {
        result = ::jxx::NEW<X509Principal>(
            ::jxx::NEW<::jxx::lang::String>(text));
        OPENSSL_free(text);
    }
    X509_free(certificate);
    return result;
}
} // namespace

::jxx::lang::jlong OpenSslSession::nowMillis() {
    return static_cast<::jxx::lang::jlong>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
}

OpenSslSession::OpenSslSession(
    const ::jxx::Ptr<::jxx::lang::String>& cipher,
    const ::jxx::Ptr<::jxx::lang::String>& protocol,
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<CertificateArray>& peerCertificates,
    const ::jxx::Ptr<CertificateArray>& localCertificates,
    const ::jxx::lang::ByteArray& id,
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext>& context)
    : cipher_(cipher)
    , protocol_(protocol)
    , host_(host)
    , port_(port)
    , peerCertificates_(peerCertificates)
    , localCertificates_(localCertificates)
    , id_(id == nullptr
          ? ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(0)
          : id)
    , context_(context)
    , concreteContext_(::jxx::CAST<OpenSslSessionContext>(context))
    , creationTime_(nowMillis())
    , lastAccessedTime_(creationTime_) {
}

void OpenSslSession::touch() const { lastAccessedTime_ = nowMillis(); }
::jxx::lang::jint OpenSslSession::getApplicationBufferSize() const { touch(); return 16384; }
::jxx::Ptr<::jxx::lang::String> OpenSslSession::getCipherSuite() const { touch(); return cipher_; }
::jxx::lang::jlong OpenSslSession::getCreationTime() const { return creationTime_; }
::jxx::lang::ByteArray OpenSslSession::getId() const { touch(); const auto r=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte,1U>>(id_->length);for(::jxx::lang::jint i=0;i<id_->length;++i)(*r)[i]=(*id_)[i];return r; }
::jxx::lang::jlong OpenSslSession::getLastAccessedTime() const { return lastAccessedTime_; }
::jxx::Ptr<OpenSslSession::CertificateArray> OpenSslSession::getLocalCertificates() const { touch(); return localCertificates_; }
::jxx::Ptr<::jxx::security::Principal>
OpenSslSession::getLocalPrincipal() const {
    touch();
    return certificatePrincipal(localCertificates_);
}
::jxx::lang::jint OpenSslSession::getPacketBufferSize() const { touch(); return 16709; }
::jxx::Ptr<OpenSslSession::CertificateArray> OpenSslSession::getPeerCertificates() const { touch(); if(peerCertificates_==nullptr)throw ::jxx::ext::net::ssl::SSLPeerUnverifiedException("peer not authenticated");return peerCertificates_; }
::jxx::Ptr<::jxx::lang::String> OpenSslSession::getPeerHost() const { touch(); return host_; }
::jxx::lang::jint OpenSslSession::getPeerPort() const { touch(); return port_; }
::jxx::Ptr<::jxx::security::Principal>
OpenSslSession::getPeerPrincipal() const {
    touch();
    const auto principal = certificatePrincipal(peerCertificates_);
    if (principal == nullptr)
        throw ::jxx::ext::net::ssl::SSLPeerUnverifiedException(
            "peer not authenticated");
    return principal;
}
::jxx::Ptr<::jxx::lang::String> OpenSslSession::getProtocol() const { touch(); return protocol_; }
::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext> OpenSslSession::getSessionContext() const { return context_; }
::jxx::Ptr<::jxx::lang::Object> OpenSslSession::getValue(const ::jxx::Ptr<::jxx::lang::String>& name) const {if(name==nullptr)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex> l(mutex_);auto i=values_.find(name->utf8());return i==values_.end()?nullptr:i->second;}
::jxx::Ptr<OpenSslSession::StringArray> OpenSslSession::getValueNames() const {std::lock_guard<std::mutex> l(mutex_);auto r=::jxx::NEW<StringArray>(static_cast<::jxx::lang::jint>(values_.size()));::jxx::lang::jint i=0;for(const auto&v:values_)(*r)[i++]=::jxx::NEW<::jxx::lang::String>(v.first);return r;}
void OpenSslSession::invalidate() {
    ::jxx::lang::ByteArray sessionId;
    ::jxx::Ptr<::jxx::lang::String> peerHost;
    ::jxx::lang::jint peerPort = -1;
    std::shared_ptr<OpenSslSessionContext> context;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!valid_) return;
        valid_ = false;
        values_.clear();
        sessionId = id_;
        peerHost = host_;
        peerPort = port_;
        context = concreteContext_.lock();
    }
    if (context != nullptr)
        context->invalidateSession(sessionId, peerHost, peerPort);
}
::jxx::lang::jbool OpenSslSession::isValid() const { return valid_; }
void OpenSslSession::putValue(const ::jxx::Ptr<::jxx::lang::String>& name,const ::jxx::Ptr<::jxx::lang::Object>& value){if(name==nullptr||value==nullptr)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex> l(mutex_);values_[name->utf8()]=value;}
void OpenSslSession::removeValue(const ::jxx::Ptr<::jxx::lang::String>& name){if(name==nullptr)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex> l(mutex_);values_.erase(name->utf8());}

} // namespace jxx::ext::net::ssl::internal
