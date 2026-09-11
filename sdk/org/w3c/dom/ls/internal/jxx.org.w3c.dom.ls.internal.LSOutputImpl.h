#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSOutput.h"

namespace jxx::org::w3c::dom::ls::internal {

class LSOutputImpl final
    : public ::jxx::lang::ClassBase<
          LSOutputImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::ls::LSOutput> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        LSOutputImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::ls::LSOutput>;

    LSOutputImpl();
    ~LSOutputImpl() override;

    ::jxx::Ptr<::jxx::io::Writer> getCharacterStream() const override;
    void setCharacterStream(const ::jxx::Ptr<::jxx::io::Writer>& value) override;
    ::jxx::Ptr<::jxx::io::OutputStream> getByteStream() const override;
    void setByteStream(const ::jxx::Ptr<::jxx::io::OutputStream>& value) override;
    ::jxx::Ptr<::jxx::lang::String> getSystemId() const override;
    void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    ::jxx::Ptr<::jxx::lang::String> getEncoding() const override;
    void setEncoding(const ::jxx::Ptr<::jxx::lang::String>& value) override;

private:
    ::jxx::Ptr<::jxx::io::Writer> characterStream_;
    ::jxx::Ptr<::jxx::io::OutputStream> byteStream_;
    ::jxx::Ptr<::jxx::lang::String> systemId_;
    ::jxx::Ptr<::jxx::lang::String> encoding_;
};

} // namespace jxx::org::w3c::dom::ls::internal
