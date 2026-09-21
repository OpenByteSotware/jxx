#include "util/jxx.util.StringJoiner.h"

namespace jxx::util {

std::string StringJoiner::text_(
    const ::jxx::Ptr<::jxx::lang::CharSequence>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    return value->toString()->utf8();
}

StringJoiner::StringJoiner(
    const ::jxx::Ptr<::jxx::lang::CharSequence>& delimiter)
    : StringJoiner(
          delimiter,
          ::jxx::CAST<::jxx::lang::CharSequence>(
              ::jxx::NEW<::jxx::lang::String>("")),
          ::jxx::CAST<::jxx::lang::CharSequence>(
              ::jxx::NEW<::jxx::lang::String>(""))) {}

StringJoiner::StringJoiner(
    const ::jxx::Ptr<::jxx::lang::CharSequence>& delimiter,
    const ::jxx::Ptr<::jxx::lang::CharSequence>& prefix,
    const ::jxx::Ptr<::jxx::lang::CharSequence>& suffix)
    : Super(),
      prefix_(text_(prefix)),
      delimiter_(text_(delimiter)),
      suffix_(text_(suffix)),
      emptyValue_(prefix_ + suffix_) {}

::jxx::Ptr<StringJoiner> StringJoiner::setEmptyValue(
    const ::jxx::Ptr<::jxx::lang::CharSequence>& emptyValue) {
    emptyValue_ = text_(emptyValue);
    return ::jxx::CAST<StringJoiner>(thisPtr());
}

::jxx::Ptr<StringJoiner> StringJoiner::add(
    const ::jxx::Ptr<::jxx::lang::CharSequence>& newElement) {
    elements_.push_back(newElement == nullptr ? "null" : text_(newElement));
    return ::jxx::CAST<StringJoiner>(thisPtr());
}

::jxx::Ptr<StringJoiner> StringJoiner::merge(
    const ::jxx::Ptr<StringJoiner>& other) {
    if (other == nullptr) throw ::jxx::lang::NullPointerException();
    if (!other->elements_.empty()) elements_.push_back(other->joined_());
    return ::jxx::CAST<StringJoiner>(thisPtr());
}

std::string StringJoiner::joined_() const {
    std::string value;
    for (std::size_t index = 0; index < elements_.size(); ++index) {
        if (index != 0) value += delimiter_;
        value += elements_[index];
    }
    return value;
}

::jxx::lang::jint StringJoiner::length() const {
    return static_cast<::jxx::lang::jint>(
        elements_.empty() ? emptyValue_.size()
                          : prefix_.size() + joined_().size() + suffix_.size());
}

::jxx::Ptr<::jxx::lang::String> StringJoiner::toString() const {
    if (elements_.empty()) return ::jxx::NEW<::jxx::lang::String>(emptyValue_);
    return ::jxx::NEW<::jxx::lang::String>(prefix_ + joined_() + suffix_);
}

} // namespace jxx::util
