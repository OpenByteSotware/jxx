#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketNative.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslManagerBridge.h"
namespace jxx::ext::net::ssl::internal { OpenSslSocketNative::~OpenSslSocketNative(){if(connection!=nullptr)BIO_free_all(connection);if(context!=nullptr)SSL_CTX_free(context);} }
