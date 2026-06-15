#include "net/jxx.net.JarURLConnection.h"

#include <stdexcept>

#include "net/jxx.net.URL.h"

namespace jxx::net
{
    JarURLConnection::JarURLConnection(jxx::Ptr<URL> url)
        : URLConnection(std::move(url))
    {
    }

    jxx::Ptr<URL> JarURLConnection::getJarFileURL() const { return jarFileURL_; }
    jxx::Ptr<jxx::lang::String> JarURLConnection::getEntryName() const { return entryName_; }
    jxx::Ptr<jxx::lang::Object> JarURLConnection::getJarFile() const { return nullptr; }
    jxx::Ptr<jxx::lang::Object> JarURLConnection::getManifest() const { return nullptr; }
    jxx::Ptr<jxx::lang::Object> JarURLConnection::getJarEntry() const { return nullptr; }
    jxx::Ptr<jxx::lang::Object> JarURLConnection::getAttributes() const { return nullptr; }
    jxx::Ptr<jxx::lang::Object> JarURLConnection::getMainAttributes() const { return nullptr; }
    jxx::Ptr<jxx::lang::Object> JarURLConnection::getCertificates() const { return nullptr; }
}
