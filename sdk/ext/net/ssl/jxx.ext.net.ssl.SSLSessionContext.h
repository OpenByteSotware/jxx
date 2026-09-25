#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Enumeration.h"

namespace jxx::ext::net::ssl {

class SSLSession;

class SSLSessionContext
    : public ::jxx::lang::InterfaceBase<SSLSessionContext> {
public:
    using IdArray =
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>;

    ~SSLSessionContext() override = default;

    virtual ::jxx::Ptr<::jxx::util::Enumeration<IdArray>>
    getIds() = 0;

    virtual ::jxx::Ptr<SSLSession> getSession(
        const ::jxx::lang::ByteArray& sessionId) = 0;

    virtual ::jxx::lang::jint getSessionTimeout() const = 0;
    virtual void setSessionTimeout(::jxx::lang::jint seconds) = 0;
    virtual ::jxx::lang::jint getSessionCacheSize() const = 0;
    virtual void setSessionCacheSize(::jxx::lang::jint size) = 0;
};

} // namespace jxx::ext::net::ssl
