#pragma once

#include "lang/jxx_types.h"

namespace jxx::org::w3c::dom {
class Node;
}

namespace jxx::org::w3c::dom::traversal {

class NodeFilter {
public:
    virtual ~NodeFilter() = default;

    enum : ::jxx::lang::jshort {
        FILTER_ACCEPT = 1,
        FILTER_REJECT = 2,
        FILTER_SKIP = 3
    };

    enum : ::jxx::lang::jint {
        SHOW_ALL = -1,
        SHOW_ELEMENT = 0x00000001,
        SHOW_ATTRIBUTE = 0x00000002,
        SHOW_TEXT = 0x00000004,
        SHOW_CDATA_SECTION = 0x00000008,
        SHOW_ENTITY_REFERENCE = 0x00000010,
        SHOW_ENTITY = 0x00000020,
        SHOW_PROCESSING_INSTRUCTION = 0x00000040,
        SHOW_COMMENT = 0x00000080,
        SHOW_DOCUMENT = 0x00000100,
        SHOW_DOCUMENT_TYPE = 0x00000200,
        SHOW_DOCUMENT_FRAGMENT = 0x00000400,
        SHOW_NOTATION = 0x00000800
    };

    virtual ::jxx::lang::jshort acceptNode(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& node) = 0;
};

} // namespace jxx::org::w3c::dom::traversal
