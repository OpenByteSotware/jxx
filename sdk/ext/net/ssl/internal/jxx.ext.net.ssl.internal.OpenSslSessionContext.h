#pragma once

#include <deque>
#include <mutex>
#include <string>
#include <unordered_map>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSessionContext.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslSessionContext final
    : public ::jxx::lang::ClassBase<
          OpenSslSessionContext,
          ::jxx::lang::Object,
          ::jxx::ext::net::ssl::SSLSessionContext> {
public:
    OpenSslSessionContext();
    ~OpenSslSessionContext() override = default;

    ::jxx::Ptr<::jxx::util::Enumeration<IdArray>> getIds() override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession> getSession(
        const ::jxx::lang::ByteArray& sessionId) override;
    ::jxx::lang::jint getSessionTimeout() const override;
    void setSessionTimeout(::jxx::lang::jint seconds) override;
    ::jxx::lang::jint getSessionCacheSize() const override;
    void setSessionCacheSize(::jxx::lang::jint size) override;

    void registerSession(
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>& session);
    void removeSession(const ::jxx::lang::ByteArray& sessionId);

private:
    static std::string keyOf(const ::jxx::lang::ByteArray& sessionId);
    static ::jxx::lang::ByteArray copyId(
        const ::jxx::lang::ByteArray& sessionId);
    void purgeExpiredLocked();
    void enforceCacheSizeLocked();

    mutable std::mutex mutex_;
    std::unordered_map<
        std::string,
        ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>> sessions_;
    std::deque<std::string> insertionOrder_;
    ::jxx::lang::jint timeoutSeconds_ = 86400;
    ::jxx::lang::jint cacheSize_ = 0;
};

} // namespace jxx::ext::net::ssl::internal
