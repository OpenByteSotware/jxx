#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx::org::w3c::dom::views {

class DocumentView;

class AbstractView {
public:
    virtual ~AbstractView() = default;

    virtual ::jxx::Ptr<DocumentView> getDocument() const = 0;
};

} // namespace jxx::org::w3c::dom::views
