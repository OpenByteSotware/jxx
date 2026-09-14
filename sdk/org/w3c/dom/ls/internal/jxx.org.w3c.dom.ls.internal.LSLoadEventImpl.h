#pragma once

#include "org/w3c/dom/events/internal/jxx.org.w3c.dom.events.internal.EventImpl.h"
#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSLoadEvent.h"

namespace jxx::org::w3c::dom::ls::internal {

class LSLoadEventImpl final
    : public ::jxx::org::w3c::dom::events::internal::EventImpl
    , public virtual ::jxx::org::w3c::dom::ls::LSLoadEvent {
public:
    using JxxSuper =
        ::jxx::org::w3c::dom::events::internal::EventImpl;

    LSLoadEventImpl(
        const ::jxx::Ptr<::jxx::org::w3c::dom::Document>& newDocument,
        const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>& input);

    ~LSLoadEventImpl() override;

    ::jxx::Ptr<::jxx::org::w3c::dom::Document>
    getNewDocument() const override;

    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>
    getInput() const override;

private:
    ::jxx::Ptr<::jxx::org::w3c::dom::Document> newDocument_;
    ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput> input_;
};

} // namespace jxx::org::w3c::dom::ls::internal
