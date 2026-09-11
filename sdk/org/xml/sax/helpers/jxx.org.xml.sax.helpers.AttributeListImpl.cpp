#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributeListImpl.h"

#include <cstddef>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::xml::sax::helpers {

namespace {

bool stringsEqual(
    const ::jxx::Ptr<::jxx::lang::String>& left,
    const ::jxx::Ptr<::jxx::lang::String>& right) {
    if (left == right) return true;
    if (!left || !right) return false;
    return left->equals(right);
}

} // namespace

AttributeListImpl::AttributeListImpl()
    : Super() {
}

AttributeListImpl::AttributeListImpl(
    const ::jxx::Ptr<::jxx::org::xml::sax::AttributeList>& attributes)
    : Super() {
    setAttributeList(attributes);
}

AttributeListImpl::~AttributeListImpl() = default;

::jxx::lang::jint AttributeListImpl::getLength() const {
    return static_cast<::jxx::lang::jint>(entries_.size());
}

const AttributeListImpl::Entry* AttributeListImpl::entryAt_(
    ::jxx::lang::jint index) const noexcept {
    if (index < 0 ||
        static_cast<std::size_t>(index) >= entries_.size()) {
        return nullptr;
    }
    return &entries_[static_cast<std::size_t>(index)];
}

::jxx::Ptr<::jxx::lang::String> AttributeListImpl::getName(
    ::jxx::lang::jint index) const {
    const auto entry = entryAt_(index);
    return entry == nullptr ? nullptr : entry->name;
}

::jxx::Ptr<::jxx::lang::String> AttributeListImpl::getType(
    ::jxx::lang::jint index) const {
    const auto entry = entryAt_(index);
    return entry == nullptr ? nullptr : entry->type;
}

::jxx::Ptr<::jxx::lang::String> AttributeListImpl::getValue(
    ::jxx::lang::jint index) const {
    const auto entry = entryAt_(index);
    return entry == nullptr ? nullptr : entry->value;
}

::jxx::Ptr<::jxx::lang::String> AttributeListImpl::getType(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    for (const auto& entry : entries_) {
        if (stringsEqual(entry.name, name)) return entry.type;
    }
    return nullptr;
}

::jxx::Ptr<::jxx::lang::String> AttributeListImpl::getValue(
    const ::jxx::Ptr<::jxx::lang::String>& name) const {
    for (const auto& entry : entries_) {
        if (stringsEqual(entry.name, name)) return entry.value;
    }
    return nullptr;
}

void AttributeListImpl::setAttributeList(
    const ::jxx::Ptr<::jxx::org::xml::sax::AttributeList>& attributes) {
    if (!attributes) throw ::jxx::lang::NullPointerException();
    entries_.clear();
    entries_.reserve(static_cast<std::size_t>(attributes->getLength()));
    for (::jxx::lang::jint index = 0;
         index < attributes->getLength();
         ++index) {
        addAttribute(
            attributes->getName(index),
            attributes->getType(index),
            attributes->getValue(index));
    }
}

void AttributeListImpl::addAttribute(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& type,
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    entries_.push_back(Entry{name, type, value});
}

void AttributeListImpl::removeAttribute(
    const ::jxx::Ptr<::jxx::lang::String>& name) {
    for (auto iterator = entries_.begin();
         iterator != entries_.end();
         ++iterator) {
        if (stringsEqual(iterator->name, name)) {
            entries_.erase(iterator);
            return;
        }
    }
}

void AttributeListImpl::clear() {
    entries_.clear();
}

} // namespace jxx::org::xml::sax::helpers
