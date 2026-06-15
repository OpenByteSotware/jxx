#pragma once

#include "net/jxx.net.URLConnection.h"

namespace jxx::net
{
    class HttpURLConnection : public URLConnection
    {
    protected:
        explicit HttpURLConnection(jxx::Ptr<URL> url);

    public:
        ~HttpURLConnection() override = default;

    public:
        static void setFollowRedirects(jxx::lang::jbool set);
        static jxx::lang::jbool getFollowRedirects();

        void setInstanceFollowRedirects(jxx::lang::jbool followRedirects);
        jxx::lang::jbool getInstanceFollowRedirects() const noexcept;

        void setRequestMethod(jxx::Ptr<jxx::lang::String> method);
        jxx::Ptr<jxx::lang::String> getRequestMethod() const;

        virtual jxx::lang::jint getResponseCode() const;
        virtual jxx::Ptr<jxx::lang::String> getResponseMessage() const;

        virtual void disconnect() = 0;
        virtual jxx::lang::jbool usingProxy() const = 0;

    protected:
        jxx::Ptr<jxx::lang::String> method_;
        jxx::lang::jint responseCode_ = -1;
        jxx::Ptr<jxx::lang::String> responseMessage_;
        jxx::lang::jbool instanceFollowRedirects_ = true;
    };
}
