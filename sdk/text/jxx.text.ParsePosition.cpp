#include "text/jxx.text.ParsePosition.h"

#include <string>

#include "lang/jxx.lang.String.h"

namespace jxx::text {

ParsePosition::ParsePosition(::jxx::lang::jint index)
    : index_(index) {}

::jxx::lang::jint ParsePosition::getIndex() const noexcept { return index_; }
void ParsePosition::setIndex(::jxx::lang::jint index) noexcept { index_ = index; }
::jxx::lang::jint ParsePosition::getErrorIndex() const noexcept { return errorIndex_; }
void ParsePosition::setErrorIndex(::jxx::lang::jint value) noexcept { errorIndex_ = value; }

::jxx::lang::jbool ParsePosition::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    const auto position = ::jxx::CAST<ParsePosition>(other);
    return position != nullptr && index_ == position->index_ &&
           errorIndex_ == position->errorIndex_;
}

::jxx::lang::jint ParsePosition::hashCode() const {
    return (errorIndex_ << 16) | (index_ & 0xffff);
}

::jxx::Ptr<::jxx::lang::String> ParsePosition::toString() const {
    return ::jxx::NEW<::jxx::lang::String>(
        "jxx.text.ParsePosition[index=" + std::to_string(index_) +
        ",errorIndex=" + std::to_string(errorIndex_) + "]");
}

::jxx::Ptr<::jxx::lang::Object> ParsePosition::cloneImpl() const {
    auto copy = ::jxx::NEW<ParsePosition>(index_);
    copy->setErrorIndex(errorIndex_);
    return ::jxx::CAST<::jxx::lang::Object>(copy);
}

} // namespace jxx::text
