#pragma once

#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"

namespace jxx::org::w3c::dom {
class Document;
}

namespace jxx::org::w3c::dom::ls {
class LSInput;

class LSLoadEvent : public virtual ::jxx::org::w3c::dom::events::Event {
public:
    ~LSLoadEvent() override = default;
    virtual ::jxx::Ptr<::jxx::org::w3c::dom::Document> getNewDocument() const = 0;
    virtual ::jxx::Ptr<LSInput> getInput() const = 0;
};

} // namespace jxx::org::w3c::dom::ls
