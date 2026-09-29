#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"

namespace jxx::ext::net::ssl::internal {

::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::KeyManagerArray>
loadDefaultPropertyKeyManagers();

} // namespace jxx::ext::net::ssl::internal
