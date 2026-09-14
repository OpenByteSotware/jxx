#pragma once

#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.EventImpl.h"
#include "org/w3c/dom/events/jxx.org.w3c.dom.events.MutationEvent.h"

namespace jxx::org::w3c::dom::events::internal {

class MutationEventImpl final
    : public EventImpl
    , public virtual ::jxx::org::w3c::dom::events::MutationEvent {
public:
    using JxxSuper = EventImpl;

    MutationEventImpl();
    ~MutationEventImpl() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::Node>
    getRelatedNode() const override;

    ::jxx::Ptr<::jxx::lang::String>
    getPrevValue() const override;

    ::jxx::Ptr<::jxx::lang::String>
    getNewValue() const override;

    ::jxx::Ptr<::jxx::lang::String>
    getAttrName() const override;

    ::jxx::lang::jshort
    getAttrChange() const override;

    void initMutationEvent(
        const ::jxx::Ptr<::jxx::lang::String>& typeArg,
        ::jxx::lang::jbool canBubbleArg,
        ::jxx::lang::jbool cancelableArg,
        const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& relatedNodeArg,
        const ::jxx::Ptr<::jxx::lang::String>& prevValueArg,
        const ::jxx::Ptr<::jxx::lang::String>& newValueArg,
        const ::jxx::Ptr<::jxx::lang::String>& attrNameArg,
        ::jxx::lang::jshort attrChangeArg) override;

private:
    ::jxx::Ptr<::jxx::org::w3c::dom::Node> relatedNode_;
    ::jxx::Ptr<::jxx::lang::String> prevValue_;
    ::jxx::Ptr<::jxx::lang::String> newValue_;
    ::jxx::Ptr<::jxx::lang::String> attrName_;
    ::jxx::lang::jshort attrChange_ = 0;
};

} // namespace jxx::org::w3c::dom::events::internal
