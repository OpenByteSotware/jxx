#include <algorithm>
#include <openssl/ssl.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslProtocolPolicy.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCompatibility.h"


#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl::internal {

int protocolVersion(const std::string& protocol) {
    if (protocol == "TLSv1") return TLS1_VERSION;
    if (protocol == "TLSv1.1") return TLS1_1_VERSION;
    if (protocol == "TLSv1.2") return TLS1_2_VERSION;
    if (protocol == "TLSv1.3") {
#if defined(TLS1_3_VERSION) && OPENSSL_VERSION_NUMBER >= 0x10101000L && !defined(LIBRESSL_VERSION_NUMBER)
        return TLS1_3_VERSION;
#else
        throw ::jxx::lang::IllegalArgumentException(
            "TLSv1.3 is not supported by this OpenSSL build");
#endif
    }
    throw ::jxx::lang::IllegalArgumentException("unsupported TLS protocol");
}

std::vector<std::string> supportedProtocolNames() {
    std::vector<std::string> result{
        "TLSv1", "TLSv1.1", "TLSv1.2"};
    if (hasTls13Api()) result.push_back("TLSv1.3");
    return result;
}

std::vector<std::string> contextProtocolNames(
    const ::jxx::Ptr<::jxx::lang::String>& protocol) {
    const auto values = contextProtocols(protocol);
    std::vector<std::string> result;
    result.reserve(static_cast<std::size_t>(values->length));
    for (::jxx::lang::jint index = 0; index < values->length; ++index)
        result.push_back((*values)[index]->utf8());
    return result;
}

std::pair<int, int> enabledProtocolRange(
    const std::vector<std::string>& protocols) {
    if (protocols.empty())
        throw ::jxx::lang::IllegalArgumentException("enabled protocols are empty");
    int minimum = protocolVersion(protocols.front());
    int maximum = minimum;
    for (const auto& protocol : protocols) {
        const int version = protocolVersion(protocol);
        minimum = std::min(minimum, version);
        maximum = std::max(maximum, version);
    }
    return {minimum, maximum};
}


void applyEnabledProtocols(
    ssl_ctx_st* context,
    const std::vector<std::string>& protocols) {
    if (context == nullptr)
        throw ::jxx::lang::IllegalArgumentException("TLS context is null");

    const auto range = enabledProtocolRange(protocols);
    if (SSL_CTX_set_min_proto_version(context, range.first) != 1 ||
        SSL_CTX_set_max_proto_version(context, range.second) != 1)
        throw ::jxx::lang::IllegalArgumentException(
            "could not configure enabled TLS protocols");

    const auto contains = [&protocols](const char* value) {
        return std::find(protocols.begin(), protocols.end(),
                         std::string(value)) != protocols.end();
    };
    unsigned long disabled = 0UL;
#ifdef SSL_OP_NO_TLSv1
    if (!contains("TLSv1")) disabled |= SSL_OP_NO_TLSv1;
#endif
#ifdef SSL_OP_NO_TLSv1_1
    if (!contains("TLSv1.1")) disabled |= SSL_OP_NO_TLSv1_1;
#endif
#ifdef SSL_OP_NO_TLSv1_2
    if (!contains("TLSv1.2")) disabled |= SSL_OP_NO_TLSv1_2;
#endif
#ifdef SSL_OP_NO_TLSv1_3
    if (!contains("TLSv1.3")) disabled |= SSL_OP_NO_TLSv1_3;
#endif
    if (disabled != 0UL) SSL_CTX_set_options(context, disabled);
}

std::pair<int, int> protocolRange(
    const ::jxx::Ptr<::jxx::lang::String>& protocol) {
    const auto value = protocol == nullptr
        ? std::string("TLS")
        : protocol->utf8();

    if (value == "TLSv1")
        return {TLS1_VERSION, TLS1_VERSION};
    if (value == "TLSv1.1")
        return {TLS1_1_VERSION, TLS1_1_VERSION};
    if (value == "TLSv1.2")
        return {TLS1_2_VERSION, TLS1_2_VERSION};
    if (value == "TLSv1.3")
        return {highestSupportedTlsVersion(), highestSupportedTlsVersion()};

    return {TLS1_2_VERSION, highestSupportedTlsVersion()};
}

::jxx::Ptr<ProtocolArray> contextProtocols(
    const ::jxx::Ptr<::jxx::lang::String>& protocol) {
    const auto range = protocolRange(protocol);
    const bool has10 = range.first <= TLS1_VERSION &&
        range.second >= TLS1_VERSION;
    const bool has11 = range.first <= TLS1_1_VERSION &&
        range.second >= TLS1_1_VERSION;
    const bool has12 = range.first <= TLS1_2_VERSION &&
        range.second >= TLS1_2_VERSION;
    const bool has13 = hasTls13Api() &&
        range.first <= highestSupportedTlsVersion() &&
        range.second >= highestSupportedTlsVersion() &&
        highestSupportedTlsVersion() != TLS1_2_VERSION;
    const auto result = ::jxx::NEW<ProtocolArray>(
        static_cast<::jxx::lang::jint>(has10 + has11 + has12 + has13));
    ::jxx::lang::jint index = 0;
    if (has10) (*result)[index++] =
        ::jxx::NEW<::jxx::lang::String>("TLSv1");
    if (has11) (*result)[index++] =
        ::jxx::NEW<::jxx::lang::String>("TLSv1.1");
    if (has12) (*result)[index++] =
        ::jxx::NEW<::jxx::lang::String>("TLSv1.2");
    if (has13) (*result)[index] =
        ::jxx::NEW<::jxx::lang::String>("TLSv1.3");
    return result;
}

} // namespace jxx::ext::net::ssl::internal
