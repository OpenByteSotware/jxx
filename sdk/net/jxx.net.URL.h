#pragma once
#include "lang/jxx.lang.ClassInfo.h"

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io
{
    class InputStream;
}

namespace jxx::net
{
    class Proxy;
    class URI;
    class URLConnection;
    class URLStreamHandler;
    class URLStreamHandlerFactory;

    class URL final : public jxx::lang::ClassBase<URL, jxx::lang::Object> {
public:
    using JavaSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<URL, jxx::lang::Object>;

    public:
        explicit URL(const jxx::Ptr<jxx::lang::String>& spec);
        URL(const jxx::Ptr<URL>& context,
            const jxx::Ptr<jxx::lang::String>& spec);
        URL(const jxx::Ptr<URL>& context,
            const jxx::Ptr<jxx::lang::String>& spec,
            const jxx::Ptr<URLStreamHandler>& handler);
        URL(const jxx::Ptr<jxx::lang::String>& protocol,
            const jxx::Ptr<jxx::lang::String>& host,
            jxx::lang::jint port,
            const jxx::Ptr<jxx::lang::String>& file);
        URL(const jxx::Ptr<jxx::lang::String>& protocol,
            const jxx::Ptr<jxx::lang::String>& host,
            const jxx::Ptr<jxx::lang::String>& file);
        ~URL() override = default;

    public:
        static void setURLStreamHandlerFactory(const jxx::Ptr<URLStreamHandlerFactory>& fac);

        jxx::Ptr<jxx::lang::String> getProtocol() const;
        jxx::Ptr<jxx::lang::String> getAuthority() const;
        jxx::Ptr<jxx::lang::String> getHost() const;
        jxx::lang::jint getPort() const noexcept;
        jxx::lang::jint getDefaultPort() const;
        jxx::Ptr<jxx::lang::String> getPath() const;
        jxx::Ptr<jxx::lang::String> getQuery() const;
        jxx::Ptr<jxx::lang::String> getFile() const;
        jxx::Ptr<jxx::lang::String> getRef() const;
        jxx::Ptr<jxx::lang::String> getUserInfo() const;

        jxx::Ptr<URI> toURI() const;

        jxx::Ptr<URLConnection> openConnection();
        jxx::Ptr<URLConnection> openConnection(const jxx::Ptr<Proxy>& p);
        jxx::Ptr<jxx::io::InputStream> openStream();
        jxx::Ptr<jxx::lang::Object> getContent();

        jxx::lang::jbool sameFile(const jxx::Ptr<URL>& other) const;
        jxx::Ptr<jxx::lang::String> toExternalForm() const;
        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& other) const override;
        jxx::lang::jint hashCode() const override;

    private:
        void parse_(const jxx::Ptr<URL>& context,
                    const jxx::Ptr<jxx::lang::String>& spec,
                    const jxx::Ptr<URLStreamHandler>& handler);
        jxx::Ptr<URLStreamHandler> handlerFor_(const jxx::Ptr<jxx::lang::String>& protocol) const;

    private:
        jxx::Ptr<jxx::lang::String> protocol_;
        jxx::Ptr<jxx::lang::String> authority_;
        jxx::Ptr<jxx::lang::String> userInfo_;
        jxx::Ptr<jxx::lang::String> host_;
        jxx::lang::jint port_ = -1;
        jxx::Ptr<jxx::lang::String> path_;
        jxx::Ptr<jxx::lang::String> query_;
        jxx::Ptr<jxx::lang::String> ref_;
        jxx::Ptr<URLStreamHandler> handler_;
    };
}
