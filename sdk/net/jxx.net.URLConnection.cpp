#include "net/jxx.net.URLConnection.h"

#include <stdexcept>

#include "net/internal/jxx.net.internal.UrlUtil.h"
#include "net/jxx.net.ContentHandler.h"
#include "net/jxx.net.ContentHandlerFactory.h"
#include "net/jxx.net.FileNameMap.h"
#include "net/jxx.net.URL.h"

namespace
{
    class SimpleFileNameMap final : public jxx::net::FileNameMap
    {
    public:
        jxx::Ptr<jxx::lang::String> getContentTypeFor(jxx::Ptr<jxx::lang::String> fileName) override
        {
            if (!fileName)
                return nullptr;
            return std::make_shared<jxx::lang::String>(jxx::net::internal::extensionToMime(fileName->utf8()));
        }
    };

    class StreamContentHandler final : public jxx::net::ContentHandler
    {
    public:
        jxx::Ptr<jxx::lang::Object> getContent(jxx::Ptr<jxx::net::URLConnection> urlc) override
        {
            return urlc ? urlc->getInputStream() : nullptr;
        }
    };

    jxx::Ptr<jxx::net::FileNameMap> g_fileNameMap = std::make_shared<SimpleFileNameMap>();
    jxx::Ptr<jxx::net::ContentHandlerFactory> g_contentHandlerFactory;
    jxx::Ptr<jxx::net::ContentHandler> g_defaultContentHandler = std::make_shared<StreamContentHandler>();
}

namespace jxx::net
{
    URLConnection::URLConnection(jxx::Ptr<URL> url)
        : url_(std::move(url))
    {
    }

    jxx::Ptr<FileNameMap> URLConnection::getFileNameMap() { return g_fileNameMap; }
    void URLConnection::setFileNameMap(jxx::Ptr<FileNameMap> map) { if (map) g_fileNameMap = map; }
    void URLConnection::setContentHandlerFactory(jxx::Ptr<ContentHandlerFactory> fac) { g_contentHandlerFactory = std::move(fac); }

    jxx::Ptr<jxx::lang::String> URLConnection::guessContentTypeFromName(jxx::Ptr<jxx::lang::String> fname)
    {
        return g_fileNameMap ? g_fileNameMap->getContentTypeFor(std::move(fname)) : nullptr;
    }

    jxx::Ptr<jxx::lang::String> URLConnection::guessContentTypeFromStream(jxx::Ptr<jxx::io::InputStream> /*is*/)
    {
        return nullptr;
    }

    jxx::Ptr<URL> URLConnection::getURL() const { return url_; }

    jxx::lang::jint URLConnection::getContentLength() const { return getHeaderFieldInt(std::make_shared<jxx::lang::String>("content-length"), -1); }
    jxx::lang::jlong URLConnection::getContentLengthLong() const { return getHeaderFieldLong(std::make_shared<jxx::lang::String>("content-length"), -1); }
    jxx::Ptr<jxx::lang::String> URLConnection::getContentType() const { return getHeaderField(std::make_shared<jxx::lang::String>("content-type")); }
    jxx::Ptr<jxx::lang::String> URLConnection::getContentEncoding() const { return getHeaderField(std::make_shared<jxx::lang::String>("content-encoding")); }
    jxx::lang::jlong URLConnection::getExpiration() const { return 0; }
    jxx::lang::jlong URLConnection::getDate() const { return 0; }
    jxx::lang::jlong URLConnection::getLastModified() const { return 0; }

    jxx::Ptr<jxx::lang::String> URLConnection::getHeaderField(jxx::Ptr<jxx::lang::String> name) const
    {
        if (!name)
            return nullptr;
        const auto it = headerFields_.find(name->utf8());
        if (it == headerFields_.end())
            return nullptr;
        return std::make_shared<jxx::lang::String>(it->second);
    }

    jxx::Ptr<jxx::lang::String> URLConnection::getHeaderField(jxx::lang::jint n) const
    {
        if (n < 0 || static_cast<std::size_t>(n) >= headerFields_.size())
            return nullptr;
        auto it = headerFields_.begin();
        std::advance(it, n);
        return std::make_shared<jxx::lang::String>(it->second);
    }

    jxx::Ptr<jxx::lang::String> URLConnection::getHeaderFieldKey(jxx::lang::jint n) const
    {
        if (n < 0 || static_cast<std::size_t>(n) >= headerFields_.size())
            return nullptr;
        auto it = headerFields_.begin();
        std::advance(it, n);
        return std::make_shared<jxx::lang::String>(it->first);
    }

    jxx::lang::jlong URLConnection::getHeaderFieldDate(jxx::Ptr<jxx::lang::String> name,
                                                       jxx::lang::jlong defaultValue) const
    {
        (void)name;
        return defaultValue;
    }

    jxx::lang::jint URLConnection::getHeaderFieldInt(jxx::Ptr<jxx::lang::String> name,
                                                     jxx::lang::jint defaultValue) const
    {
        auto v = getHeaderField(std::move(name));
        if (!v)
            return defaultValue;
        try { return std::stoi(v->utf8()); } catch (...) { return defaultValue; }
    }

    jxx::lang::jlong URLConnection::getHeaderFieldLong(jxx::Ptr<jxx::lang::String> name,
                                                       jxx::lang::jlong defaultValue) const
    {
        auto v = getHeaderField(std::move(name));
        if (!v)
            return defaultValue;
        try { return std::stoll(v->utf8()); } catch (...) { return defaultValue; }
    }

    jxx::Ptr<ContentHandler> URLConnection::getContentHandler_()
    {
        if (g_contentHandlerFactory)
        {
            auto type = getContentType();
            if (type)
            {
                auto h = g_contentHandlerFactory->createContentHandler(type);
                if (h)
                    return h;
            }
        }
        return g_defaultContentHandler;
    }

    jxx::Ptr<jxx::lang::Object> URLConnection::getContent()
    {
        auto h = getContentHandler_();
        return h ? h->getContent(std::static_pointer_cast<URLConnection>(shared_from_this())) : nullptr;
    }

    jxx::Ptr<jxx::lang::Object> URLConnection::getContent(jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::Class>, 1U>> classes)
    {
        auto h = getContentHandler_();
        return h ? h->getContent(std::static_pointer_cast<URLConnection>(shared_from_this()), std::move(classes)) : nullptr;
    }

    jxx::Ptr<jxx::io::InputStream> URLConnection::getInputStream()
    {
        throw std::runtime_error("getInputStream not implemented for this URLConnection");
    }

    jxx::Ptr<jxx::io::OutputStream> URLConnection::getOutputStream()
    {
        throw std::runtime_error("getOutputStream not implemented for this URLConnection");
    }

    jxx::lang::jbool URLConnection::getDoInput() const noexcept { return doInput_; }
    void URLConnection::setDoInput(jxx::lang::jbool x) { doInput_ = x; }
    jxx::lang::jbool URLConnection::getDoOutput() const noexcept { return doOutput_; }
    void URLConnection::setDoOutput(jxx::lang::jbool x) { doOutput_ = x; }
    jxx::lang::jbool URLConnection::getUseCaches() const noexcept { return useCaches_; }
    void URLConnection::setUseCaches(jxx::lang::jbool x) { useCaches_ = x; }
    jxx::lang::jbool URLConnection::getDefaultUseCaches() const noexcept { return defaultUseCaches_; }
    void URLConnection::setDefaultUseCaches(jxx::lang::jbool x) { defaultUseCaches_ = x; }
    jxx::lang::jlong URLConnection::getIfModifiedSince() const noexcept { return ifModifiedSince_; }
    void URLConnection::setIfModifiedSince(jxx::lang::jlong x) { ifModifiedSince_ = x; }
    jxx::lang::jbool URLConnection::getAllowUserInteraction() const noexcept { return allowUserInteraction_; }
    void URLConnection::setAllowUserInteraction(jxx::lang::jbool x) { allowUserInteraction_ = x; }
    jxx::lang::jint URLConnection::getConnectTimeout() const noexcept { return connectTimeout_; }
    void URLConnection::setConnectTimeout(jxx::lang::jint x) { connectTimeout_ = x; }
    jxx::lang::jint URLConnection::getReadTimeout() const noexcept { return readTimeout_; }
    void URLConnection::setReadTimeout(jxx::lang::jint x) { readTimeout_ = x; }

    void URLConnection::setRequestProperty(jxx::Ptr<jxx::lang::String> key,
                                           jxx::Ptr<jxx::lang::String> value)
    {
        if (!key)
            throw std::invalid_argument("null key");
        requestProps_[key->utf8()] = value ? value->utf8() : std::string();
    }

    void URLConnection::addRequestProperty(jxx::Ptr<jxx::lang::String> key,
                                           jxx::Ptr<jxx::lang::String> value)
    {
        setRequestProperty(std::move(key), std::move(value));
    }

    jxx::Ptr<jxx::lang::String> URLConnection::getRequestProperty(jxx::Ptr<jxx::lang::String> key) const
    {
        if (!key)
            return nullptr;
        const auto it = requestProps_.find(key->utf8());
        return (it == requestProps_.end()) ? nullptr : std::make_shared<jxx::lang::String>(it->second);
    }
}
