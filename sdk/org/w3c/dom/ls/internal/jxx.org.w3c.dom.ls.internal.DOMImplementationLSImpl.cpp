#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"

#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSInputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSOutputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSParserImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSSerializerImpl.h"
#include "lang/jxx.lang.String.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"

namespace jxx::org::w3c::dom::ls::internal {

DOMImplementationLSImpl::DOMImplementationLSImpl()
    : Super() {
}

DOMImplementationLSImpl::~DOMImplementationLSImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParser>
DOMImplementationLSImpl::createLSParser(
    ::jxx::lang::jshort mode,
    const ::jxx::Ptr<::jxx::lang::String>& schemaType) {
    const bool schemaSupported =
        schemaType == nullptr ||
        schemaType->utf8() == "http://www.w3.org/2001/XMLSchema" ||
        schemaType->utf8() == "http://www.w3.org/TR/REC-xml";
    if (mode != ::jxx::org::w3c::dom::ls::DOMImplementationLS::MODE_SYNCHRONOUS ||
        !schemaSupported) {
        throw ::jxx::org::w3c::dom::DOMException(
            ::jxx::org::w3c::dom::DOMException::NOT_SUPPORTED_ERR,
            ::jxx::NEW<::jxx::lang::String>("Unsupported parser mode or schema type"));
    }
    return ::jxx::CAST<::jxx::org::w3c::dom::ls::LSParser>(
        ::jxx::NEW<LSParserImpl>());
}

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSSerializer>
DOMImplementationLSImpl::createLSSerializer() {
    return ::jxx::CAST<::jxx::org::w3c::dom::ls::LSSerializer>(
        ::jxx::NEW<LSSerializerImpl>());
}

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>
DOMImplementationLSImpl::createLSInput() {
    return ::jxx::CAST<::jxx::org::w3c::dom::ls::LSInput>(
        ::jxx::NEW<LSInputImpl>());
}

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSOutput>
DOMImplementationLSImpl::createLSOutput() {
    return ::jxx::CAST<::jxx::org::w3c::dom::ls::LSOutput>(
        ::jxx::NEW<LSOutputImpl>());
}

::jxx::Ptr<::jxx::org::w3c::dom::ls::DOMImplementationLS>
loadSaveImplementation() {
    static const auto instance = ::jxx::NEW<DOMImplementationLSImpl>();
    return ::jxx::CAST<::jxx::org::w3c::dom::ls::DOMImplementationLS>(instance);
}

} // namespace jxx::org::w3c::dom::ls::internal
