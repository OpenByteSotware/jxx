#include "net/jxx.net.URL.h"

#include <stdexcept>

#include "net/internal/jxx.net.internal.UrlUtil.h"
#include "net/jxx.net.HttpURLConnection.h"
#include "net/jxx.net.JarURLConnection.h"
#include "net/jxx.net.MalformedURLException.h"
#include "net/jxx.net.Proxy.h" // optional later bundle; forward only if absent at compile integration it can be adjusted
#include "net/jxx.net.URI.h"
#include "net/jxx.net.URLConnection.h"
#include "net/jxx.net.URLDecoder.h"
#include "net/jxx.net.URLStreamHandler.h"
#include "net/jxx.net.URLStreamHandlerFactory.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.FileInputStream.h"

namespace
{
    jxx::Ptr<jxx::net::URLStreamHandlerFactory> g_factory;

    inline jxx::Ptr<jxx::lang::String> S_(const std::string& s)
    {
        return s.empty() ? nullptr : jxx::NEW<jxx::lang::String>(s);
    }

    class BasicURLConnection final : public jxx::net::URLConnection
    {
    public:
        BasicURLConnection(jxx::Ptr<jxx::net::URL> url,
                           const std::string& path,
                           const std::string& type)
            : URLConnection(std::move(url)),
              filePath_(path)
        {
            if (!type.empty())
                headerFields_["content-type"] = type;
        }

        void connect() override
        {
            connected_ = true;
        }

        jxx::Ptr<jxx::io::InputStream> getInputStream() override
        {
            connect();
            if (filePath_.empty())
                throw std::runtime_error("no backing file");
            return jxx::NEW<jxx::io::FileInputStream>(jxx::NEW<jxx::lang::String>(filePath_));
        }

    private:
        std::string filePath_;
    };

    class BasicHttpURLConnection final : public jxx::net::HttpURLConnection
    {
    public:
        explicit BasicHttpURLConnection(jxx::Ptr<jxx::net::URL> url)
            : HttpURLConnection(std::move(url))
        {
            responseMessage_ = jxx::NEW<jxx::lang::String>("HTTP transport not yet implemented in Bundle B");
        }

        void connect() override
        {
            connected_ = true;
            responseCode_ = 0;
        }

        void disconnect() override
        {
            connected_ = false;
        }

        jxx::lang::jbool usingProxy() const override
        {
            return false;
        }
    };

    class BasicJarURLConnection final : public jxx::net::JarURLConnection
    {
    public:
        BasicJarURLConnection(jxx::Ptr<jxx::net::URL> url,
                              jxx::Ptr<jxx::net::URL> jarFileURL,
                              jxx::Ptr<jxx::lang::String> entryName)
            : JarURLConnection(std::move(url))
        {
            jarFileURL_ = std::move(jarFileURL);
            entryName_ = std::move(entryName);
        }

        void connect() override
        {
            connected_ = true;
        }
    };

    class DefaultURLStreamHandler final : public jxx::net::URLStreamHandler
    {
    public:
        explicit DefaultURLStreamHandler(std::string protocol)
            : protocol_(std::move(protocol))
        {
        }

    protected:
        jxx::Ptr<jxx::net::URLConnection> openConnection(jxx::Ptr<jxx::net::URL> u) override
        {
            const auto proto = protocol_;
            if (proto == "file")
            {
                auto path = u->getPath();
                auto decoded = path ? jxx::net::URLDecoder::decode(path, jxx::NEW<jxx::lang::String>("UTF-8")) : nullptr;
                std::string p = decoded ? decoded->utf8() : std::string();
            #if defined(_WIN32)
                if (p.size() >= 3 && p[0] == '/' && std::isalpha(static_cast<unsigned char>(p[1])) && p[2] == ':')
                    p = p.substr(1);
            #endif
                const auto type = jxx::net::URLConnection::guessContentTypeFromName(jxx::NEW<jxx::lang::String>(p));
                return jxx::NEW<BasicURLConnection>(u, p, type ? type->utf8() : std::string());
            }
            if (proto == "http" || proto == "https")
                return jxx::NEW<BasicHttpURLConnection>(u);
            if (proto == "jar")
            {
                auto spec = u->toExternalForm()->utf8();
                const auto bang = spec.find("!/");
                if (bang == std::string::npos)
                    return jxx::NEW<BasicJarURLConnection>(u, nullptr, nullptr);
                auto jarUrl = jxx::NEW<jxx::net::URL>(jxx::NEW<jxx::lang::String>(spec.substr(4, bang - 4)));
                auto entry = jxx::NEW<jxx::lang::String>(spec.substr(bang + 2));
                return jxx::NEW<BasicJarURLConnection>(u, jarUrl, entry);
            }
            return jxx::NEW<BasicURLConnection>(u, std::string(), std::string());
        }

        jxx::lang::jint getDefaultPort() const override
        {
            return jxx::net::internal::defaultPortForScheme(protocol_);
        }

    private:
        std::string protocol_;
    };
}

namespace jxx::net
{
    URL::URL(jxx::Ptr<jxx::lang::String> spec)
    {
        parse_(nullptr, std::move(spec), nullptr);
    }

    URL::URL(jxx::Ptr<URL> context,
             jxx::Ptr<jxx::lang::String> spec)
    {
        parse_(std::move(context), std::move(spec), nullptr);
    }

    URL::URL(jxx::Ptr<URL> context,
             jxx::Ptr<jxx::lang::String> spec,
             jxx::Ptr<URLStreamHandler> handler)
    {
        parse_(std::move(context), std::move(spec), std::move(handler));
    }

    URL::URL(jxx::Ptr<jxx::lang::String> protocol,
             jxx::Ptr<jxx::lang::String> host,
             jxx::lang::jint port,
             jxx::Ptr<jxx::lang::String> file)
    {
        std::string spec = (protocol ? protocol->utf8() : std::string()) + "://" + (host ? host->utf8() : std::string());
        if (port >= 0)
            spec += ":" + std::to_string(port);
        spec += (file ? file->utf8() : std::string());
        parse_(nullptr, jxx::NEW<jxx::lang::String>(spec), nullptr);
    }

    URL::URL(jxx::Ptr<jxx::lang::String> protocol,
             jxx::Ptr<jxx::lang::String> host,
             jxx::Ptr<jxx::lang::String> file)
        : URL(std::move(protocol), std::move(host), -1, std::move(file))
    {
    }

    void URL::setURLStreamHandlerFactory(jxx::Ptr<URLStreamHandlerFactory> fac)
    {
        g_factory = std::move(fac);
    }

    jxx::Ptr<URLStreamHandler> URL::handlerFor_(jxx::Ptr<jxx::lang::String> protocol) const
    {
        if (g_factory && protocol)
        {
            auto h = g_factory->createURLStreamHandler(protocol);
            if (h)
                return h;
        }
        return jxx::NEW<DefaultURLStreamHandler>(protocol ? protocol->utf8() : std::string());
    }

    void URL::parse_(jxx::Ptr<URL> context,
                     jxx::Ptr<jxx::lang::String> spec,
                     jxx::Ptr<URLStreamHandler> handler)
    {
        if (!spec)
            throw MalformedURLException("null URL");

        auto parsed = internal::parseUriLike(spec->utf8());
        if (parsed.scheme.empty() && context)
        {
            const auto base = std::dynamic_pointer_cast<URI>(context->toURI());
            const auto rel = jxx::NEW<URI>(spec);
            parsed = internal::parseUriLike(base->resolve(rel)->toString()->utf8());
        }
        if (parsed.scheme.empty())
            throw MalformedURLException("missing protocol");

        protocol_ = S_(parsed.scheme);
        authority_ = S_(parsed.authority);
        userInfo_ = S_(parsed.userInfo);
        host_ = S_(parsed.host);
        port_ = parsed.port;
        path_ = S_(parsed.path);
        query_ = S_(parsed.query);
        ref_ = S_(parsed.fragment);
        handler_ = handler ? std::move(handler) : handlerFor_(protocol_);
    }

    jxx::Ptr<jxx::lang::String> URL::getProtocol() const { return protocol_; }
    jxx::Ptr<jxx::lang::String> URL::getAuthority() const { return authority_; }
    jxx::Ptr<jxx::lang::String> URL::getHost() const { return host_; }
    jxx::lang::jint URL::getPort() const noexcept { return port_; }
    jxx::lang::jint URL::getDefaultPort() const { return handler_ ? handler_->getDefaultPort() : internal::defaultPortForScheme(protocol_ ? protocol_->utf8() : std::string()); }
    jxx::Ptr<jxx::lang::String> URL::getPath() const { return path_; }
    jxx::Ptr<jxx::lang::String> URL::getQuery() const { return query_; }
    jxx::Ptr<jxx::lang::String> URL::getFile() const { return jxx::NEW<jxx::lang::String>((path_ ? path_->utf8() : std::string()) + (query_ ? std::string("?") + query_->utf8() : std::string())); }
    jxx::Ptr<jxx::lang::String> URL::getRef() const { return ref_; }
    jxx::Ptr<jxx::lang::String> URL::getUserInfo() const { return userInfo_; }

    jxx::Ptr<URI> URL::toURI() const
    {
        return jxx::NEW<URI>(toExternalForm());
    }

    jxx::Ptr<URLConnection> URL::openConnection()
    {
        if (!handler_)
            throw MalformedURLException("no stream handler");
        return handler_->openConnection(jxx::CAST<URL, jxx::lang::Object>(this->thisPtr));
    }

    jxx::Ptr<URLConnection> URL::openConnection(jxx::Ptr<Proxy> p)
    {
        if (!handler_)
            throw MalformedURLException("no stream handler");
        return handler_->openConnection(jxx::CAST<URL, jxx::lang::Object>(this->thisPtr), std::move(p));
    }

    jxx::Ptr<jxx::io::InputStream> URL::openStream()
    {
        auto c = openConnection();
        return c ? c->getInputStream() : nullptr;
    }

    jxx::Ptr<jxx::lang::Object> URL::getContent()
    {
        auto c = openConnection();
        return c ? c->getContent() : nullptr;
    }

    jxx::lang::jbool URL::sameFile(jxx::Ptr<URL> other) const
    {
        if (!other)
            return false;
        const auto thisPort = (port_ >= 0) ? port_ : getDefaultPort();
        const auto otherPort = (other->port_ >= 0) ? other->port_ : other->getDefaultPort();
        return
            ((!protocol_ && !other->protocol_) || (protocol_ && other->protocol_ && protocol_->equals(other->protocol_))) &&
            ((!host_ && !other->host_) || (host_ && other->host_ && host_->equals(other->host_))) &&
            thisPort == otherPort &&
            ((!path_ && !other->path_) || (path_ && other->path_ && path_->equals(other->path_)));
    }

    jxx::Ptr<jxx::lang::String> URL::toExternalForm() const
    {
        internal::ParsedUrl p;
        p.scheme = protocol_ ? protocol_->utf8() : std::string();
        p.authority = authority_ ? authority_->utf8() : std::string();
        p.userInfo = userInfo_ ? userInfo_->utf8() : std::string();
        p.host = host_ ? host_->utf8() : std::string();
        p.port = port_;
        p.path = path_ ? path_->utf8() : std::string();
        p.query = query_ ? query_->utf8() : std::string();
        p.fragment = ref_ ? ref_->utf8() : std::string();
        return jxx::NEW<jxx::lang::String>(internal::rebuildUri(p));
    }

    jxx::Ptr<jxx::lang::String> URL::toString() const { return toExternalForm(); }

    jxx::lang::jbool URL::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto u = std::dynamic_pointer_cast<URL>(other);
        return u && toExternalForm()->equals(u->toExternalForm());
    }

    jxx::lang::jint URL::hashCode() const { return toExternalForm()->hashCode(); }
}
