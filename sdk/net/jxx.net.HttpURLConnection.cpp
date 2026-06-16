#include "net/jxx.net.HttpURLConnection.h"

#include <algorithm>
#include <stdexcept>

namespace
{
    jxx::lang::jbool g_followRedirects = true;
}

namespace jxx::net
{
    HttpURLConnection::HttpURLConnection(jxx::Ptr<URL> url)
        : URLConnection(std::move(url)),
          method_(jxx::NEW<jxx::lang::String>("GET")),
          responseMessage_(jxx::NEW<jxx::lang::String>("Not Connected"))
    {
    }

    void HttpURLConnection::setFollowRedirects(jxx::lang::jbool set) { g_followRedirects = set; }
    jxx::lang::jbool HttpURLConnection::getFollowRedirects() { return g_followRedirects; }

    void HttpURLConnection::setInstanceFollowRedirects(jxx::lang::jbool x) { instanceFollowRedirects_ = x; }
    jxx::lang::jbool HttpURLConnection::getInstanceFollowRedirects() const noexcept { return instanceFollowRedirects_; }

    void HttpURLConnection::setRequestMethod(jxx::Ptr<jxx::lang::String> method)
    {
        if (!method)
            throw std::invalid_argument("null method");
        const auto m = method->utf8();
        static const char* allowed[] = {"GET","POST","HEAD","OPTIONS","PUT","DELETE","TRACE"};
        if (std::find(std::begin(allowed), std::end(allowed), m) == std::end(allowed))
            throw std::invalid_argument("invalid HTTP method");
        method_ = method;
    }

    jxx::Ptr<jxx::lang::String> HttpURLConnection::getRequestMethod() const { return method_; }
    jxx::lang::jint HttpURLConnection::getResponseCode() const { return responseCode_; }
    jxx::Ptr<jxx::lang::String> HttpURLConnection::getResponseMessage() const { return responseMessage_; }
}
