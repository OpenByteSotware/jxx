#pragma once

#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"

namespace jxx::org::w3c::dom::ls {
class LSInput;

class LSProgressEvent : public ::jxx::org::w3c::dom::events::Event {
public:
    ~LSProgressEvent() override = default;
    virtual ::jxx::Ptr<LSInput> getInput() const = 0;
    virtual ::jxx::lang::jint getPosition() const = 0;
    virtual ::jxx::lang::jint getTotalSize() const = 0;
};

} // namespace jxx::org::w3c::dom::ls
