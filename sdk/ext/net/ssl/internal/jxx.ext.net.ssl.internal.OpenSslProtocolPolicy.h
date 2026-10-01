#pragma once

#include <string>
#include <utility>
#include <vector>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

struct ssl_ctx_st;

namespace jxx::ext::net::ssl::internal {

using ProtocolArray = ::jxx::lang::JxxArray<
    ::jxx::Ptr<::jxx::lang::String>, 1U>;

int protocolVersion(const std::string& protocol);
std::pair<int, int> protocolRange(
    const ::jxx::Ptr<::jxx::lang::String>& protocol);
std::pair<int, int> enabledProtocolRange(
    const std::vector<std::string>& protocols);
std::vector<std::string> contextProtocolNames(
    const ::jxx::Ptr<::jxx::lang::String>& protocol);
std::vector<std::string> supportedProtocolNames();
void applyEnabledProtocols(
    ssl_ctx_st* context,
    const std::vector<std::string>& protocols);

::jxx::Ptr<ProtocolArray> contextProtocols(
    const ::jxx::Ptr<::jxx::lang::String>& protocol);

} // namespace jxx::ext::net::ssl::internal
