#pragma once

#include <string>
#include <vector>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

struct ssl_ctx_st;

namespace jxx::ext::net::ssl::internal {

using CipherSuiteArray =
    ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>,
        1U>;

void applyEnabledCipherSuites(
    ssl_ctx_st* context,
    const std::vector<std::string>& suites);

void validateEnabledCipherSuites(
    const std::vector<std::string>& suites);

::jxx::Ptr<CipherSuiteArray>
clientDefaultCipherSuites();

::jxx::Ptr<CipherSuiteArray>
clientSupportedCipherSuites();

::jxx::Ptr<CipherSuiteArray>
serverDefaultCipherSuites();

::jxx::Ptr<CipherSuiteArray>
serverSupportedCipherSuites();

} // namespace jxx::ext::net::ssl::internal
