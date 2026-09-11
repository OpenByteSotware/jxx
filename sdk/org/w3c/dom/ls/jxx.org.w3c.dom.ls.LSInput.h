#pragma once

#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::org::w3c::dom::ls {

class LSInput {
public:
    virtual ~LSInput() = default;

    virtual ::jxx::Ptr<::jxx::io::Reader> getCharacterStream() const = 0;
    virtual void setCharacterStream(const ::jxx::Ptr<::jxx::io::Reader>& characterStream) = 0;

    virtual ::jxx::Ptr<::jxx::io::InputStream> getByteStream() const = 0;
    virtual void setByteStream(const ::jxx::Ptr<::jxx::io::InputStream>& byteStream) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getStringData() const = 0;
    virtual void setStringData(const ::jxx::Ptr<::jxx::lang::String>& stringData) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getSystemId() const = 0;
    virtual void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getPublicId() const = 0;
    virtual void setPublicId(const ::jxx::Ptr<::jxx::lang::String>& publicId) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getBaseURI() const = 0;
    virtual void setBaseURI(const ::jxx::Ptr<::jxx::lang::String>& baseURI) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getEncoding() const = 0;
    virtual void setEncoding(const ::jxx::Ptr<::jxx::lang::String>& encoding) = 0;

    virtual ::jxx::lang::jbool getCertifiedText() const = 0;
    virtual void setCertifiedText(::jxx::lang::jbool certifiedText) = 0;
};

} // namespace jxx::org::w3c::dom::ls
