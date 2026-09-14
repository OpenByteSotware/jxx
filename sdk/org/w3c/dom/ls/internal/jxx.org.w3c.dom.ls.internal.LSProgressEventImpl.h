#pragma once

#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.EventImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSProgressEvent.h"

namespace jxx::org::w3c::dom::ls::internal {

class LSProgressEventImpl final
    : public ::jxx::org::w3c::dom::events::internal::EventImpl
    , public virtual ::jxx::org::w3c::dom::ls::LSProgressEvent {
public:
    using JxxSuper =
        ::jxx::org::w3c::dom::events::internal::EventImpl;

    LSProgressEventImpl(
        const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>& input,
        ::jxx::lang::jint position,
        ::jxx::lang::jint totalSize);

    ~LSProgressEventImpl() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>
    getInput() const override;

    ::jxx::lang::jint getPosition() const override;
    ::jxx::lang::jint getTotalSize() const override;

private:
    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput> input_;
    ::jxx::lang::jint position_;
    ::jxx::lang::jint totalSize_;
};

} // namespace jxx::org::w3c::dom::ls::internal
