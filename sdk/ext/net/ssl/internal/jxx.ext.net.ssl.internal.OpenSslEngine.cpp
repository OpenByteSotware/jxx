#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslEngine.h"

#include <algorithm>
#include <string>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslManagerBridge.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLHandshakeException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SNIHostName.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "util/jxx.util.ArrayList.h"
#include <openssl/x509.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLPeerUnverifiedException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLProtocolException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "nio/jxx.nio.ByteBuffer.h"

namespace jxx::ext::net::ssl::internal {
namespace {

::jxx::Ptr<SSLEngineResult> result(
    SSLEngineResult::Status status,
    SSLEngineResult::HandshakeStatus handshake,
    ::jxx::lang::jint consumed,
    ::jxx::lang::jint produced) {
    return ::jxx::NEW<SSLEngineResult>(
        status, handshake, consumed, produced);
}

std::string join(const std::vector<std::string>& values) {
    std::string joined;
    for (const auto& value : values) {
        if (!joined.empty()) joined.push_back(':');
        joined += value;
    }
    return joined;
}

} // namespace

OpenSslEngine::OpenSslEngine(
    const std::shared_ptr<OpenSslContextConfig>& config,
    const ::jxx::Ptr<::jxx::lang::String>& peerHost,
    ::jxx::lang::jint peerPort)
    : Super(peerHost, peerPort)
    , config_(config) {
    if (peerPort < -1 || peerPort > 65535)
        throw ::jxx::lang::IllegalArgumentException();
}

OpenSslEngine::~OpenSslEngine() {
    if (ssl_ != nullptr) {
        SSL_free(ssl_);
        ssl_ = nullptr;
        inboundBio_ = nullptr;
        outboundBio_ = nullptr;
    }
    if (context_ != nullptr) {
        SSL_CTX_free(context_);
        context_ = nullptr;
    }
}

void OpenSslEngine::ensureInitialized() {
    if (initialized_) return;
    context_ = SSL_CTX_new(TLS_method());
    if (context_ == nullptr)
        throw ::jxx::ext::net::ssl::SSLProtocolException(
            "SSL_CTX_new failed");
    if (SSL_CTX_set_min_proto_version(context_, TLS1_2_VERSION) != 1 ||
        SSL_CTX_set_max_proto_version(context_, TLS1_3_VERSION) != 1)
        throw ::jxx::ext::net::ssl::SSLProtocolException(
            "could not configure TLS protocol range");

    if (!enabledCipherSuites_.empty()) {
        std::vector<std::string> tls13;
        std::vector<std::string> legacy;
        for (const auto& suite : enabledCipherSuites_)
            (suite.rfind("TLS_", 0) == 0 ? tls13 : legacy).push_back(suite);
        if (!legacy.empty() &&
            SSL_CTX_set_cipher_list(context_, join(legacy).c_str()) != 1)
            throw ::jxx::lang::IllegalArgumentException();
        if (!tls13.empty() &&
            SSL_CTX_set_ciphersuites(context_, join(tls13).c_str()) != 1)
            throw ::jxx::lang::IllegalArgumentException();
    }

    managerBridge_ = std::make_unique<OpenSslManagerBridge>(config_);
    SSL_CTX_set_cert_verify_callback(
        context_, openSslVerifyCallback, managerBridge_.get());
    SSL_CTX_set_client_cert_cb(context_, openSslClientCertificateCallback);
    SSL_CTX_set_default_verify_paths(context_);

    ssl_ = SSL_new(context_);
    if (ssl_ == nullptr)
        throw ::jxx::ext::net::ssl::SSLProtocolException("SSL_new failed");
    inboundBio_ = BIO_new(BIO_s_mem());
    outboundBio_ = BIO_new(BIO_s_mem());
    if (inboundBio_ == nullptr || outboundBio_ == nullptr)
        throw ::jxx::ext::net::ssl::SSLProtocolException(
            "memory BIO allocation failed");
    BIO_set_mem_eof_return(inboundBio_, -1);
    BIO_set_mem_eof_return(outboundBio_, -1);
    SSL_set_bio(ssl_, inboundBio_, outboundBio_);
    SSL_set_ex_data(
        ssl_, openSslManagerBridgeExDataIndex(), managerBridge_.get());

    if (clientMode_) {
        SSL_set_connect_state(ssl_);
        const auto peerHost = explicitSniHost_ != nullptr
            ? explicitSniHost_ : getPeerHost();
        if (peerHost != nullptr && !peerHost->utf8().empty()) {
            if (SSL_set_tlsext_host_name(ssl_, peerHost->utf8().c_str()) != 1)
                throw ::jxx::ext::net::ssl::SSLProtocolException(
                    "could not configure SNI host");
            if (endpointIdentificationAlgorithm_ != nullptr &&
                endpointIdentificationAlgorithm_->utf8() == "HTTPS" &&
                SSL_set1_host(ssl_, peerHost->utf8().c_str()) != 1)
                throw ::jxx::ext::net::ssl::SSLProtocolException(
                    "could not configure HTTPS endpoint identification");
        }
    } else {
        SSL_set_accept_state(ssl_);
        int verifyMode = SSL_VERIFY_NONE;
        if (needClientAuth_)
            verifyMode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
        else if (wantClientAuth_)
            verifyMode = SSL_VERIFY_PEER;
        SSL_set_verify(ssl_, verifyMode, nullptr);
        if (managerBridge_->selectServerIdentity(ssl_) != 1)
            throw ::jxx::ext::net::ssl::SSLHandshakeException(
                "no usable server certificate identity");
    }
    initialized_ = true;
}

void OpenSslEngine::beginHandshake() {
    if (handshakeStarted_)
        throw ::jxx::lang::IllegalStateException(
            "handshake already started");
    ensureInitialized();
    handshakeStarted_ = true;
    driveHandshake();
}

void OpenSslEngine::driveHandshake() {
    if (!handshakeStarted_ || SSL_is_init_finished(ssl_)) return;
    const int operation = SSL_do_handshake(ssl_);
    if (operation == 1) {
        handshakeStatus_ = SSLEngineResult::HandshakeStatus::FINISHED;
        completeSession();
        return;
    }
    const int error = SSL_get_error(ssl_, operation);
    updateHandshakeStatus(error);
}

void OpenSslEngine::updateHandshakeStatus(int error) {
    if (BIO_ctrl_pending(outboundBio_) > 0) {
        handshakeStatus_ = SSLEngineResult::HandshakeStatus::NEED_WRAP;
        return;
    }
    if (error == SSL_ERROR_WANT_READ) {
        handshakeStatus_ = SSLEngineResult::HandshakeStatus::NEED_UNWRAP;
        return;
    }
    if (error == SSL_ERROR_WANT_WRITE) {
        handshakeStatus_ = SSLEngineResult::HandshakeStatus::NEED_WRAP;
        return;
    }
    throw ::jxx::ext::net::ssl::SSLHandshakeException(
        "OpenSSL handshake failed");
}

void OpenSslEngine::completeSession() {
    if (session_ != nullptr) return;
    if (!enableSessionCreation_ && !SSL_session_reused(ssl_))
        throw ::jxx::ext::net::ssl::SSLHandshakeException(
            "session creation is disabled");
    SSL_SESSION* nativeSession = SSL_get_session(ssl_);
    unsigned int idLength = 0;
    const unsigned char* id = nativeSession == nullptr
        ? nullptr
        : SSL_SESSION_get_id(nativeSession, &idLength);
    const auto sessionId = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(
            static_cast<::jxx::lang::jint>(idLength));
    for (unsigned int index = 0; index < idLength; ++index)
        (*sessionId)[static_cast<::jxx::lang::jint>(index)] =
            static_cast<::jxx::lang::jbyte>(id[index]);
    using CertificateArray = OpenSslSession::CertificateArray;
    ::jxx::Ptr<CertificateArray> peerCertificates;
    STACK_OF(X509)* chain = SSL_get_peer_cert_chain(ssl_);
    if (chain != nullptr) {
        const int count = sk_X509_num(chain);
        peerCertificates = ::jxx::NEW<CertificateArray>(count);
        for (int index = 0; index < count; ++index) {
            X509* certificate = sk_X509_value(chain, index);
            const int length = i2d_X509(certificate, nullptr);
            if (length <= 0) throw ::jxx::ext::net::ssl::SSLProtocolException(
                "could not encode peer certificate");
            const auto encoded = ::jxx::NEW<
                ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
            unsigned char* cursor = reinterpret_cast<unsigned char*>(&(*encoded)[0]);
            if (i2d_X509(certificate, &cursor) != length)
                throw ::jxx::ext::net::ssl::SSLProtocolException(
                    "could not encode peer certificate");
            (*peerCertificates)[index] = ::jxx::CAST<
                ::jxx::security::cert::Certificate>(
                    ::jxx::NEW<OpenSslX509Certificate>(encoded));
        }
    }
    ::jxx::Ptr<CertificateArray> localCertificates;
    X509* local = SSL_get_certificate(ssl_);
    if (local != nullptr) {
        const int length = i2d_X509(local, nullptr);
        const auto encoded = ::jxx::NEW<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length);
        unsigned char* cursor = reinterpret_cast<unsigned char*>(&(*encoded)[0]);
        if (length <= 0 || i2d_X509(local, &cursor) != length)
            throw ::jxx::ext::net::ssl::SSLProtocolException(
                "could not encode local certificate");
        localCertificates = ::jxx::NEW<CertificateArray>(1);
        (*localCertificates)[0] = ::jxx::CAST<
            ::jxx::security::cert::Certificate>(
                ::jxx::NEW<OpenSslX509Certificate>(encoded));
    }
    const auto context = clientMode_
        ? config_->clientSessionContext
        : config_->serverSessionContext;
    session_ = ::jxx::NEW<OpenSslSession>(
        ::jxx::NEW<::jxx::lang::String>(SSL_get_cipher_name(ssl_)),
        ::jxx::NEW<::jxx::lang::String>(SSL_get_version(ssl_)),
        getPeerHost(), getPeerPort(), peerCertificates,
        localCertificates, sessionId, context);
    if (context != nullptr) context->registerSession(session_);
}

::jxx::lang::jint OpenSslEngine::feedNetwork(
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source) {
    if (source == nullptr) throw ::jxx::lang::NullPointerException();
    const auto count = source->remaining();
    if (count == 0) return 0;
    std::vector<unsigned char> data(static_cast<std::size_t>(count));
    for (::jxx::lang::jint index = 0; index < count; ++index)
        data[static_cast<std::size_t>(index)] =
            static_cast<unsigned char>(source->get());
    const int written = BIO_write(inboundBio_, data.data(), count);
    if (written < 0)
        throw ::jxx::ext::net::ssl::SSLProtocolException(
            "could not feed encrypted input");
    return written;
}

::jxx::lang::jint OpenSslEngine::drainNetwork(
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) {
    if (destination == nullptr) throw ::jxx::lang::NullPointerException();
    const auto pending = static_cast<::jxx::lang::jint>(
        BIO_ctrl_pending(outboundBio_));
    if (pending == 0) return 0;
    if (destination->remaining() < pending) return -1;
    std::vector<unsigned char> data(static_cast<std::size_t>(pending));
    const int read = BIO_read(outboundBio_, data.data(), pending);
    if (read < 0) return 0;
    for (int index = 0; index < read; ++index)
        destination->put(static_cast<::jxx::lang::jbyte>(data[index]));
    return read;
}

::jxx::Ptr<SSLEngineResult> OpenSslEngine::wrap(
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) {
    if (source == nullptr || destination == nullptr)
        throw ::jxx::lang::NullPointerException();
    ensureInitialized();
    if (!handshakeStarted_) beginHandshake();

    const auto pending = static_cast<::jxx::lang::jint>(
        BIO_ctrl_pending(outboundBio_));
    if (pending > destination->remaining())
        return result(SSLEngineResult::Status::BUFFER_OVERFLOW,
                      handshakeStatus_, 0, 0);

    ::jxx::lang::jint consumed = 0;
    if (SSL_is_init_finished(ssl_) && source->remaining() > 0) {
        const auto count = source->remaining();
        std::vector<unsigned char> plain(static_cast<std::size_t>(count));
        for (::jxx::lang::jint index = 0; index < count; ++index)
            plain[static_cast<std::size_t>(index)] =
                static_cast<unsigned char>(source->get(index + source->position()));
        const int written = SSL_write(ssl_, plain.data(), count);
        if (written > 0) {
            for (int index = 0; index < written; ++index) source->get();
            consumed = written;
        } else {
            updateHandshakeStatus(SSL_get_error(ssl_, written));
        }
    } else {
        driveHandshake();
    }

    const auto produced = drainNetwork(destination);
    if (produced < 0)
        return result(SSLEngineResult::Status::BUFFER_OVERFLOW,
                      handshakeStatus_, 0, 0);
    const auto status = outboundDone_
        ? SSLEngineResult::Status::CLOSED
        : SSLEngineResult::Status::OK;
    auto reported = handshakeStatus_;
    if (reported == SSLEngineResult::HandshakeStatus::FINISHED) {
        handshakeFinishedReported_ = true;
    } else if (handshakeFinishedReported_) {
        reported = SSLEngineResult::HandshakeStatus::NOT_HANDSHAKING;
    }
    return result(status, reported, consumed, produced);
}

::jxx::Ptr<SSLEngineResult> OpenSslEngine::unwrap(
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) {
    if (source == nullptr || destination == nullptr)
        throw ::jxx::lang::NullPointerException();
    ensureInitialized();
    if (!handshakeStarted_) beginHandshake();
    const auto initialPosition = source->position();
    const auto consumed = feedNetwork(source);
    if (!SSL_is_init_finished(ssl_)) driveHandshake();

    ::jxx::lang::jint produced = 0;
    if (SSL_is_init_finished(ssl_)) {
        if (destination->remaining() == 0) {
            source->position(initialPosition);
            return result(SSLEngineResult::Status::BUFFER_OVERFLOW,
                          handshakeStatus_, 0, 0);
        }
        std::vector<unsigned char> plain(
            static_cast<std::size_t>(destination->remaining()));
        const int read = SSL_read(
            ssl_, plain.data(), static_cast<int>(plain.size()));
        if (read > 0) {
            for (int index = 0; index < read; ++index)
                destination->put(
                    static_cast<::jxx::lang::jbyte>(plain[index]));
            produced = read;
        } else {
            const int error = SSL_get_error(ssl_, read);
            if (error == SSL_ERROR_ZERO_RETURN) inboundDone_ = true;
            else if (error != SSL_ERROR_WANT_READ)
                throw ::jxx::ext::net::ssl::SSLProtocolException(
                    "OpenSSL unwrap failed");
        }
    }

    if (consumed == 0 && produced == 0 &&
        handshakeStatus_ == SSLEngineResult::HandshakeStatus::NEED_UNWRAP)
        return result(SSLEngineResult::Status::BUFFER_UNDERFLOW,
                      handshakeStatus_, 0, 0);
    return result(inboundDone_ ? SSLEngineResult::Status::CLOSED
                               : SSLEngineResult::Status::OK,
                  handshakeStatus_, consumed, produced);
}

::jxx::Ptr<SSLEngineResult> OpenSslEngine::wrap(
    const ::jxx::Ptr<ByteBufferArray>& sources,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length,
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) {
    if (sources == nullptr || offset < 0 || length < 0 ||
        offset > sources->length - length)
        throw ::jxx::lang::IllegalArgumentException();
    for (::jxx::lang::jint index = offset; index < offset + length; ++index) {
        const auto source = (*sources)[index];
        if (source == nullptr) throw ::jxx::lang::NullPointerException();
        if (source->remaining() > 0) return wrap(source, destination);
    }
    const auto empty = ::jxx::nio::ByteBuffer::allocate(0);
    return wrap(empty, destination);
}

::jxx::Ptr<SSLEngineResult> OpenSslEngine::unwrap(
    const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
    const ::jxx::Ptr<ByteBufferArray>& destinations,
    ::jxx::lang::jint offset,
    ::jxx::lang::jint length) {
    if (destinations == nullptr || offset < 0 || length < 0 ||
        offset > destinations->length - length)
        throw ::jxx::lang::IllegalArgumentException();
    for (::jxx::lang::jint index = offset; index < offset + length; ++index) {
        const auto destination = (*destinations)[index];
        if (destination == nullptr) throw ::jxx::lang::NullPointerException();
        if (destination->remaining() > 0) return unwrap(source, destination);
    }
    return result(SSLEngineResult::Status::BUFFER_OVERFLOW,
                  handshakeStatus_, 0, 0);
}

void OpenSslEngine::closeOutbound() {
    ensureInitialized();
    if (outboundDone_) return;
    SSL_shutdown(ssl_);
    outboundDone_ = true;
    handshakeStatus_ = BIO_ctrl_pending(outboundBio_) > 0
        ? SSLEngineResult::HandshakeStatus::NEED_WRAP
        : SSLEngineResult::HandshakeStatus::NOT_HANDSHAKING;
}

void OpenSslEngine::closeInbound() {
    if (!inboundDone_ && initialized_ && SSL_get_shutdown(ssl_) == 0)
        throw ::jxx::ext::net::ssl::SSLProtocolException(
            "inbound closed before peer close_notify");
    inboundDone_ = true;
}

::jxx::Ptr<::jxx::lang::Runnable> OpenSslEngine::getDelegatedTask() {
    return nullptr;
}

::jxx::Ptr<OpenSslEngine::StringArray>
OpenSslEngine::toArray(const std::vector<std::string>& values) {
    const auto result = ::jxx::NEW<StringArray>(
        static_cast<::jxx::lang::jint>(values.size()));
    for (std::size_t index = 0; index < values.size(); ++index)
        (*result)[static_cast<::jxx::lang::jint>(index)] =
            ::jxx::NEW<::jxx::lang::String>(values[index]);
    return result;
}

std::vector<std::string> OpenSslEngine::toVector(
    const ::jxx::Ptr<StringArray>& values) {
    if (values == nullptr) throw ::jxx::lang::IllegalArgumentException();
    std::vector<std::string> result;
    for (::jxx::lang::jint index = 0; index < values->length; ++index) {
        if ((*values)[index] == nullptr)
            throw ::jxx::lang::IllegalArgumentException();
        result.push_back((*values)[index]->utf8());
    }
    return result;
}

::jxx::Ptr<OpenSslEngine::StringArray>
OpenSslEngine::getSupportedProtocols() const {
    return toArray({"TLSv1.2", "TLSv1.3"});
}

::jxx::Ptr<OpenSslEngine::StringArray>
OpenSslEngine::getEnabledProtocols() const {
    return enabledProtocols_.empty()
        ? getSupportedProtocols()
        : toArray(enabledProtocols_);
}

void OpenSslEngine::setEnabledProtocols(
    const ::jxx::Ptr<StringArray>& protocols) {
    if (handshakeStarted_) throw ::jxx::lang::IllegalStateException();
    enabledProtocols_ = toVector(protocols);
}

::jxx::Ptr<OpenSslEngine::StringArray>
OpenSslEngine::getSupportedCipherSuites() const {
    return toArray({
        "TLS_AES_128_GCM_SHA256",
        "TLS_AES_256_GCM_SHA384",
        "TLS_CHACHA20_POLY1305_SHA256"});
}

::jxx::Ptr<OpenSslEngine::StringArray>
OpenSslEngine::getEnabledCipherSuites() const {
    return enabledCipherSuites_.empty()
        ? getSupportedCipherSuites()
        : toArray(enabledCipherSuites_);
}

void OpenSslEngine::setEnabledCipherSuites(
    const ::jxx::Ptr<StringArray>& suites) {
    if (handshakeStarted_) throw ::jxx::lang::IllegalStateException();
    enabledCipherSuites_ = toVector(suites);
}

SSLEngineResult::HandshakeStatus
OpenSslEngine::getHandshakeStatus() const {
    return handshakeStatus_;
}

::jxx::Ptr<SSLParameters> OpenSslEngine::getSSLParameters() const {
    const auto parameters = SSLEngine::getSSLParameters();
    parameters->setEndpointIdentificationAlgorithm(
        endpointIdentificationAlgorithm_);
    if (explicitSniHost_ != nullptr) {
        const auto names = ::jxx::NEW<
            ::jxx::util::ArrayList<::jxx::ext::net::ssl::SNIServerName>>();
        names->add(::jxx::NEW<::jxx::ext::net::ssl::SNIHostName>(
            explicitSniHost_));
        parameters->setServerNames(names);
    }
    return parameters;
}

void OpenSslEngine::setSSLParameters(
    const ::jxx::Ptr<SSLParameters>& parameters) {
    if (handshakeStarted_) throw ::jxx::lang::IllegalStateException();
    SSLEngine::setSSLParameters(parameters);
    endpointIdentificationAlgorithm_ =
        parameters->getEndpointIdentificationAlgorithm();
    if (endpointIdentificationAlgorithm_ != nullptr &&
        !endpointIdentificationAlgorithm_->utf8().empty() &&
        endpointIdentificationAlgorithm_->utf8() != "HTTPS")
        throw ::jxx::lang::IllegalArgumentException(
            "unsupported endpoint identification algorithm");
    explicitSniHost_ = nullptr;
    const auto names = parameters->getServerNames();
    if (names != nullptr) {
        for (::jxx::lang::jint index = 0; index < names->size(); ++index) {
            const auto host = ::jxx::CAST<::jxx::ext::net::ssl::SNIHostName>(
                names->get(index));
            if (host != nullptr) {
                explicitSniHost_ = host->getAsciiName();
                break;
            }
        }
    }
}

::jxx::Ptr<SSLSession> OpenSslEngine::getSession() const {
    return session_;
}

void OpenSslEngine::setUseClientMode(::jxx::lang::jbool mode) {
    if (handshakeStarted_) throw ::jxx::lang::IllegalStateException();
    clientMode_ = mode;
}

::jxx::lang::jbool OpenSslEngine::getUseClientMode() const {
    return clientMode_;
}

void OpenSslEngine::setNeedClientAuth(::jxx::lang::jbool need) {
    if (handshakeStarted_) throw ::jxx::lang::IllegalStateException();
    needClientAuth_ = need;
    if (need) wantClientAuth_ = false;
}

::jxx::lang::jbool OpenSslEngine::getNeedClientAuth() const {
    return needClientAuth_;
}

void OpenSslEngine::setWantClientAuth(::jxx::lang::jbool want) {
    if (handshakeStarted_) throw ::jxx::lang::IllegalStateException();
    wantClientAuth_ = want;
    if (want) needClientAuth_ = false;
}

::jxx::lang::jbool OpenSslEngine::getWantClientAuth() const {
    return wantClientAuth_;
}

void OpenSslEngine::setEnableSessionCreation(::jxx::lang::jbool enabled) {
    if (handshakeStarted_) throw ::jxx::lang::IllegalStateException();
    enableSessionCreation_ = enabled;
}

::jxx::lang::jbool OpenSslEngine::getEnableSessionCreation() const {
    return enableSessionCreation_;
}

::jxx::lang::jbool OpenSslEngine::isInboundDone() const {
    return inboundDone_;
}

::jxx::lang::jbool OpenSslEngine::isOutboundDone() const {
    return outboundDone_ &&
        (outboundBio_ == nullptr || BIO_ctrl_pending(outboundBio_) == 0);
}

} // namespace jxx::ext::net::ssl::internal
