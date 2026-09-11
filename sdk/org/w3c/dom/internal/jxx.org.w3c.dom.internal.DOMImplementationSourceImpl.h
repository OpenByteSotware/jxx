#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "org/w3c/dom/jxx.org.w3c.dom.DOMImplementationSource.h"

namespace jxx::org::w3c::dom::internal {

class DOMImplementationSourceImpl final
    : public ::jxx::lang::ClassBase<
          DOMImplementationSourceImpl,
          ::jxx::lang::Object,
          ::jxx::org::w3c::dom::DOMImplementationSource> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        DOMImplementationSourceImpl,
        JxxSuper,
        ::jxx::org::w3c::dom::DOMImplementationSource>;

    DOMImplementationSourceImpl();
    ~DOMImplementationSourceImpl() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>
    getDOMImplementation(
        const ::jxx::Ptr<::jxx::lang::String>& features) override;

    ::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementationList>
    getDOMImplementationList(
        const ::jxx::Ptr<::jxx::lang::String>& features) override;
};

::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementationSource>
implementationSource();

} // namespace jxx::org::w3c::dom::internal
