#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::net
{
    class URL;

    class URI final : public jxx::lang::Object
    {
    public:
        explicit URI(jxx::Ptr<jxx::lang::String> str);
        URI(jxx::Ptr<jxx::lang::String> scheme,
            jxx::Ptr<jxx::lang::String> ssp,
            jxx::Ptr<jxx::lang::String> fragment);
        URI(jxx::Ptr<jxx::lang::String> scheme,
            jxx::Ptr<jxx::lang::String> authority,
            jxx::Ptr<jxx::lang::String> path,
            jxx::Ptr<jxx::lang::String> query,
            jxx::Ptr<jxx::lang::String> fragment);
        URI(jxx::Ptr<jxx::lang::String> scheme,
            jxx::Ptr<jxx::lang::String> host,
            jxx::Ptr<jxx::lang::String> path,
            jxx::Ptr<jxx::lang::String> fragment);
        URI(jxx::Ptr<jxx::lang::String> scheme,
            jxx::Ptr<jxx::lang::String> userInfo,
            jxx::Ptr<jxx::lang::String> host,
            jxx::lang::jint port,
            jxx::Ptr<jxx::lang::String> path,
            jxx::Ptr<jxx::lang::String> query,
            jxx::Ptr<jxx::lang::String> fragment);
        ~URI() override = default;

    public:
        static jxx::Ptr<URI> create(jxx::Ptr<jxx::lang::String> str);

        jxx::lang::jbool isAbsolute() const;
        jxx::lang::jbool isOpaque() const;

        jxx::Ptr<jxx::lang::String> getScheme() const;
        jxx::Ptr<jxx::lang::String> getSchemeSpecificPart() const;
        jxx::Ptr<jxx::lang::String> getRawSchemeSpecificPart() const;
        jxx::Ptr<jxx::lang::String> getAuthority() const;
        jxx::Ptr<jxx::lang::String> getRawAuthority() const;
        jxx::Ptr<jxx::lang::String> getUserInfo() const;
        jxx::Ptr<jxx::lang::String> getRawUserInfo() const;
        jxx::Ptr<jxx::lang::String> getHost() const;
        jxx::lang::jint getPort() const noexcept;
        jxx::Ptr<jxx::lang::String> getPath() const;
        jxx::Ptr<jxx::lang::String> getRawPath() const;
        jxx::Ptr<jxx::lang::String> getQuery() const;
        jxx::Ptr<jxx::lang::String> getRawQuery() const;
        jxx::Ptr<jxx::lang::String> getFragment() const;
        jxx::Ptr<jxx::lang::String> getRawFragment() const;

        jxx::Ptr<URI> normalize() const;
        jxx::Ptr<URI> resolve(jxx::Ptr<URI> uri) const;
        jxx::Ptr<URI> resolve(jxx::Ptr<jxx::lang::String> str) const;
        jxx::Ptr<URI> relativize(jxx::Ptr<URI> uri) const;
        jxx::Ptr<URI> parseServerAuthority() const;
        jxx::Ptr<URL> toURL() const;

        jxx::Ptr<jxx::lang::String> toASCIIString() const;
        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> other) const override;
        jxx::lang::jint hashCode() const override;
        jxx::lang::jint compareTo(jxx::Ptr<URI> other) const;

    private:
        void parse_(jxx::Ptr<jxx::lang::String> spec);

    private:
        jxx::Ptr<jxx::lang::String> scheme_;
        jxx::Ptr<jxx::lang::String> schemeSpecificPart_;
        jxx::Ptr<jxx::lang::String> authority_;
        jxx::Ptr<jxx::lang::String> userInfo_;
        jxx::Ptr<jxx::lang::String> host_;
        jxx::lang::jint port_ = -1;
        jxx::Ptr<jxx::lang::String> path_;
        jxx::Ptr<jxx::lang::String> query_;
        jxx::Ptr<jxx::lang::String> fragment_;
        jxx::lang::jbool opaque_ = false;
    };
}
