#pragma once

#include <map>

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.lang.buildin_array.h"

namespace jxx::lang
{
    class Class;
}

namespace jxx::io
{
    class InputStream;
    class OutputStream;
}

namespace jxx::net
{
    class URL;
    class FileNameMap;
    class ContentHandler;
    class ContentHandlerFactory;

    class URLConnection : public jxx::lang::Object
    {
    protected:
        explicit URLConnection(jxx::Ptr<URL> url);

    public:
        ~URLConnection() override = default;

    public:
        static jxx::Ptr<FileNameMap> getFileNameMap();
        static void setFileNameMap(jxx::Ptr<FileNameMap> map);
        static void setContentHandlerFactory(jxx::Ptr<ContentHandlerFactory> fac);

        static jxx::Ptr<jxx::lang::String> guessContentTypeFromName(jxx::Ptr<jxx::lang::String> fname);
        static jxx::Ptr<jxx::lang::String> guessContentTypeFromStream(jxx::Ptr<jxx::io::InputStream> is);

        virtual void connect() = 0;

        jxx::Ptr<URL> getURL() const;
        jxx::lang::jint getContentLength() const;
        jxx::lang::jlong getContentLengthLong() const;
        jxx::Ptr<jxx::lang::String> getContentType() const;
        jxx::Ptr<jxx::lang::String> getContentEncoding() const;
        jxx::lang::jlong getExpiration() const;
        jxx::lang::jlong getDate() const;
        jxx::lang::jlong getLastModified() const;

        jxx::Ptr<jxx::lang::String> getHeaderField(jxx::Ptr<jxx::lang::String> name) const;
        jxx::Ptr<jxx::lang::String> getHeaderField(jxx::lang::jint n) const;
        jxx::Ptr<jxx::lang::String> getHeaderFieldKey(jxx::lang::jint n) const;
        jxx::lang::jlong getHeaderFieldDate(jxx::Ptr<jxx::lang::String> name,
                                            jxx::lang::jlong defaultValue) const;
        jxx::lang::jint getHeaderFieldInt(jxx::Ptr<jxx::lang::String> name,
                                          jxx::lang::jint defaultValue) const;
        jxx::lang::jlong getHeaderFieldLong(jxx::Ptr<jxx::lang::String> name,
                                            jxx::lang::jlong defaultValue) const;

        jxx::Ptr<jxx::lang::Object> getContent();
        jxx::Ptr<jxx::lang::Object> getContent(jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::Class>, 1U>> classes);

        virtual jxx::Ptr<jxx::io::InputStream> getInputStream();
        virtual jxx::Ptr<jxx::io::OutputStream> getOutputStream();

        jxx::lang::jbool getDoInput() const noexcept;
        void setDoInput(jxx::lang::jbool doinput);
        jxx::lang::jbool getDoOutput() const noexcept;
        void setDoOutput(jxx::lang::jbool dooutput);
        jxx::lang::jbool getUseCaches() const noexcept;
        void setUseCaches(jxx::lang::jbool usecaches);
        jxx::lang::jbool getDefaultUseCaches() const noexcept;
        void setDefaultUseCaches(jxx::lang::jbool defaultusecaches);
        jxx::lang::jlong getIfModifiedSince() const noexcept;
        void setIfModifiedSince(jxx::lang::jlong ifmodifiedsince);
        jxx::lang::jbool getAllowUserInteraction() const noexcept;
        void setAllowUserInteraction(jxx::lang::jbool allowuserinteraction);
        jxx::lang::jint getConnectTimeout() const noexcept;
        void setConnectTimeout(jxx::lang::jint timeout);
        jxx::lang::jint getReadTimeout() const noexcept;
        void setReadTimeout(jxx::lang::jint timeout);

        void setRequestProperty(jxx::Ptr<jxx::lang::String> key,
                                jxx::Ptr<jxx::lang::String> value);
        void addRequestProperty(jxx::Ptr<jxx::lang::String> key,
                                jxx::Ptr<jxx::lang::String> value);
        jxx::Ptr<jxx::lang::String> getRequestProperty(jxx::Ptr<jxx::lang::String> key) const;

    protected:
        jxx::Ptr<ContentHandler> getContentHandler_();

    protected:
        jxx::Ptr<URL> url_;
        jxx::lang::jbool doInput_ = true;
        jxx::lang::jbool doOutput_ = false;
        jxx::lang::jbool useCaches_ = true;
        jxx::lang::jbool defaultUseCaches_ = true;
        jxx::lang::jbool allowUserInteraction_ = false;
        jxx::lang::jlong ifModifiedSince_ = 0;
        jxx::lang::jint connectTimeout_ = 0;
        jxx::lang::jint readTimeout_ = 0;
        jxx::lang::jbool connected_ = false;
        std::map<std::string, std::string> requestProps_;
        std::map<std::string, std::string> headerFields_;
    };
}
