#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketNative.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslManagerBridge.h"
#include "security/jxx.security.SecureRandom.h"
#include "lang/jxx.lang.NullPointerException.h"
#include <algorithm>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslStreams.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedListener.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
namespace jxx::ext::net::ssl::internal
{
    namespace {
        std::vector<std::string> toVector(const ::jxx::Ptr<OpenSslSocket::StringArray>& values) {
            if (values == nullptr) throw ::jxx::lang::IllegalArgumentException();
            std::vector<std::string> result;
            result.reserve(static_cast<std::size_t>(values->length));
            for (::jxx::lang::jint index = 0; index < values->length; ++index) {
                if ((*values)[index] == nullptr) throw ::jxx::lang::IllegalArgumentException();
                result.push_back((*values)[index]->utf8());
            }
            return result;
        }

        ::jxx::Ptr<OpenSslSocket::StringArray> toArray(const std::vector<std::string>& values) {
            const auto result = ::jxx::NEW<OpenSslSocket::StringArray>(
                static_cast<::jxx::lang::jint>(values.size()));
            for (std::size_t index = 0; index < values.size(); ++index)
                (*result)[static_cast<::jxx::lang::jint>(index)] =
                    ::jxx::NEW<::jxx::lang::String>(values[index]);
            return result;
        }

        std::string join(const std::vector<std::string>& values, const char delimiter) {
            std::string result;
            for (const auto& value : values) {
                if (!result.empty()) result.push_back(delimiter);
                result += value;
            }
            return result;
        }
    }

	OpenSslSocket::OpenSslSocket(const ::jxx::Ptr<::jxx::lang::String>& h, ::jxx::lang::jint p, const std::shared_ptr<OpenSslContextConfig>& config) :config_(config), native_(new OpenSslSocketNative()), host_(h), port_(p)
	{
	}OpenSslSocket::~OpenSslSocket() = default; void OpenSslSocket::startHandshake()
	{
		if (session_ != nullptr)return; native_->context = SSL_CTX_new(TLS_client_method());
        if (native_->context == nullptr) throw ::jxx::io::IOException("SSL_CTX_new failed");

        int minimumVersion = TLS1_2_VERSION;
        int maximumVersion = TLS1_3_VERSION;
        if (!enabledProtocols_.empty()) {
            bool tls12 = false;
            bool tls13 = false;
            for (const auto& protocol : enabledProtocols_) {
                if (protocol == "TLSv1.2") tls12 = true;
                else if (protocol == "TLSv1.3") tls13 = true;
                else throw ::jxx::lang::IllegalArgumentException();
            }
            if (!tls12 && !tls13) throw ::jxx::lang::IllegalArgumentException();
            minimumVersion = tls12 ? TLS1_2_VERSION : TLS1_3_VERSION;
            maximumVersion = tls13 ? TLS1_3_VERSION : TLS1_2_VERSION;
        }
        if (SSL_CTX_set_min_proto_version(native_->context, minimumVersion) != 1 ||
            SSL_CTX_set_max_proto_version(native_->context, maximumVersion) != 1)
            throw ::jxx::io::IOException("Could not apply enabled TLS protocols");

        if (!enabledCipherSuites_.empty()) {
            std::vector<std::string> tls13Suites;
            std::vector<std::string> legacySuites;
            for (const auto& cipher : enabledCipherSuites_) {
                if (cipher.rfind("TLS_", 0) == 0) tls13Suites.push_back(cipher);
                else legacySuites.push_back(cipher);
            }
            if (!legacySuites.empty() &&
                SSL_CTX_set_cipher_list(native_->context, join(legacySuites, ':').c_str()) != 1)
                throw ::jxx::lang::IllegalArgumentException();
            if (!tls13Suites.empty() &&
                SSL_CTX_set_ciphersuites(native_->context, join(tls13Suites, ':').c_str()) != 1)
                throw ::jxx::lang::IllegalArgumentException();
        }

        if (config_ != nullptr && config_->secureRandom != nullptr) {
            const auto seed = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(64);
            config_->secureRandom->nextBytes(seed);
            RAND_seed(&(*seed)[0], seed->length);
        }
		SSL_CTX_set_min_proto_version(native_->context, TLS1_2_VERSION); 
		SSL_CTX_set_max_proto_version(native_->context, TLS1_3_VERSION); 
		SSL_CTX_set_verify(native_->context, SSL_VERIFY_PEER, nullptr);
		if (SSL_CTX_set_default_verify_paths(native_->context) != 1)
            throw ::jxx::io::IOException("Could not load default trust paths");

        native_->managerBridge =
            std::make_unique<OpenSslManagerBridge>(config_);
        SSL_CTX_set_cert_verify_callback(
            native_->context,
            openSslVerifyCallback,
            native_->managerBridge.get());
        SSL_CTX_set_client_cert_cb(
            native_->context,
            openSslClientCertificateCallback); 
		native_->connection = BIO_new_ssl_connect(native_->context); 
		std::string endpoint = host_->utf8() + ":" + std::to_string(port_); 
		BIO_set_conn_hostname(native_->connection, endpoint.c_str());
		SSL* ssl = nullptr; BIO_get_ssl(native_->connection, &ssl);
        SSL_set_ex_data(ssl, openSslManagerBridgeExDataIndex(), native_->managerBridge.get());
		SSL_set_tlsext_host_name(ssl, host_->utf8().c_str());
		SSL_set1_host(ssl, host_->utf8().c_str()); 
		if (BIO_do_connect(native_->connection) <= 0 ||
			BIO_do_handshake(native_->connection) <= 0)throw ::jxx::io::IOException("TLS handshake failed"); session_ = ::jxx::NEW<OpenSslSession>(::jxx::NEW<::jxx::lang::String>(SSL_get_cipher_name(ssl)), ::jxx::NEW<::jxx::lang::String>(SSL_get_version(ssl)), host_, port_); const auto socket = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(
			   this->thisPtr());
		if (socket == nullptr) {
			throw ::jxx::lang::IllegalStateException(
				::jxx::NEW<::jxx::lang::String>(
					"OpenSslSocket has no JXX-managed self reference"));
		}
		const auto event = ::jxx::NEW<HandshakeCompletedEvent>(
			socket,
			session_);
		for (const auto& listener : listeners_) {
			listener->handshakeCompleted(event);
		}
	}::jxx::Ptr<::jxx::io::InputStream> OpenSslSocket::getInputStream()
	{
		startHandshake(); return ::jxx::NEW<OpenSslInputStream>(this);
	}::jxx::Ptr<::jxx::io::OutputStream> OpenSslSocket::getOutputStream()
	{
		startHandshake(); return ::jxx::NEW<OpenSslOutputStream>(this);
	}void OpenSslSocket::close()
	{
		native_.reset(new OpenSslSocketNative()); session_ = nullptr;
	}int OpenSslSocket::tlsRead(unsigned char* b, int n)
	{
		startHandshake(); int r = BIO_read(native_->connection, b, n); return r <= 0 ? -1 : r;
	}int OpenSslSocket::tlsWrite(const unsigned char* b, int n)
	{
		startHandshake(); int r = BIO_write(native_->connection, b, n); if (r <= 0)throw ::jxx::io::IOException("TLS write failed"); return r;
	}::jxx::Ptr<OpenSslSocket::StringArray> OpenSslSocket::getSupportedProtocols()const
	{
		auto a = ::jxx::NEW<StringArray>(2); (*a)[0] = ::jxx::NEW<::jxx::lang::String>("TLSv1.2"); (*a)[1] = ::jxx::NEW<::jxx::lang::String>("TLSv1.3"); return a;
	}::jxx::Ptr<OpenSslSocket::StringArray> OpenSslSocket::getEnabledProtocols()const
	{
		return enabledProtocols_.empty() ? getSupportedProtocols() : toArray(enabledProtocols_);
	}void OpenSslSocket::setEnabledProtocols(const ::jxx::Ptr<StringArray>& values)
	{
        if (session_ != nullptr) throw ::jxx::lang::IllegalStateException();
        enabledProtocols_ = toVector(values);
	}::jxx::Ptr<OpenSslSocket::StringArray> OpenSslSocket::getSupportedCipherSuites()const
	{
		return ::jxx::NEW<StringArray>(0);
	}::jxx::Ptr<OpenSslSocket::StringArray> OpenSslSocket::getEnabledCipherSuites()const
	{
		return enabledCipherSuites_.empty() ? getSupportedCipherSuites() : toArray(enabledCipherSuites_);
	}void OpenSslSocket::setEnabledCipherSuites(const ::jxx::Ptr<StringArray>& values)
	{
        if (session_ != nullptr) throw ::jxx::lang::IllegalStateException();
        enabledCipherSuites_ = toVector(values);
	}::jxx::Ptr<SSLSession> OpenSslSocket::getSession()
	{
		startHandshake(); return session_;
	}void OpenSslSocket::addHandshakeCompletedListener(const ::jxx::Ptr<HandshakeCompletedListener>& l)
	{
		if (l == nullptr)throw ::jxx::lang::IllegalArgumentException(); listeners_.push_back(l);
	}void OpenSslSocket::removeHandshakeCompletedListener(const ::jxx::Ptr<HandshakeCompletedListener>& l)
	{
		listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), l), listeners_.end());
	}void OpenSslSocket::setUseClientMode(::jxx::lang::jbool v)
	{
		client_ = v;
	}::jxx::lang::jbool OpenSslSocket::getUseClientMode()const
	{
		return client_;
	}void OpenSslSocket::setNeedClientAuth(::jxx::lang::jbool v)
	{
		need_ = v; if (v)want_ = false;
	}::jxx::lang::jbool OpenSslSocket::getNeedClientAuth()const
	{
		return need_;
	}void OpenSslSocket::setWantClientAuth(::jxx::lang::jbool v)
	{
		want_ = v; if (v)need_ = false;
	}::jxx::lang::jbool OpenSslSocket::getWantClientAuth()const
	{
		return want_;
	}void OpenSslSocket::setEnableSessionCreation(::jxx::lang::jbool v)
	{
		create_ = v;
	}::jxx::lang::jbool OpenSslSocket::getEnableSessionCreation()const
	{
		return create_;
	}
}
