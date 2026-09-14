#include "org/w3c/dom/ls/internal/jxx.org.w3c.dom.ls.internal.LSProgressEventImpl.h"

#include "lang/jxx.lang.String.h"

namespace jxx::org::w3c::dom::ls::internal {

LSProgressEventImpl::LSProgressEventImpl(
    const ::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>& input,
    ::jxx::lang::jint position,
    ::jxx::lang::jint totalSize)
    : JxxSuper()
    , input_(input)
    , position_(position)
    , totalSize_(totalSize) {
    JxxSuper::initEvent(
        ::jxx::NEW<::jxx::lang::String>("progress"),
        false,
        false);
}

LSProgressEventImpl::~LSProgressEventImpl() = default;

::jxx::Ptr<::jxx::org::w3c::dom::ls::LSInput>
LSProgressEventImpl::getInput() const {
    return input_;
}

::jxx::lang::jint LSProgressEventImpl::getPosition() const {
    return position_;
}

::jxx::lang::jint LSProgressEventImpl::getTotalSize() const {
    return totalSize_;
}

} // namespace jxx::org::w3c::dom::ls::internal
