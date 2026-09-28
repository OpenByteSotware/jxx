#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslProtocolPolicy.h"

#include <openssl/ssl.h>

namespace jxx::ext::net::ssl::internal {

std::pair<int, int> protocolRange(
    const ::jxx::Ptr<::jxx::lang::String>& protocol) {
    const auto value = protocol == nullptr
        ? std::string("TLS")
        : protocol->utf8();

    if (value == "TLSv1.2")
        return {TLS1_2_VERSION, TLS1_2_VERSION};
    if (value == "TLSv1.3")
        return {TLS1_3_VERSION, TLS1_3_VERSION};

    return {TLS1_2_VERSION, TLS1_3_VERSION};
}

::jxx::Ptr<ProtocolArray> contextProtocols(
    const ::jxx::Ptr<::jxx::lang::String>& protocol) {
    const auto range = protocolRange(protocol);
    const bool has12 = range.first <= TLS1_2_VERSION &&
        range.second >= TLS1_2_VERSION;
    const bool has13 = range.first <= TLS1_3_VERSION &&
        range.second >= TLS1_3_VERSION;
    const auto result = ::jxx::NEW<ProtocolArray>(
        static_cast<::jxx::lang::jint>(has12 + has13));
    ::jxx::lang::jint index = 0;
    if (has12) (*result)[index++] =
        ::jxx::NEW<::jxx::lang::String>("TLSv1.2");
    if (has13) (*result)[index] =
        ::jxx::NEW<::jxx::lang::String>("TLSv1.3");
    return result;
}

} // namespace jxx::ext::net::ssl::internal
