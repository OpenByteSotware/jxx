#pragma once

#include <utility>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::ext::net::ssl::internal {

using ProtocolArray = ::jxx::lang::JxxArray<
    ::jxx::Ptr<::jxx::lang::String>, 1U>;

std::pair<int, int> protocolRange(
    const ::jxx::Ptr<::jxx::lang::String>& protocol);

::jxx::Ptr<ProtocolArray> contextProtocols(
    const ::jxx::Ptr<::jxx::lang::String>& protocol);

} // namespace jxx::ext::net::ssl::internal
