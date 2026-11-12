#pragma once

#include "ext/xml/transform/jxx.ext.xml.transform.Source.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::transform::stream {

class StreamSource final
    : public ::jxx::lang::ClassBase<
          StreamSource,
          ::jxx::lang::Object,
          ::jxx::ext::xml::transform::Source> {
public:
    StreamSource();
    explicit StreamSource(const ::jxx::Ptr<::jxx::io::InputStream>& inputStream);
    StreamSource(const ::jxx::Ptr<::jxx::io::InputStream>& inputStream, const ::jxx::Ptr<::jxx::lang::String>& systemId);
    explicit StreamSource(const ::jxx::Ptr<::jxx::io::Reader>& reader);
    StreamSource(const ::jxx::Ptr<::jxx::io::Reader>& reader, const ::jxx::Ptr<::jxx::lang::String>& systemId);
    explicit StreamSource(const ::jxx::Ptr<::jxx::lang::String>& systemId);
    explicit StreamSource(const ::jxx::Ptr<::jxx::io::File>& file);

    void setInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& inputStream);
    ::jxx::Ptr<::jxx::io::InputStream> getInputStream() const;
    void setReader(const ::jxx::Ptr<::jxx::io::Reader>& reader);
    ::jxx::Ptr<::jxx::io::Reader> getReader() const;
    void setPublicId(const ::jxx::Ptr<::jxx::lang::String>& publicId);
    ::jxx::Ptr<::jxx::lang::String> getPublicId() const;
    void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId) override;
    ::jxx::Ptr<::jxx::lang::String> getSystemId() const override;
    void setSystemId(const ::jxx::Ptr<::jxx::io::File>& file);
    ::jxx::lang::jbool isEmpty() const;
    static ::jxx::Ptr<::jxx::lang::String> FEATURE();

private:
    ::jxx::Ptr<::jxx::io::InputStream> inputStream_;
    ::jxx::Ptr<::jxx::io::Reader> reader_;
    ::jxx::Ptr<::jxx::lang::String> publicId_;
    ::jxx::Ptr<::jxx::lang::String> systemId_;
};

} // namespace jxx::ext::xml::transform::stream
