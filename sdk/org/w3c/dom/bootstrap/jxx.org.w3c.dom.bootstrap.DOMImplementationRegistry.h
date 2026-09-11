#pragma once

#include <memory>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::w3c::dom {
class DOMImplementation;
class DOMImplementationList;
class DOMImplementationSource;
}

namespace jxx::org::w3c::dom::bootstrap {

class DOMImplementationRegistry final
    : public ::jxx::lang::ClassBase<
          DOMImplementationRegistry,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        DOMImplementationRegistry,
        JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::String> PROPERTY;

    static ::jxx::Ptr<DOMImplementationRegistry> newInstance();

    ~DOMImplementationRegistry() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementation>
    getDOMImplementation(
        const ::jxx::Ptr<::jxx::lang::String>& features);

    ::jxx::Ptr<::jxx::org::w3c::dom::DOMImplementationList>
    getDOMImplementationList(
        const ::jxx::Ptr<::jxx::lang::String>& features);

    void addSource(
        const ::jxx::Ptr<
            ::jxx::org::w3c::dom::DOMImplementationSource>& source);

private:
    struct Impl;

    DOMImplementationRegistry();

    std::shared_ptr<Impl> impl_;
};

} // namespace jxx::org::w3c::dom::bootstrap
