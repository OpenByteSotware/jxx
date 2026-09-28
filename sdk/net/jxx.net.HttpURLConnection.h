#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "net/jxx.net.URLConnection.h"

namespace jxx::io { class InputStream; }

namespace jxx::net {

class HttpURLConnection
    : public ::jxx::lang::ClassBase<HttpURLConnection, URLConnection> {
public:
    using JxxSuper = URLConnection;
    using Super = ::jxx::lang::ClassBase<HttpURLConnection, URLConnection>;

    static constexpr ::jxx::lang::jint HTTP_CONTINUE = 100;
    static constexpr ::jxx::lang::jint HTTP_OK = 200;
    static constexpr ::jxx::lang::jint HTTP_CREATED = 201;
    static constexpr ::jxx::lang::jint HTTP_ACCEPTED = 202;
    static constexpr ::jxx::lang::jint HTTP_NOT_AUTHORITATIVE = 203;
    static constexpr ::jxx::lang::jint HTTP_NO_CONTENT = 204;
    static constexpr ::jxx::lang::jint HTTP_RESET = 205;
    static constexpr ::jxx::lang::jint HTTP_PARTIAL = 206;
    static constexpr ::jxx::lang::jint HTTP_MULT_CHOICE = 300;
    static constexpr ::jxx::lang::jint HTTP_MOVED_PERM = 301;
    static constexpr ::jxx::lang::jint HTTP_MOVED_TEMP = 302;
    static constexpr ::jxx::lang::jint HTTP_SEE_OTHER = 303;
    static constexpr ::jxx::lang::jint HTTP_NOT_MODIFIED = 304;
    static constexpr ::jxx::lang::jint HTTP_USE_PROXY = 305;
    static constexpr ::jxx::lang::jint HTTP_BAD_REQUEST = 400;
    static constexpr ::jxx::lang::jint HTTP_UNAUTHORIZED = 401;
    static constexpr ::jxx::lang::jint HTTP_PAYMENT_REQUIRED = 402;
    static constexpr ::jxx::lang::jint HTTP_FORBIDDEN = 403;
    static constexpr ::jxx::lang::jint HTTP_NOT_FOUND = 404;
    static constexpr ::jxx::lang::jint HTTP_BAD_METHOD = 405;
    static constexpr ::jxx::lang::jint HTTP_NOT_ACCEPTABLE = 406;
    static constexpr ::jxx::lang::jint HTTP_PROXY_AUTH = 407;
    static constexpr ::jxx::lang::jint HTTP_CLIENT_TIMEOUT = 408;
    static constexpr ::jxx::lang::jint HTTP_CONFLICT = 409;
    static constexpr ::jxx::lang::jint HTTP_GONE = 410;
    static constexpr ::jxx::lang::jint HTTP_LENGTH_REQUIRED = 411;
    static constexpr ::jxx::lang::jint HTTP_PRECON_FAILED = 412;
    static constexpr ::jxx::lang::jint HTTP_ENTITY_TOO_LARGE = 413;
    static constexpr ::jxx::lang::jint HTTP_REQ_TOO_LONG = 414;
    static constexpr ::jxx::lang::jint HTTP_UNSUPPORTED_TYPE = 415;
    static constexpr ::jxx::lang::jint HTTP_SERVER_ERROR = 500;
    static constexpr ::jxx::lang::jint HTTP_INTERNAL_ERROR = 500;
    static constexpr ::jxx::lang::jint HTTP_NOT_IMPLEMENTED = 501;
    static constexpr ::jxx::lang::jint HTTP_BAD_GATEWAY = 502;
    static constexpr ::jxx::lang::jint HTTP_UNAVAILABLE = 503;
    static constexpr ::jxx::lang::jint HTTP_GATEWAY_TIMEOUT = 504;
    static constexpr ::jxx::lang::jint HTTP_VERSION = 505;

    ~HttpURLConnection() override = default;

    static void setFollowRedirects(::jxx::lang::jbool set);
    static ::jxx::lang::jbool getFollowRedirects();

    void setInstanceFollowRedirects(::jxx::lang::jbool followRedirects);
    ::jxx::lang::jbool getInstanceFollowRedirects() const noexcept;

    void setRequestMethod(const ::jxx::Ptr<::jxx::lang::String>& method);
    ::jxx::Ptr<::jxx::lang::String> getRequestMethod() const;

    void setFixedLengthStreamingMode(::jxx::lang::jint contentLength);
    void setFixedLengthStreamingMode(::jxx::lang::jlong contentLength);
    void setChunkedStreamingMode(::jxx::lang::jint chunkLength);

    virtual ::jxx::lang::jint getResponseCode();
    virtual ::jxx::Ptr<::jxx::lang::String> getResponseMessage();
    virtual ::jxx::Ptr<::jxx::io::InputStream> getErrorStream();

    virtual void disconnect() = 0;
    virtual ::jxx::lang::jbool usingProxy() const = 0;

protected:
    explicit HttpURLConnection(const ::jxx::Ptr<URL>& url);

    ::jxx::Ptr<::jxx::lang::String> method_;
    ::jxx::lang::jint responseCode_ = -1;
    ::jxx::Ptr<::jxx::lang::String> responseMessage_;
    ::jxx::lang::jbool instanceFollowRedirects_ = true;
    ::jxx::lang::jint fixedContentLength_ = -1;
    ::jxx::lang::jlong fixedContentLengthLong_ = -1;
    ::jxx::lang::jint chunkLength_ = -1;
};

} // namespace jxx::net
