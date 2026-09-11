#pragma once

#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2.h"
#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.LocatorImpl.h"

namespace jxx::org::xml::sax::ext {

class Locator2Impl
    : public ::jxx::lang::ClassBase<
          Locator2Impl,
          ::jxx::org::xml::sax::helpers::LocatorImpl,
          Locator2> {
public:
    using JxxSuper =
        ::jxx::org::xml::sax::helpers::LocatorImpl;

    using Super = ::jxx::lang::ClassBase<
        Locator2Impl,
        JxxSuper,
        Locator2>;

    Locator2Impl();

    explicit Locator2Impl(
        const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator);

    ~Locator2Impl() override;

    ::jxx::Ptr<::jxx::lang::String>
    getXMLVersion() const override;

    ::jxx::Ptr<::jxx::lang::String>
    getEncoding() const override;

    void setXMLVersion(
        const ::jxx::Ptr<::jxx::lang::String>& version);

    void setEncoding(
        const ::jxx::Ptr<::jxx::lang::String>& encoding);

private:
    ::jxx::Ptr<::jxx::lang::String> version_;
    ::jxx::Ptr<::jxx::lang::String> encoding_;
};

} // namespace jxx::org::xml::sax::ext
