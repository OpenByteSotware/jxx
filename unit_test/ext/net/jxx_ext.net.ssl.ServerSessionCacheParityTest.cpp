#include <gtest/gtest.h>
#include <openssl/ssl.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSessionCache.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"

TEST(ServerSessionCacheParityTest, ConfiguresServerCacheCallbacks) {
    SSL_CTX* nativeContext = SSL_CTX_new(TLS_server_method());
    ASSERT_NE(nativeContext, nullptr);
    const auto context = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::OpenSslSessionContext>();
    ::jxx::ext::net::ssl::internal::configureServerSessionCache(
        nativeContext, context.get());
    EXPECT_NE(SSL_CTX_sess_get_new_cb(nativeContext), nullptr);
    EXPECT_NE(SSL_CTX_sess_get_remove_cb(nativeContext), nullptr);
    EXPECT_NE(SSL_CTX_sess_get_get_cb(nativeContext), nullptr);
    SSL_CTX_free(nativeContext);
}
