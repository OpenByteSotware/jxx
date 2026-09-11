#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Locator2Impl.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::org::xml::sax::ext {

Locator2Impl::Locator2Impl()
    : Super() {
}

Locator2Impl::Locator2Impl(
    const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator)
    : Super(locator) {
    if (locator == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto extended = ::jxx::CAST<Locator2>(locator);
    if (extended != nullptr) {
        version_ = extended->getXMLVersion();
        encoding_ = extended->getEncoding();
    }
}

Locator2Impl::~Locator2Impl() = default;

::jxx::Ptr<::jxx::lang::String>
Locator2Impl::getXMLVersion() const {
    return version_;
}

::jxx::Ptr<::jxx::lang::String>
Locator2Impl::getEncoding() const {
    return encoding_;
}

void Locator2Impl::setXMLVersion(
    const ::jxx::Ptr<::jxx::lang::String>& version) {
    version_ = version;
}

void Locator2Impl::setEncoding(
    const ::jxx::Ptr<::jxx::lang::String>& encoding) {
    encoding_ = encoding;
}

} // namespace jxx::org::xml::sax::ext
