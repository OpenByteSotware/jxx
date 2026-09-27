#pragma once

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::ext::net::ssl::internal {

using CipherSuiteArray =
    ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>,
        1U>;

::jxx::Ptr<CipherSuiteArray>
clientDefaultCipherSuites();

::jxx::Ptr<CipherSuiteArray>
clientSupportedCipherSuites();

::jxx::Ptr<CipherSuiteArray>
serverDefaultCipherSuites();

::jxx::Ptr<CipherSuiteArray>
serverSupportedCipherSuites();

} // namespace jxx::ext::net::ssl::internal
