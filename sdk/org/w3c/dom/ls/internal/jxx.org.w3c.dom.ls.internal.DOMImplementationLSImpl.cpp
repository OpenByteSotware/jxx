#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.DOMImplementationLSImpl.h"

#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSInputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSOutputImpl.h"
#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSSerializerImpl.h"

namespace jxx::org::w3c::dom::ls::internal {

DOMImplementationLSImpl::DOMImplementationLSImpl()
    : Super() {
}

DOMImplementationLSImpl::~DOMImplementationLSImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParser>
DOMImplementationLSImpl::createLSParser(
    ::jxx::lang::jshort,
    const ::jxx::Ptr<::jxx::lang::String>&) {
    return nullptr;
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
