#pragma once

#include "lang/jxx_types.h"

namespace jxx::org::w3c::dom {
class Element;
class Node;
}

namespace jxx::org::w3c::dom::ls {

class LSParserFilter {
public:
    virtual ~LSParserFilter() = default;

    enum : ::jxx::lang::jshort {
        FILTER_ACCEPT = 1,
        FILTER_REJECT = 2,
        FILTER_SKIP = 3,
        FILTER_INTERRUPT = 4
    };

    virtual ::jxx::lang::jshort startElement(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Element>& elementArg) = 0;

    virtual ::jxx::lang::jshort acceptNode(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& nodeArg) = 0;

    virtual ::jxx::lang::jint getWhatToShow() const = 0;
};

} // namespace jxx::org::w3c::dom::ls
