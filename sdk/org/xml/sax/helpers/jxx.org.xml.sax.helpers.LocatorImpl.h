#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "org/xml/sax/jxx.org.xml.sax.Locator.h"

namespace jxx::org::xml::sax::helpers {

class LocatorImpl
    : public ::jxx::lang::ClassBase<
          LocatorImpl,
          ::jxx::lang::Object,
          ::jxx::org::xml::sax::Locator> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        LocatorImpl,
        JxxSuper,
        ::jxx::org::xml::sax::Locator>;

    LocatorImpl();

    explicit LocatorImpl(
        const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator);

    ~LocatorImpl() override;

    ::jxx::Ptr<::jxx::lang::String> getPublicId() const override;
    ::jxx::Ptr<::jxx::lang::String> getSystemId() const override;
    ::jxx::lang::jint getLineNumber() const override;
    ::jxx::lang::jint getColumnNumber() const override;

    void setPublicId(
        const ::jxx::Ptr<::jxx::lang::String>& publicId);

    void setSystemId(
        const ::jxx::Ptr<::jxx::lang::String>& systemId);

    void setLineNumber(::jxx::lang::jint lineNumber);
    void setColumnNumber(::jxx::lang::jint columnNumber);

private:
    ::jxx::Ptr<::jxx::lang::String> publicId_;
    ::jxx::Ptr<::jxx::lang::String> systemId_;
    ::jxx::lang::jint lineNumber_ = 0;
    ::jxx::lang::jint columnNumber_ = 0;
};

} // namespace jxx::org::xml::sax::helpers
