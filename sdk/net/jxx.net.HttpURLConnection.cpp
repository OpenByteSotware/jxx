#include "net/jxx.net.HttpURLConnection.h"

#include <algorithm>
#include <array>
#include <mutex>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.ProtocolException.h"

namespace {
std::mutex g_redirectMutex;
::jxx::lang::jbool g_followRedirects = true;
}

namespace jxx::net {

HttpURLConnection::HttpURLConnection(const ::jxx::Ptr<URL>& url)
    : Super(url),
      method_(::jxx::NEW<::jxx::lang::String>("GET")),
      responseMessage_(::jxx::NEW<::jxx::lang::String>("Not Connected"))
{
    std::lock_guard<std::mutex> lock(g_redirectMutex);
    instanceFollowRedirects_ = g_followRedirects;
}

void HttpURLConnection::setFollowRedirects(::jxx::lang::jbool set)
{
    std::lock_guard<std::mutex> lock(g_redirectMutex);
    g_followRedirects = set;
}

::jxx::lang::jbool HttpURLConnection::getFollowRedirects()
{
    std::lock_guard<std::mutex> lock(g_redirectMutex);
    return g_followRedirects;
}

void HttpURLConnection::setInstanceFollowRedirects(::jxx::lang::jbool value)
{
    instanceFollowRedirects_ = value;
}

::jxx::lang::jbool HttpURLConnection::getInstanceFollowRedirects() const noexcept
{
    return instanceFollowRedirects_;
}

void HttpURLConnection::setRequestMethod(
    const ::jxx::Ptr<::jxx::lang::String>& method)
{
    if (connected_) throw ::jxx::net::ProtocolException("already connected");
    if (method == nullptr) throw ::jxx::net::ProtocolException("null method");

    const auto text = method->utf8();
    static const std::array<const char*, 7> allowed{
        "GET", "POST", "HEAD", "OPTIONS", "PUT", "DELETE", "TRACE"};

    if (std::find(allowed.begin(), allowed.end(), text) == allowed.end())
        throw ::jxx::net::ProtocolException("invalid HTTP method");

    method_ = method;
}

::jxx::Ptr<::jxx::lang::String> HttpURLConnection::getRequestMethod() const
{
    return method_;
}

void HttpURLConnection::setFixedLengthStreamingMode(::jxx::lang::jint length)
{
    setFixedLengthStreamingMode(static_cast<::jxx::lang::jlong>(length));
}

void HttpURLConnection::setFixedLengthStreamingMode(::jxx::lang::jlong length)
{
    if (connected_) throw ::jxx::lang::IllegalStateException();
    if (chunkLength_ >= 0) throw ::jxx::lang::IllegalStateException();
    if (length < 0) throw ::jxx::lang::IllegalArgumentException();

    fixedContentLengthLong_ = length;
    fixedContentLength_ = length <= 0x7fffffffLL
        ? static_cast<::jxx::lang::jint>(length)
        : -1;
}

void HttpURLConnection::setChunkedStreamingMode(::jxx::lang::jint length)
{
    if (connected_) throw ::jxx::lang::IllegalStateException();
    if (fixedContentLength_ >= 0 || fixedContentLengthLong_ >= 0)
        throw ::jxx::lang::IllegalStateException();

    chunkLength_ = length <= 0 ? 4096 : length;
}

::jxx::lang::jint HttpURLConnection::getResponseCode()
{
    if (!connected_) connect();
    return responseCode_;
}

::jxx::Ptr<::jxx::lang::String> HttpURLConnection::getResponseMessage()
{
    if (!connected_) connect();
    return responseMessage_;
}

::jxx::Ptr<::jxx::io::InputStream> HttpURLConnection::getErrorStream()
{
    return nullptr;
}

} // namespace jxx::net
