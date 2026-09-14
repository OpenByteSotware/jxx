#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx::org::w3c::dom::views {

class AbstractView;

class DocumentView {
public:
    virtual ~DocumentView() = default;

    virtual ::jxx::Ptr<AbstractView> getDefaultView() const = 0;
};

} // namespace jxx::org::w3c::dom::views
