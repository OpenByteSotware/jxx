#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::net
{
    class URL;
    class URLConnection;
    class Proxy;

    class URLStreamHandler : public jxx::lang::Object
    {
    public:
        ~URLStreamHandler() override = default;

    protected:
        virtual jxx::Ptr<URLConnection> openConnection(const jxx::Ptr<URL> u) = 0;
        virtual jxx::Ptr<URLConnection> openConnection(const jxx::Ptr<URL> u,
                                                       jxx::Ptr<Proxy> /*p*/)
        {
            return openConnection(std::move(u));
        }

        virtual jxx::lang::jint getDefaultPort() const;
        virtual jxx::lang::jbool equals(const jxx::Ptr<URL> u1,
                                        jxx::Ptr<URL> u2) const;
        virtual jxx::lang::jint hashCode(const jxx::Ptr<URL> u) const;
        virtual jxx::lang::jbool sameFile(const jxx::Ptr<URL> u1,
                                          jxx::Ptr<URL> u2) const;
        virtual jxx::Ptr<jxx::lang::String> toExternalForm(const jxx::Ptr<URL> u) const;

        friend class URL;
    };
}
