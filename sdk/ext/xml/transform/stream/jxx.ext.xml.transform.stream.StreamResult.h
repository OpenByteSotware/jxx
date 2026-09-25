#pragma once
#include "ext/xml/transform/jxx.ext.xml.transform.Result.h"
#include "io/jxx.io.File.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::ext::xml::transform::stream {
class StreamResult final : public ::jxx::lang::ClassBase<StreamResult, ::jxx::lang::Object, ::jxx::ext::xml::transform::Result> {
public:
    StreamResult();
    explicit StreamResult(const ::jxx::Ptr<::jxx::io::OutputStream>& outputStream);
    explicit StreamResult(const ::jxx::Ptr<::jxx::io::Writer>& writer);
    explicit StreamResult(const ::jxx::Ptr<::jxx::lang::String>& systemId);
    explicit StreamResult(const ::jxx::Ptr<::jxx::io::File>& file);
    void setOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& outputStream);
    ::jxx::Ptr<::jxx::io::OutputStream> getOutputStream() const;
    void setWriter(const ::jxx::Ptr<::jxx::io::Writer>& writer);
    ::jxx::Ptr<::jxx::io::Writer> getWriter() const;
    void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& systemId) override;
    void setSystemId(const ::jxx::Ptr<::jxx::io::File>& file);
    ::jxx::Ptr<::jxx::lang::String> getSystemId() const override;
    static ::jxx::Ptr<::jxx::lang::String> FEATURE();
private:
    ::jxx::Ptr<::jxx::io::OutputStream> outputStream_;
    ::jxx::Ptr<::jxx::io::Writer> writer_;
    ::jxx::Ptr<::jxx::lang::String> systemId_;
};
}
