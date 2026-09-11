#pragma once

#include "org/w3c/dom/traversal/jxx.org.w3c.dom.traversal.NodeFilter.h"

namespace jxx::org::w3c::dom::ls {

class LSSerializerFilter
    : public ::jxx::org::w3c::dom::traversal::NodeFilter {
public:
    ~LSSerializerFilter() override = default;
    virtual ::jxx::lang::jint getWhatToShow() const = 0;
};

} // namespace jxx::org::w3c::dom::ls
