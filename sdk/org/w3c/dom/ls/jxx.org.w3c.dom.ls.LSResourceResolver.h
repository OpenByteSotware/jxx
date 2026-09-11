#pragma once

#include "lang/jxx_types.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::w3c::dom::ls {

class LSInput;

class LSResourceResolver {
public:
    virtual ~LSResourceResolver() = default;

    virtual ::jxx::Ptr<LSInput> resolveResource(
        const ::jxx::Ptr<::jxx::lang::String>& type,
        const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
        const ::jxx::Ptr<::jxx::lang::String>& publicId,
        const ::jxx::Ptr<::jxx::lang::String>& systemId,
        const ::jxx::Ptr<::jxx::lang::String>& baseURI) = 0;
};

} // namespace jxx::org::w3c::dom::ls
