#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslProtocolPolicy.h"

#include <openssl/ssl.h>

#include <algorithm>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl::internal {

int protocolVersion(const std::string& protocol) {
    if (protocol == "TLSv1") return TLS1_VERSION;
    if (protocol == "TLSv1.1") return TLS1_1_VERSION;
    if (protocol == "TLSv1.2") return TLS1_2_VERSION;
    if (protocol == "TLSv1.3") return TLS1_3_VERSION;
    throw ::jxx::lang::IllegalArgumentException("unsupported TLS protocol");
}

std::vector<std::string> supportedProtocolNames() {
    return {"TLSv1", "TLSv1.1", "TLSv1.2", "TLSv1.3"};
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
        return {TLS1_3_VERSION, TLS1_3_VERSION};

    return {TLS1_2_VERSION, TLS1_3_VERSION};
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
    const bool has13 = range.first <= TLS1_3_VERSION &&
        range.second >= TLS1_3_VERSION;
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
