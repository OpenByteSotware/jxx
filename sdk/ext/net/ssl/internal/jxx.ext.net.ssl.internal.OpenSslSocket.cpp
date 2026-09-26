#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketNative.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslManagerBridge.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslStreams.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedListener.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "security/jxx.security.SecureRandom.h"
#include "net/jxx.net.Socket.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"

#include <algorithm>
#include <string>
#include <vector>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>

namespace jxx::ext::net::ssl::internal {
namespace {
std::vector<std::string> toVector(const ::jxx::Ptr<OpenSslSocket::StringArray>& v) {
    if (v == nullptr) throw ::jxx::lang::IllegalArgumentException();
    std::vector<std::string> r;
    r.reserve(static_cast<std::size_t>(v->length));
    for (::jxx::lang::jint i = 0; i < v->length; ++i) {
        if ((*v)[i] == nullptr) throw ::jxx::lang::IllegalArgumentException();
        r.push_back((*v)[i]->utf8());
    }
    return r;
}
::jxx::Ptr<OpenSslSocket::StringArray> toArray(const std::vector<std::string>& v) {
    auto r = ::jxx::NEW<OpenSslSocket::StringArray>(static_cast<::jxx::lang::jint>(v.size()));
    for (std::size_t i = 0; i < v.size(); ++i)
        (*r)[static_cast<::jxx::lang::jint>(i)] = ::jxx::NEW<::jxx::lang::String>(v[i]);
    return r;
}
std::string join(const std::vector<std::string>& v) {
    std::string r;
    for (const auto& s : v) { if (!r.empty()) r.push_back(':'); r += s; }
    return r;
}
}

OpenSslSocket::OpenSslSocket(const std::shared_ptr<OpenSslContextConfig>& config)
    : config_(config), native_(new OpenSslSocketNative()),
      host_(::jxx::NEW<::jxx::lang::String>("")) {}

OpenSslSocket::OpenSslSocket(const ::jxx::Ptr<::jxx::lang::String>& h,
    ::jxx::lang::jint p, const std::shared_ptr<OpenSslContextConfig>& config)
    : config_(config), native_(new OpenSslSocketNative()), host_(h), port_(p) {
    if (h == nullptr) throw ::jxx::lang::NullPointerException();
}

OpenSslSocket::OpenSslSocket(const ::jxx::Ptr<::jxx::net::Socket>& transport,
    const ::jxx::Ptr<::jxx::lang::String>& host, ::jxx::lang::jint port,
    ::jxx::lang::jbool autoClose, const std::vector<unsigned char>& consumed,
    const std::shared_ptr<OpenSslContextConfig>& config)
    : config_(config), native_(new OpenSslSocketNative()), host_(host), port_(port),
      transport_(transport), autoClose_(autoClose), consumed_(consumed) {
    if (transport_ == nullptr || host_ == nullptr) throw ::jxx::lang::IllegalArgumentException();
}

OpenSslSocket::~OpenSslSocket() = default;

void OpenSslSocket::startHandshake() {
    if (session_ != nullptr) return;
    if (transport_ == nullptr &&
        (host_ == nullptr || host_->utf8().empty() || port_ <= 0))
        throw ::jxx::lang::IllegalStateException("SSL socket is not connected");
    native_->context = SSL_CTX_new(client_ ? TLS_client_method() : TLS_server_method());
    if (native_->context == nullptr) throw ::jxx::io::IOException("SSL_CTX_new failed");
    int minv = TLS1_2_VERSION, maxv = TLS1_3_VERSION;
    if (!enabledProtocols_.empty()) {
        bool v12 = false, v13 = false;
        for (const auto& p : enabledProtocols_) {
            if (p == "TLSv1.2") v12 = true;
            else if (p == "TLSv1.3") v13 = true;
            else throw ::jxx::lang::IllegalArgumentException();
        }
        minv = v12 ? TLS1_2_VERSION : TLS1_3_VERSION;
        maxv = v13 ? TLS1_3_VERSION : TLS1_2_VERSION;
    }
    if (SSL_CTX_set_min_proto_version(native_->context, minv) != 1 ||
        SSL_CTX_set_max_proto_version(native_->context, maxv) != 1)
        throw ::jxx::io::IOException("Could not apply enabled TLS protocols");
    if (!enabledCipherSuites_.empty()) {
        std::vector<std::string> modern, legacy;
        for (const auto& c : enabledCipherSuites_)
            (c.rfind("TLS_", 0) == 0 ? modern : legacy).push_back(c);
        if (!legacy.empty() && SSL_CTX_set_cipher_list(native_->context, join(legacy).c_str()) != 1)
            throw ::jxx::lang::IllegalArgumentException();
        if (!modern.empty() && SSL_CTX_set_ciphersuites(native_->context, join(modern).c_str()) != 1)
            throw ::jxx::lang::IllegalArgumentException();
    }
    if (config_ != nullptr && config_->secureRandom != nullptr) {
        auto seed = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(64);
        config_->secureRandom->nextBytes(seed);
        RAND_seed(&(*seed)[0], seed->length);
    }
    int verify = SSL_VERIFY_PEER;
    if (!client_ && need_) verify |= SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
    if (!client_ && !need_ && !want_) verify = SSL_VERIFY_NONE;
    SSL_CTX_set_verify(native_->context, verify, nullptr);
    if (SSL_CTX_set_default_verify_paths(native_->context) != 1)
        throw ::jxx::io::IOException("Could not load default trust paths");
    native_->managerBridge = std::make_unique<OpenSslManagerBridge>(config_);
    SSL_CTX_set_cert_verify_callback(native_->context, openSslVerifyCallback, native_->managerBridge.get());
    SSL_CTX_set_client_cert_cb(native_->context, openSslClientCertificateCallback);
    SSL* ssl = SSL_new(native_->context);
    if (ssl == nullptr)
        throw ::jxx::io::IOException("SSL_new failed");
    if (transport_ != nullptr) {
        const auto handle = transport_->nativeSocketHandle();
        if (handle == ::jxx::net::internal::kInvalidSocket) {
            SSL_free(ssl);
            throw ::jxx::io::IOException("layered socket has no native transport");
        }
        BIO* socketBio = BIO_new_socket(
            static_cast<int>(handle), BIO_NOCLOSE);
        if (socketBio == nullptr) {
            SSL_free(ssl);
            throw ::jxx::io::IOException("BIO_new_socket failed");
        }
        SSL_set_bio(ssl, socketBio, socketBio);
        native_->connection = BIO_new(BIO_f_ssl());
        if (native_->connection == nullptr) {
            SSL_free(ssl);
            throw ::jxx::io::IOException("BIO_f_ssl allocation failed");
        }
        BIO_set_ssl(native_->connection, ssl, BIO_CLOSE);
    } else {
        native_->connection = BIO_new_ssl_connect(native_->context);
        if (native_->connection == nullptr) {
            SSL_free(ssl);
            throw ::jxx::io::IOException("BIO_new_ssl_connect failed");
        }
        BIO_get_ssl(native_->connection, &ssl);
        const std::string endpoint = host_->utf8() + ":" + std::to_string(port_);
        BIO_set_conn_hostname(native_->connection, endpoint.c_str());
    }
    SSL_set_ex_data(ssl, openSslManagerBridgeExDataIndex(), native_->managerBridge.get());
    if (client_ && config_ != nullptr && config_->clientSessionContext != nullptr) {
        SSL_SESSION* cached = config_->clientSessionContext->acquireNativeSession(
            host_, port_);
        if (cached != nullptr) {
            const int applied = SSL_set_session(ssl, cached);
            SSL_SESSION_free(cached);
            if (applied != 1)
                throw ::jxx::io::IOException("Could not apply cached TLS session");
        }
    }
    if (client_) {
        SSL_set_connect_state(ssl);
        SSL_set_tlsext_host_name(ssl, host_->utf8().c_str());
        SSL_set1_host(ssl, host_->utf8().c_str());
    } else {
        SSL_set_accept_state(ssl);
        if (native_->managerBridge->selectServerIdentity(ssl) != 1)
            throw ::jxx::io::IOException("No usable server certificate identity");
    }
    if (transport_ == nullptr && client_ &&
        BIO_do_connect(native_->connection) <= 0)
        throw ::jxx::io::IOException("TLS transport connect failed");
    if (!consumed_.empty())
        throw ::jxx::io::IOException(
            "pre-consumed TLS bytes require the SSLEngine memory-BIO path");
    if (BIO_do_handshake(native_->connection) <= 0)
        throw ::jxx::io::IOException("TLS handshake failed");
    SSL_SESSION* ns = SSL_get_session(ssl);
    if (client_ && ns != nullptr && config_ != nullptr &&
        config_->clientSessionContext != nullptr)
        config_->clientSessionContext->registerNativeSession(host_, port_, ns);
    unsigned int idn = 0;
    const unsigned char* id = ns == nullptr ? nullptr : SSL_SESSION_get_id(ns, &idn);
    auto sid = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(static_cast<::jxx::lang::jint>(idn));
    for (unsigned int i = 0; i < idn; ++i) (*sid)[static_cast<::jxx::lang::jint>(i)] = static_cast<::jxx::lang::jbyte>(id[i]);
    auto sc = client_ ? config_->clientSessionContext : config_->serverSessionContext;
    using CA = OpenSslSession::CertificateArray; ::jxx::Ptr<CA> peers, locals;
    STACK_OF(X509)* chain = SSL_get_peer_cert_chain(ssl);
    if (chain != nullptr) {
        int n = sk_X509_num(chain); peers = ::jxx::NEW<CA>(n);
        for (int i = 0; i < n; ++i) {
            X509* cert = sk_X509_value(chain, i); int len = i2d_X509(cert, nullptr);
            auto enc = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(len);
            unsigned char* cur = reinterpret_cast<unsigned char*>(&(*enc)[0]);
            if (len <= 0 || i2d_X509(cert, &cur) != len) throw ::jxx::io::IOException("Could not encode peer certificate");
            (*peers)[i] = ::jxx::CAST<::jxx::security::cert::Certificate>(::jxx::NEW<OpenSslX509Certificate>(enc));
        }
    }
    X509* local = SSL_get_certificate(ssl);
    if (local != nullptr) {
        int len = i2d_X509(local, nullptr); locals = ::jxx::NEW<CA>(1);
        auto enc = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(len);
        unsigned char* cur = reinterpret_cast<unsigned char*>(&(*enc)[0]);
        if (len <= 0 || i2d_X509(local, &cur) != len) throw ::jxx::io::IOException("Could not encode local certificate");
        (*locals)[0] = ::jxx::CAST<::jxx::security::cert::Certificate>(::jxx::NEW<OpenSslX509Certificate>(enc));
    }
    session_ = ::jxx::NEW<OpenSslSession>(::jxx::NEW<::jxx::lang::String>(SSL_get_cipher_name(ssl)),
        ::jxx::NEW<::jxx::lang::String>(SSL_get_version(ssl)), host_, port_, peers, locals, sid, sc);
    if (sc != nullptr) sc->registerSession(session_);
    auto self = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(thisPtr());
    if (self == nullptr) throw ::jxx::lang::IllegalStateException("OpenSslSocket has no JXX-managed self reference");
    auto event = ::jxx::NEW<HandshakeCompletedEvent>(self, session_);
    for (const auto& listener : listeners_) listener->handshakeCompleted(event);
}

::jxx::Ptr<::jxx::io::InputStream> OpenSslSocket::getInputStream(){startHandshake();return ::jxx::NEW<OpenSslInputStream>(this);}
::jxx::Ptr<::jxx::io::OutputStream> OpenSslSocket::getOutputStream(){startHandshake();return ::jxx::NEW<OpenSslOutputStream>(this);}
void OpenSslSocket::close(){native_.reset(new OpenSslSocketNative());session_=nullptr;if(autoClose_&&transport_!=nullptr)transport_->close();}
int OpenSslSocket::tlsRead(unsigned char* b,int n){startHandshake();int r=BIO_read(native_->connection,b,n);return r<=0?-1:r;}
int OpenSslSocket::tlsWrite(const unsigned char* b,int n){startHandshake();int r=BIO_write(native_->connection,b,n);if(r<=0)throw ::jxx::io::IOException("TLS write failed");return r;}
::jxx::Ptr<OpenSslSocket::StringArray> OpenSslSocket::getSupportedProtocols()const{return toArray({"TLSv1.2","TLSv1.3"});}
::jxx::Ptr<OpenSslSocket::StringArray> OpenSslSocket::getEnabledProtocols()const{return enabledProtocols_.empty()?getSupportedProtocols():toArray(enabledProtocols_);}
void OpenSslSocket::setEnabledProtocols(const ::jxx::Ptr<StringArray>&v){if(session_!=nullptr)throw ::jxx::lang::IllegalStateException();enabledProtocols_=toVector(v);}
::jxx::Ptr<OpenSslSocket::StringArray> OpenSslSocket::getSupportedCipherSuites()const{return ::jxx::NEW<StringArray>(0);}
::jxx::Ptr<OpenSslSocket::StringArray> OpenSslSocket::getEnabledCipherSuites()const{return enabledCipherSuites_.empty()?getSupportedCipherSuites():toArray(enabledCipherSuites_);}
void OpenSslSocket::setEnabledCipherSuites(const ::jxx::Ptr<StringArray>&v){if(session_!=nullptr)throw ::jxx::lang::IllegalStateException();enabledCipherSuites_=toVector(v);}
::jxx::Ptr<OpenSslSocket::SSLSession> OpenSslSocket::getSession(){startHandshake();return session_;}
void OpenSslSocket::addHandshakeCompletedListener(const ::jxx::Ptr<HandshakeCompletedListener>&v){if(v==nullptr)throw ::jxx::lang::IllegalArgumentException();listeners_.push_back(v);}
void OpenSslSocket::removeHandshakeCompletedListener(const ::jxx::Ptr<HandshakeCompletedListener>&v){listeners_.erase(std::remove(listeners_.begin(),listeners_.end(),v),listeners_.end());}
void OpenSslSocket::setUseClientMode(::jxx::lang::jbool v){if(session_!=nullptr)throw ::jxx::lang::IllegalStateException();client_=v;}
::jxx::lang::jbool OpenSslSocket::getUseClientMode()const{return client_;}
void OpenSslSocket::setNeedClientAuth(::jxx::lang::jbool v){if(session_!=nullptr)throw ::jxx::lang::IllegalStateException();need_=v;if(v)want_=false;}
::jxx::lang::jbool OpenSslSocket::getNeedClientAuth()const{return need_;}
void OpenSslSocket::setWantClientAuth(::jxx::lang::jbool v){if(session_!=nullptr)throw ::jxx::lang::IllegalStateException();want_=v;if(v)need_=false;}
::jxx::lang::jbool OpenSslSocket::getWantClientAuth()const{return want_;}
void OpenSslSocket::setEnableSessionCreation(::jxx::lang::jbool v){if(session_!=nullptr)throw ::jxx::lang::IllegalStateException();create_=v;}
::jxx::lang::jbool OpenSslSocket::getEnableSessionCreation()const{return create_;}
} // namespace jxx::ext::net::ssl::internal
