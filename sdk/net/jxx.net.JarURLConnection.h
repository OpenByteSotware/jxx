#pragma once

#include "net/jxx.net.URLConnection.h"

namespace jxx::net
{
    class URL;

    class JarURLConnection : public URLConnection
    {
    protected:
        explicit JarURLConnection(jxx::Ptr<URL> url);

    public:
        ~JarURLConnection() override = default;

    public:
        jxx::Ptr<URL> getJarFileURL() const;
        jxx::Ptr<jxx::lang::String> getEntryName() const;

        // Practical placeholders for later java.util.jar integration
        jxx::Ptr<jxx::lang::Object> getJarFile() const;
        jxx::Ptr<jxx::lang::Object> getManifest() const;
        jxx::Ptr<jxx::lang::Object> getJarEntry() const;
        jxx::Ptr<jxx::lang::Object> getAttributes() const;
        jxx::Ptr<jxx::lang::Object> getMainAttributes() const;
        jxx::Ptr<jxx::lang::Object> getCertificates() const;

    protected:
        jxx::Ptr<URL> jarFileURL_;
        jxx::Ptr<jxx::lang::String> entryName_;
    };
}
