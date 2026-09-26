#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"

#include <algorithm>
#include <chrono>
#include <vector>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.VectorEnumeration.h"

namespace jxx::ext::net::ssl::internal {
namespace {

::jxx::lang::jlong nowMillis() {
    return static_cast<::jxx::lang::jlong>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
}

} // namespace

OpenSslSessionContext::OpenSslSessionContext() = default;

OpenSslSessionContext::~OpenSslSessionContext() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& entry : nativeSessions_)
        SSL_SESSION_free(entry.second);
    nativeSessions_.clear();
}

std::string OpenSslSessionContext::keyOf(
    const ::jxx::lang::ByteArray& sessionId) {
    if (sessionId == nullptr) throw ::jxx::lang::NullPointerException();
    if (sessionId->length == 0) return std::string();
    return std::string(
        reinterpret_cast<const char*>(&(*sessionId)[0]),
        static_cast<std::size_t>(sessionId->length));
}

::jxx::lang::ByteArray OpenSslSessionContext::copyId(
    const ::jxx::lang::ByteArray& sessionId) {
    const auto copy = ::jxx::NEW<IdArray>(sessionId->length);
    for (::jxx::lang::jint index = 0; index < sessionId->length; ++index)
        (*copy)[index] = (*sessionId)[index];
    return copy;
}

void OpenSslSessionContext::purgeExpiredLocked() {
    const auto current = nowMillis();
    for (auto iterator = sessions_.begin(); iterator != sessions_.end();) {
        const auto& session = iterator->second;
        const bool expired = session != nullptr && timeoutSeconds_ > 0 &&
            current - session->getCreationTime() >=
                static_cast<::jxx::lang::jlong>(timeoutSeconds_) * 1000;
        if (session == nullptr || !session->isValid() || expired) {
            if (session != nullptr && expired) session->invalidate();
            const auto key = iterator->first;
            iterator = sessions_.erase(iterator);
            insertionOrder_.erase(
                std::remove(insertionOrder_.begin(), insertionOrder_.end(), key),
                insertionOrder_.end());
        } else {
            ++iterator;
        }
    }
}

void OpenSslSessionContext::enforceCacheSizeLocked() {
    if (cacheSize_ == 0) return;
    while (sessions_.size() > static_cast<std::size_t>(cacheSize_)) {
        if (insertionOrder_.empty()) break;
        const auto key = insertionOrder_.front();
        insertionOrder_.pop_front();
        const auto iterator = sessions_.find(key);
        if (iterator != sessions_.end()) {
            iterator->second->invalidate();
            sessions_.erase(iterator);
        }
    }
}

::jxx::Ptr<::jxx::util::Enumeration<OpenSslSessionContext::IdArray>>
OpenSslSessionContext::getIds() {
    std::lock_guard<std::mutex> lock(mutex_);
    purgeExpiredLocked();
    std::vector<::jxx::Ptr<IdArray>> ids;
    ids.reserve(sessions_.size());
    for (const auto& item : sessions_)
        ids.push_back(copyId(item.second->getId()));
    return ::jxx::NEW<::jxx::util::VectorEnumeration<IdArray>>(
        std::move(ids));
}

::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>
OpenSslSessionContext::getSession(
    const ::jxx::lang::ByteArray& sessionId) {
    const auto key = keyOf(sessionId);
    std::lock_guard<std::mutex> lock(mutex_);
    purgeExpiredLocked();
    const auto iterator = sessions_.find(key);
    return iterator == sessions_.end() ? nullptr : iterator->second;
}

::jxx::lang::jint OpenSslSessionContext::getSessionTimeout() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return timeoutSeconds_;
}

void OpenSslSessionContext::setSessionTimeout(::jxx::lang::jint seconds) {
    if (seconds < 0) throw ::jxx::lang::IllegalArgumentException();
    std::lock_guard<std::mutex> lock(mutex_);
    timeoutSeconds_ = seconds;
    purgeExpiredLocked();
}

::jxx::lang::jint OpenSslSessionContext::getSessionCacheSize() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return cacheSize_;
}

void OpenSslSessionContext::setSessionCacheSize(::jxx::lang::jint size) {
    if (size < 0) throw ::jxx::lang::IllegalArgumentException();
    std::lock_guard<std::mutex> lock(mutex_);
    cacheSize_ = size;
    purgeExpiredLocked();
    enforceCacheSizeLocked();
}

void OpenSslSessionContext::registerSession(
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>& session) {
    if (session == nullptr) throw ::jxx::lang::NullPointerException();
    const auto id = session->getId();
    if (id == nullptr || id->length == 0) return;
    const auto key = keyOf(id);
    std::lock_guard<std::mutex> lock(mutex_);
    purgeExpiredLocked();
    sessions_[key] = session;
    insertionOrder_.erase(
        std::remove(insertionOrder_.begin(), insertionOrder_.end(), key),
        insertionOrder_.end());
    insertionOrder_.push_back(key);
    enforceCacheSizeLocked();
}

void OpenSslSessionContext::removeSession(
    const ::jxx::lang::ByteArray& sessionId) {
    const auto key = keyOf(sessionId);
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(key);
    insertionOrder_.erase(
        std::remove(insertionOrder_.begin(), insertionOrder_.end(), key),
        insertionOrder_.end());
}


namespace {
std::string endpointKey(
    const ::jxx::Ptr<::jxx::lang::String>& peerHost,
    ::jxx::lang::jint peerPort) {
    if (peerHost == nullptr || peerHost->utf8().empty() || peerPort < 0)
        return std::string();
    return peerHost->utf8() + ":" + std::to_string(peerPort);
}
} // namespace

SSL_SESSION* OpenSslSessionContext::acquireNativeSession(
    const ::jxx::Ptr<::jxx::lang::String>& peerHost,
    ::jxx::lang::jint peerPort) {
    const auto key = endpointKey(peerHost, peerPort);
    if (key.empty()) return nullptr;
    std::lock_guard<std::mutex> lock(mutex_);
    const auto found = nativeSessions_.find(key);
    if (found == nativeSessions_.end()) return nullptr;
    SSL_SESSION_up_ref(found->second);
    return found->second;
}

void OpenSslSessionContext::registerNativeSession(
    const ::jxx::Ptr<::jxx::lang::String>& peerHost,
    ::jxx::lang::jint peerPort,
    SSL_SESSION* session) {
    if (session == nullptr) return;
    const auto key = endpointKey(peerHost, peerPort);
    if (key.empty()) return;
    SSL_SESSION_up_ref(session);
    std::lock_guard<std::mutex> lock(mutex_);
    const auto found = nativeSessions_.find(key);
    if (found != nativeSessions_.end()) {
        SSL_SESSION_free(found->second);
        found->second = session;
    } else {
        nativeSessions_.emplace(key, session);
    }
}

} // namespace jxx::ext::net::ssl::internal
