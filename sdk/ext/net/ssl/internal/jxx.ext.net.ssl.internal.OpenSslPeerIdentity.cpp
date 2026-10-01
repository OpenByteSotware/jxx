#include <array>
#include <memory>
#include <string>
#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPeerIdentity.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCompatibility.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLProtocolException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::ext::net::ssl::internal
{
	namespace
	{
		bool isIpLiteral(const std::string& value)
		{
			std::array<unsigned char, 16U> address{}; return inet_pton(AF_INET, value.c_str(), address.data()) == 1 || inet_pton(AF_INET6, value.c_str(), address.data()) == 1;
		}
		bool sameCertificate(X509* left, X509* right)
		{
			return left != nullptr && right != nullptr && X509_cmp(left, right) == 0;
		}
		::jxx::Ptr<::jxx::security::cert::Certificate> convertCertificate(X509* certificate)
		{
			if (certificate == nullptr)throw ::jxx::ext::net::ssl::SSLProtocolException("peer certificate is null"); const int length = i2d_X509(certificate, nullptr); if (length <= 0)throw ::jxx::ext::net::ssl::SSLProtocolException("could not encode peer certificate"); const auto encoded = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(length); unsigned char* cursor = reinterpret_cast<unsigned char*>(&(*encoded)[0]); if (i2d_X509(certificate, &cursor) != length)throw ::jxx::ext::net::ssl::SSLProtocolException("could not encode peer certificate"); return ::jxx::CAST<::jxx::security::cert::Certificate>(::jxx::NEW<OpenSslX509Certificate>(encoded));
		}
	}
	void configureHttpsEndpointIdentification(SSL* ssl, const ::jxx::Ptr<::jxx::lang::String>& peerHost)
	{
		if (ssl == nullptr || peerHost == nullptr || peerHost->utf8().empty())throw ::jxx::lang::IllegalArgumentException("HTTPS endpoint identification requires a peer host"); const auto host = peerHost->utf8(); X509_VERIFY_PARAM* parameters = SSL_get0_param(ssl); if (parameters == nullptr)throw ::jxx::ext::net::ssl::SSLProtocolException("could not obtain endpoint verification parameters"); X509_VERIFY_PARAM_set_hostflags(parameters, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS); const int configured = isIpLiteral(host) ? X509_VERIFY_PARAM_set1_ip_asc(parameters, host.c_str()) : X509_VERIFY_PARAM_set1_host(parameters, host.c_str(), host.size()); if (configured != 1)throw ::jxx::ext::net::ssl::SSLProtocolException("could not configure HTTPS endpoint identification");
	}
	::jxx::Ptr<OpenSslSession::CertificateArray> peerCertificateChain(SSL* ssl)
	{
		if (ssl == nullptr)return nullptr; X509* leaf = retainedPeerCertificate(ssl); if (leaf == nullptr)return nullptr; const std::shared_ptr<X509> leafGuard(leaf, X509_free); STACK_OF(X509)* presented = SSL_get_peer_cert_chain(ssl); const int presentedCount = presented == nullptr ? 0 : sk_X509_num(presented); int count = 1; for (int index = 0; index < presentedCount; ++index)if (!sameCertificate(leaf, sk_X509_value(presented, index)))++count; const auto result = ::jxx::NEW<OpenSslSession::CertificateArray>(count); (*result)[0] = convertCertificate(leaf); int output = 1; for (int index = 0; index < presentedCount; ++index) {
			X509* certificate = sk_X509_value(presented, index); if (sameCertificate(leaf, certificate))continue; (*result)[output++] = convertCertificate(certificate);
		}return result;
	}
}
