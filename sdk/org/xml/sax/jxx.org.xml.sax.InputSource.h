#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::io {
class InputStream;
class Reader;
}

namespace jxx::lang {
class String;
}

namespace jxx::org::xml::sax {

class InputSource
    : public ::jxx::lang::ClassBase<
          InputSource,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<InputSource, JxxSuper>;

    InputSource();
    explicit InputSource(const ::jxx::Ptr<::jxx::io::InputStream>& byteStream);
    explicit InputSource(const ::jxx::Ptr<::jxx::io::Reader>& characterStream);
    explicit InputSource(const ::jxx::Ptr<::jxx::lang::String>& systemId);
    ~InputSource() override;

    ::jxx::Ptr<::jxx::io::InputStream> getByteStream() const;
    void setByteStream(const ::jxx::Ptr<::jxx::io::InputStream>& byteStream);

    ::jxx::Ptr<::jxx::io::Reader> getCharacterStream() const;
    void setCharacterStream(const ::jxx::Ptr<::jxx::io::Reader>& characterStream);

    ::jxx::Ptr<::jxx::lang::String> getEncoding() const;
    void setEncoding(const ::jxx::Ptr<::jxx::lang::String>& encoding);

    ::jxx::Ptr<::jxx::lang::String> getPublicId() const;
    void setPublicId(const ::jxx::Ptr<::jxx::lang::String>& publicId);

    ::jxx::Ptr<::jxx::lang::String> getSystemId() const;
    void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId);

private:
    ::jxx::Ptr<::jxx::io::InputStream> byteStream_;
    ::jxx::Ptr<::jxx::io::Reader> characterStream_;
    ::jxx::Ptr<::jxx::lang::String> encoding_;
    ::jxx::Ptr<::jxx::lang::String> publicId_;
    ::jxx::Ptr<::jxx::lang::String> systemId_;
};

} // namespace jxx::org::xml::sax
