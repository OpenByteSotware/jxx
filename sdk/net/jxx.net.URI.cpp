#include "net/jxx.net.URI.h"

#include <stdexcept>

#include "net/internal/jxx.net.internal.UrlUtil.h"
#include "net/jxx.net.URISyntaxException.h"
#include "net/jxx.net.URL.h"

namespace
{
    inline jxx::Ptr<jxx::lang::String> S_(const std::string& s)
    {
        return s.empty() ? nullptr : std::make_shared<jxx::lang::String>(s);
    }
}

namespace jxx::net
{
    URI::URI(jxx::Ptr<jxx::lang::String> str)
    {
        parse_(std::move(str));
    }

    URI::URI(jxx::Ptr<jxx::lang::String> scheme,
             jxx::Ptr<jxx::lang::String> ssp,
             jxx::Ptr<jxx::lang::String> fragment)
        : scheme_(std::move(scheme)),
          schemeSpecificPart_(std::move(ssp)),
          fragment_(std::move(fragment)),
          opaque_(true)
    {
    }

    URI::URI(jxx::Ptr<jxx::lang::String> scheme,
             jxx::Ptr<jxx::lang::String> authority,
             jxx::Ptr<jxx::lang::String> path,
             jxx::Ptr<jxx::lang::String> query,
             jxx::Ptr<jxx::lang::String> fragment)
        : scheme_(std::move(scheme)),
          authority_(std::move(authority)),
          path_(std::move(path)),
          query_(std::move(query)),
          fragment_(std::move(fragment))
    {
        schemeSpecificPart_ = std::make_shared<jxx::lang::String>((authority_ ? std::string("//") + authority_->utf8() : std::string()) + (path_ ? path_->utf8() : std::string()) + (query_ ? std::string("?") + query_->utf8() : std::string()));
    }

    URI::URI(jxx::Ptr<jxx::lang::String> scheme,
             jxx::Ptr<jxx::lang::String> host,
             jxx::Ptr<jxx::lang::String> path,
             jxx::Ptr<jxx::lang::String> fragment)
        : URI(std::move(scheme), nullptr, std::move(host), -1, std::move(path), nullptr, std::move(fragment))
    {
    }

    URI::URI(jxx::Ptr<jxx::lang::String> scheme,
             jxx::Ptr<jxx::lang::String> userInfo,
             jxx::Ptr<jxx::lang::String> host,
             jxx::lang::jint port,
             jxx::Ptr<jxx::lang::String> path,
             jxx::Ptr<jxx::lang::String> query,
             jxx::Ptr<jxx::lang::String> fragment)
        : scheme_(std::move(scheme)),
          userInfo_(std::move(userInfo)),
          host_(std::move(host)),
          port_(port),
          path_(std::move(path)),
          query_(std::move(query)),
          fragment_(std::move(fragment))
    {
        std::string authority;
        if (userInfo_)
            authority += userInfo_->utf8() + "@";
        if (host_)
            authority += host_->utf8();
        if (port_ >= 0)
            authority += ":" + std::to_string(port_);
        authority_ = authority.empty() ? nullptr : std::make_shared<jxx::lang::String>(authority);
        schemeSpecificPart_ = std::make_shared<jxx::lang::String>((authority_ ? std::string("//") + authority_->utf8() : std::string()) + (path_ ? path_->utf8() : std::string()) + (query_ ? std::string("?") + query_->utf8() : std::string()));
    }

    void URI::parse_(jxx::Ptr<jxx::lang::String> spec)
    {
        if (!spec)
            throw URISyntaxException(nullptr, std::make_shared<jxx::lang::String>("null URI"));
        const auto p = internal::parseUriLike(spec->utf8());
        scheme_ = S_(p.scheme);
        schemeSpecificPart_ = S_(p.schemeSpecificPart);
        authority_ = S_(p.authority);
        userInfo_ = S_(p.userInfo);
        host_ = S_(p.host);
        port_ = p.port;
        path_ = S_(p.path);
        query_ = S_(p.query);
        fragment_ = S_(p.fragment);
        opaque_ = p.opaque;
    }

    jxx::Ptr<URI> URI::create(jxx::Ptr<jxx::lang::String> str)
    {
        return std::make_shared<URI>(std::move(str));
    }

    jxx::lang::jbool URI::isAbsolute() const { return scheme_ != nullptr; }
    jxx::lang::jbool URI::isOpaque() const { return opaque_; }

    jxx::Ptr<jxx::lang::String> URI::getScheme() const { return scheme_; }
    jxx::Ptr<jxx::lang::String> URI::getSchemeSpecificPart() const { return schemeSpecificPart_; }
    jxx::Ptr<jxx::lang::String> URI::getRawSchemeSpecificPart() const { return schemeSpecificPart_; }
    jxx::Ptr<jxx::lang::String> URI::getAuthority() const { return authority_; }
    jxx::Ptr<jxx::lang::String> URI::getRawAuthority() const { return authority_; }
    jxx::Ptr<jxx::lang::String> URI::getUserInfo() const { return userInfo_; }
    jxx::Ptr<jxx::lang::String> URI::getRawUserInfo() const { return userInfo_; }
    jxx::Ptr<jxx::lang::String> URI::getHost() const { return host_; }
    jxx::lang::jint URI::getPort() const noexcept { return port_; }
    jxx::Ptr<jxx::lang::String> URI::getPath() const { return path_; }
    jxx::Ptr<jxx::lang::String> URI::getRawPath() const { return path_; }
    jxx::Ptr<jxx::lang::String> URI::getQuery() const { return query_; }
    jxx::Ptr<jxx::lang::String> URI::getRawQuery() const { return query_; }
    jxx::Ptr<jxx::lang::String> URI::getFragment() const { return fragment_; }
    jxx::Ptr<jxx::lang::String> URI::getRawFragment() const { return fragment_; }

    jxx::Ptr<URI> URI::normalize() const
    {
        if (!path_)
            return std::make_shared<URI>(toString());
        auto u = std::make_shared<URI>(toString());
        u->path_ = std::make_shared<jxx::lang::String>(internal::normalizePath(path_->utf8()));
        const auto p = internal::parseUriLike(u->toString()->utf8());
        u->schemeSpecificPart_ = S_(p.schemeSpecificPart);
        return u;
    }

    jxx::Ptr<URI> URI::resolve(jxx::Ptr<URI> uri) const
    {
        if (!uri)
            return std::make_shared<URI>(toString());
        if (uri->isAbsolute())
            return uri;

        auto u = std::make_shared<URI>(toString());
        if (uri->path_ && !uri->path_->utf8().empty())
            u->path_ = std::make_shared<jxx::lang::String>(internal::resolvePath(path_ ? path_->utf8() : std::string(), uri->path_->utf8()));
        if (uri->query_)
            u->query_ = uri->query_;
        if (uri->fragment_)
            u->fragment_ = uri->fragment_;
        const auto rebuilt = u->toString();
        return std::make_shared<URI>(rebuilt);
    }

    jxx::Ptr<URI> URI::resolve(jxx::Ptr<jxx::lang::String> str) const
    {
        return resolve(std::make_shared<URI>(std::move(str)));
    }

    jxx::Ptr<URI> URI::relativize(jxx::Ptr<URI> uri) const
    {
        if (!uri)
            return nullptr;
        if ((scheme_ && uri->scheme_ && !scheme_->equals(uri->scheme_)) ||
            (authority_ && uri->authority_ && !authority_->equals(uri->authority_)))
            return uri;
        const std::string basePath = path_ ? path_->utf8() : std::string();
        const std::string childPath = uri->path_ ? uri->path_->utf8() : std::string();
        if (childPath.rfind(basePath, 0) != 0)
            return uri;
        return std::make_shared<URI>(std::make_shared<jxx::lang::String>(childPath.substr(basePath.size())));
    }

    jxx::Ptr<URI> URI::parseServerAuthority() const
    {
        return std::make_shared<URI>(toString());
    }

    jxx::Ptr<URL> URI::toURL() const
    {
        return std::make_shared<URL>(toString());
    }

    jxx::Ptr<jxx::lang::String> URI::toASCIIString() const { return toString(); }

    jxx::Ptr<jxx::lang::String> URI::toString() const
    {
        internal::ParsedUrl p;
        p.scheme = scheme_ ? scheme_->utf8() : std::string();
        p.schemeSpecificPart = schemeSpecificPart_ ? schemeSpecificPart_->utf8() : std::string();
        p.authority = authority_ ? authority_->utf8() : std::string();
        p.userInfo = userInfo_ ? userInfo_->utf8() : std::string();
        p.host = host_ ? host_->utf8() : std::string();
        p.port = port_;
        p.path = path_ ? path_->utf8() : std::string();
        p.query = query_ ? query_->utf8() : std::string();
        p.fragment = fragment_ ? fragment_->utf8() : std::string();
        p.opaque = opaque_;
        return std::make_shared<jxx::lang::String>(internal::rebuildUri(p));
    }

    jxx::lang::jbool URI::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto u = std::dynamic_pointer_cast<URI>(other);
        return u && toString()->equals(u->toString());
    }

    jxx::lang::jint URI::hashCode() const { return toString()->hashCode(); }

    jxx::lang::jint URI::compareTo(jxx::Ptr<URI> other) const
    {
        if (!other)
            return 1;
        const auto a = toString()->utf8();
        const auto b = other->toString()->utf8();
        if (a < b) return -1;
        if (a > b) return 1;
        return 0;
    }
}
