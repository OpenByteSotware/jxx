#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSLoadEventImpl.h"

#include "lang/jxx.lang.String.h"

namespace jxx::org::w3c::dom::ls::internal {

LSLoadEventImpl::LSLoadEventImpl(
    const ::jxx::Ptr<::jxx::org::w3c::dom::Document>& newDocument,
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>& input)
    : JxxSuper()
    , newDocument_(newDocument)
    , input_(input) {
    JxxSuper::initEvent(
        ::jxx::NEW<::jxx::lang::String>("load"),
        false,
        false);
}

LSLoadEventImpl::~LSLoadEventImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::Document>
LSLoadEventImpl::getNewDocument() const {
    return newDocument_;
}

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>
LSLoadEventImpl::getInput() const {
    return input_;
}

} // namespace jxx::org::w3c::dom::ls::internal
