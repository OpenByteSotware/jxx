#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.DOMImplementationLS.h"

namespace jxx::org::w3c::dom::ls::internal {

class DOMImplementationLSImpl final
    : public ::jxx::lang::ClassBase<
          DOMImplementationLSImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::ls::DOMImplementationLS> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        DOMImplementationLSImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::ls::DOMImplementationLS>;

    DOMImplementationLSImpl();
    ~DOMImplementationLSImpl() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSParser> createLSParser(
        ::jxx::lang::jshort mode,
        const ::jxx::Ptr<::jxx::lang::String>& schemaType) override;

    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSSerializer>
    createLSSerializer() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>
    createLSInput() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSOutput>
    createLSOutput() override;
};

::jxx::Ptr<::jxx::org::w3c::dom::ls::DOMImplementationLS>
loadSaveImplementation();

} // namespace jxx::org::w3c::dom::ls::internal
