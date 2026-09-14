#pragma once

#include "org/w3c/dom/events/jxx.org.w3c.dom.events.Event.h"

namespace jxx::lang {
class String;
}

namespace jxx::org::w3c::dom {
class Node;
}

namespace jxx::org::w3c::dom::events {

class MutationEvent : public virtual Event {
public:
    static constexpr ::jxx::lang::jshort MODIFICATION = 1;
    static constexpr ::jxx::lang::jshort ADDITION = 2;
    static constexpr ::jxx::lang::jshort REMOVAL = 3;

    ~MutationEvent() override = default;

    virtual ::jxx::Ptr<::jxx::org::w3c::dom::Node>
    getRelatedNode() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getPrevValue() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getNewValue() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getAttrName() const = 0;

    virtual ::jxx::lang::jshort
    getAttrChange() const = 0;

    virtual void initMutationEvent(
        const ::jxx::Ptr<::jxx::lang::String>& typeArg,
        ::jxx::lang::jbool canBubbleArg,
        ::jxx::lang::jbool cancelableArg,
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& relatedNodeArg,
        const ::jxx::Ptr<::jxx::lang::String>& prevValueArg,
        const ::jxx::Ptr<::jxx::lang::String>& newValueArg,
        const ::jxx::Ptr<::jxx::lang::String>& attrNameArg,
        ::jxx::lang::jshort attrChangeArg) = 0;
};

} // namespace jxx::org::w3c::dom::events
