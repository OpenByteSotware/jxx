#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSInput.h"

namespace jxx::org::w3c::dom::ls::internal {

class LSInputImpl final
    : public ::jxx::lang::ClassBase<
          LSInputImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::ls::LSInput> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        LSInputImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::ls::LSInput>;

    LSInputImpl();
    ~LSInputImpl() override;

    ::jxx::Ptr<::jxx::io::Reader> getCharacterStream() const override;
    void setCharacterStream(const ::jxx::Ptr<::jxx::io::Reader>& value) override;
    ::jxx::Ptr<::jxx::io::InputStream> getByteStream() const override;
    void setByteStream(const ::jxx::Ptr<::jxx::io::InputStream>& value) override;
    ::jxx::Ptr<::jxx::lang::String> getStringData() const override;
    void setStringData(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    ::jxx::Ptr<::jxx::lang::String> getSystemId() const override;
    void setSystemId(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    ::jxx::Ptr<::jxx::lang::String> getPublicId() const override;
    void setPublicId(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    ::jxx::Ptr<::jxx::lang::String> getBaseURI() const override;
    void setBaseURI(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    ::jxx::Ptr<::jxx::lang::String> getEncoding() const override;
    void setEncoding(const ::jxx::Ptr<::jxx::lang::String>& value) override;
    ::jxx::lang::jbool getCertifiedText() const override;
    void setCertifiedText(::jxx::lang::jbool value) override;

private:
    ::jxx::Ptr<::jxx::io::Reader> characterStream_;
    ::jxx::Ptr<::jxx::io::InputStream> byteStream_;
    ::jxx::Ptr<::jxx::lang::String> stringData_;
    ::jxx::Ptr<::jxx::lang::String> systemId_;
    ::jxx::Ptr<::jxx::lang::String> publicId_;
    ::jxx::Ptr<::jxx::lang::String> baseURI_;
    ::jxx::Ptr<::jxx::lang::String> encoding_;
    ::jxx::lang::jbool certifiedText_;
};

} // namespace jxx::org::w3c::dom::ls::internal
