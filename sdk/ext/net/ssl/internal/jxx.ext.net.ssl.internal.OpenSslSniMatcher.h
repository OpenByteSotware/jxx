#pragma once
#include <openssl/ssl.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SNIMatcher.h"
#include "util/jxx.util.List.h"
namespace jxx::ext::net::ssl::internal {
using SniMatcherList = ::jxx::util::List<::jxx::ext::net::ssl::SNIMatcher>;
int openSslServerNameMatcherCallback(SSL* ssl,int* alert,void* argument) noexcept;
void configureServerNameMatchers(SSL_CTX* context,const ::jxx::Ptr<SniMatcherList>& matchers);
} // namespace jxx::ext::net::ssl::internal
