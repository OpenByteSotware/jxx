#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSessionCache.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"
namespace jxx::ext::net::ssl::internal {
namespace {
int sessionContextIndex() {
    static const int index = SSL_CTX_get_ex_new_index(0, nullptr, nullptr, nullptr, nullptr);
    return index;
}
OpenSslSessionContext* contextOf(SSL_CTX* context) {
    return static_cast<OpenSslSessionContext*>(
        SSL_CTX_get_ex_data(context, sessionContextIndex()));
}
int newSession(SSL* ssl, SSL_SESSION* session) {
    OpenSslSessionContext* context = contextOf(SSL_get_SSL_CTX(ssl));
    if (context == nullptr) return 0;
    context->registerNativeSessionById(session);
    return 0;
}
void removeSession(SSL_CTX* sslContext, SSL_SESSION* session) {
    OpenSslSessionContext* context = contextOf(sslContext);
    if (context != nullptr) context->removeNativeSessionById(session);
}
SSL_SESSION* getSession(
    SSL* ssl,
    const unsigned char* id,
    int length,
    int* copy) {
    OpenSslSessionContext* context = contextOf(SSL_get_SSL_CTX(ssl));
    if (context == nullptr) return nullptr;
    SSL_SESSION* session = context->acquireNativeSessionById(id, length);
    if (session != nullptr && copy != nullptr) *copy = 0;
    return session;
}
} // namespace
void configureServerSessionCache(
    SSL_CTX* context,
    OpenSslSessionContext* sessionContext) {
    if (context == nullptr || sessionContext == nullptr) return;
    static const unsigned char sessionIdContext[] = {
        'J','X','X','-','S','S','L','-','S','E','R','V','E','R'};
    SSL_CTX_set_ex_data(context, sessionContextIndex(), sessionContext);
    SSL_CTX_set_session_cache_mode(
        context,
        SSL_SESS_CACHE_SERVER |
        SSL_SESS_CACHE_NO_INTERNAL_STORE |
        SSL_SESS_CACHE_NO_AUTO_CLEAR);
    SSL_CTX_set_session_id_context(
        context,
        sessionIdContext,
        static_cast<unsigned int>(sizeof(sessionIdContext)));
    SSL_CTX_sess_set_new_cb(context, newSession);
    SSL_CTX_sess_set_remove_cb(context, removeSession);
    SSL_CTX_sess_set_get_cb(context, getSession);
}
} // namespace jxx::ext::net::ssl::internal
