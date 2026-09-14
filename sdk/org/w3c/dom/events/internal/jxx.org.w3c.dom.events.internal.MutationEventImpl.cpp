#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.MutationEventImpl.h"

namespace jxx::org::w3c::dom::events::internal {

MutationEventImpl::MutationEventImpl() = default;
MutationEventImpl::~MutationEventImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::Node>
MutationEventImpl::getRelatedNode() const {
    return relatedNode_;
}

::jxx::Ptr<::jxx::lang::String>
MutationEventImpl::getPrevValue() const {
    return prevValue_;
}

::jxx::Ptr<::jxx::lang::String>
MutationEventImpl::getNewValue() const {
    return newValue_;
}

::jxx::Ptr<::jxx::lang::String>
MutationEventImpl::getAttrName() const {
    return attrName_;
}

::jxx::lang::jshort
MutationEventImpl::getAttrChange() const {
    return attrChange_;
}

void MutationEventImpl::initMutationEvent(
    const ::jxx::Ptr<::jxx::lang::String>& typeArg,
    ::jxx::lang::jbool canBubbleArg,
    ::jxx::lang::jbool cancelableArg,
    const ::jxx::Ptr<::jxx::org::w3c::dom::Node>& relatedNodeArg,
    const ::jxx::Ptr<::jxx::lang::String>& prevValueArg,
    const ::jxx::Ptr<::jxx::lang::String>& newValueArg,
    const ::jxx::Ptr<::jxx::lang::String>& attrNameArg,
    ::jxx::lang::jshort attrChangeArg) {
    initEvent(typeArg, canBubbleArg, cancelableArg);
    relatedNode_ = relatedNodeArg;
    prevValue_ = prevValueArg;
    newValue_ = newValueArg;
    attrName_ = attrNameArg;
    attrChange_ = attrChangeArg;
}

} // namespace jxx::org::w3c::dom::events::internal
