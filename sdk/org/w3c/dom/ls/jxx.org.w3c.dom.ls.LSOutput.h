#pragma once

#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::w3c::dom::ls {

class LSOutput {
public:
    virtual ~LSOutput() = default;

    virtual ::jxx::Ptr<::jxx::io::Writer> getCharacterStream() const = 0;
    virtual void setCharacterStream(const ::jxx::Ptr<::jxx::io::Writer>& characterStream) = 0;

    virtual ::jxx::Ptr<::jxx::io::OutputStream> getByteStream() const = 0;
    virtual void setByteStream(const ::jxx::Ptr<::jxx::io::OutputStream>& byteStream) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getSystemId() const = 0;
    virtual void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId) = 0;

    virtual ::jxx::Ptr<::jxx::lang::String> getEncoding() const = 0;
    virtual void setEncoding(const ::jxx::Ptr<::jxx::lang::String>& encoding) = 0;
};

} // namespace jxx::org::w3c::dom::ls
