#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.LocatorImpl.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::org::xml::sax::helpers {

LocatorImpl::LocatorImpl() = default;

LocatorImpl::LocatorImpl(
    const ::jxx::Ptr<::jxx::org::xml::sax::Locator>& locator) {
    if (locator == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    publicId_ = locator->getPublicId();
    systemId_ = locator->getSystemId();
    lineNumber_ = locator->getLineNumber();
    columnNumber_ = locator->getColumnNumber();
}

LocatorImpl::~LocatorImpl() = default;

::jxx::Ptr<::jxx::lang::String> LocatorImpl::getPublicId() const {
    return publicId_;
}

::jxx::Ptr<::jxx::lang::String> LocatorImpl::getSystemId() const {
    return systemId_;
}

::jxx::lang::jint LocatorImpl::getLineNumber() const {
    return lineNumber_;
}

::jxx::lang::jint LocatorImpl::getColumnNumber() const {
    return columnNumber_;
}

void LocatorImpl::setPublicId(
    const ::jxx::Ptr<::jxx::lang::String>& publicId) {
    publicId_ = publicId;
}

void LocatorImpl::setSystemId(
    const ::jxx::Ptr<::jxx::lang::String>& systemId) {
    systemId_ = systemId;
}

void LocatorImpl::setLineNumber(::jxx::lang::jint lineNumber) {
    lineNumber_ = lineNumber;
}

void LocatorImpl::setColumnNumber(::jxx::lang::jint columnNumber) {
    columnNumber_ = columnNumber;
}

} // namespace jxx::org::xml::sax::helpers
