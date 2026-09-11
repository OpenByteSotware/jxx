#include "org/xml/sax/helpers/jxx.org.xml.sax.helpers.AttributesImpl.h"

#include <cstddef>

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"

namespace jxx::org::xml::sax::helpers {

namespace {

::jxx::lang::jbool stringsEqual(
    const ::jxx::Ptr<::jxx::lang::String>& left,
    const ::jxx::Ptr<::jxx::lang::String>& right) {

    if (left == right) {
        return true;
    }

    if (left == nullptr || right == nullptr) {
        return false;
    }

    return left->equals(right);
}

} // namespace

AttributesImpl::AttributesImpl()
    : Super() {
}

AttributesImpl::AttributesImpl(
    const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes)
    : Super() {
    setAttributes(attributes);
}

AttributesImpl::~AttributesImpl() = default;

::jxx::lang::jint AttributesImpl::getLength() const {
    return static_cast<::jxx::lang::jint>(entries_.size());
}

const AttributesImpl::Entry* AttributesImpl::entryAtOrNull_(
    ::jxx::lang::jint index) const noexcept {

    if (index < 0 ||
        static_cast<std::size_t>(index) >= entries_.size()) {
        return nullptr;
    }

    return &entries_[static_cast<std::size_t>(index)];
}

AttributesImpl::Entry& AttributesImpl::entryAt_(
    ::jxx::lang::jint index) {

    if (index < 0 ||
        static_cast<std::size_t>(index) >= entries_.size()) {
        throw ::jxx::lang::ArrayIndexOutOfBoundsException();
    }

    return entries_[static_cast<std::size_t>(index)];
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getURI(
    ::jxx::lang::jint index) const {
    const auto entry = entryAtOrNull_(index);
    return entry == nullptr ? nullptr : entry->uri;
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getLocalName(
    ::jxx::lang::jint index) const {
    const auto entry = entryAtOrNull_(index);
    return entry == nullptr ? nullptr : entry->localName;
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getQName(
    ::jxx::lang::jint index) const {
    const auto entry = entryAtOrNull_(index);
    return entry == nullptr ? nullptr : entry->qName;
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getType(
    ::jxx::lang::jint index) const {
    const auto entry = entryAtOrNull_(index);
    return entry == nullptr ? nullptr : entry->type;
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getValue(
    ::jxx::lang::jint index) const {
    const auto entry = entryAtOrNull_(index);
    return entry == nullptr ? nullptr : entry->value;
}

::jxx::lang::jint AttributesImpl::getIndex(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName) const {

    for (std::size_t index = 0; index < entries_.size(); ++index) {
        if (stringsEqual(entries_[index].uri, uri) &&
            stringsEqual(entries_[index].localName, localName)) {
            return static_cast<::jxx::lang::jint>(index);
        }
    }

    return -1;
}

::jxx::lang::jint AttributesImpl::getIndex(
    const ::jxx::Ptr<::jxx::lang::String>& qName) const {

    for (std::size_t index = 0; index < entries_.size(); ++index) {
        if (stringsEqual(entries_[index].qName, qName)) {
            return static_cast<::jxx::lang::jint>(index);
        }
    }

    return -1;
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getType(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName) const {
    return getType(getIndex(uri, localName));
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getType(
    const ::jxx::Ptr<::jxx::lang::String>& qName) const {
    return getType(getIndex(qName));
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getValue(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName) const {
    return getValue(getIndex(uri, localName));
}

::jxx::Ptr<::jxx::lang::String> AttributesImpl::getValue(
    const ::jxx::Ptr<::jxx::lang::String>& qName) const {
    return getValue(getIndex(qName));
}

void AttributesImpl::clear() {
    entries_.clear();
}

void AttributesImpl::setAttributes(
    const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes) {

    entries_.clear();

    if (attributes == nullptr) {
        return;
    }

    const auto length = attributes->getLength();
    entries_.reserve(static_cast<std::size_t>(length));

    for (::jxx::lang::jint index = 0; index < length; ++index) {
        addAttribute(
            attributes->getURI(index),
            attributes->getLocalName(index),
            attributes->getQName(index),
            attributes->getType(index),
            attributes->getValue(index));
    }
}

void AttributesImpl::addAttribute(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& qName,
    const ::jxx::Ptr<::jxx::lang::String>& type,
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    entries_.push_back(Entry{uri, localName, qName, type, value});
}

void AttributesImpl::setAttribute(
    ::jxx::lang::jint index,
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& qName,
    const ::jxx::Ptr<::jxx::lang::String>& type,
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    entryAt_(index) = Entry{uri, localName, qName, type, value};
}

void AttributesImpl::removeAttribute(::jxx::lang::jint index) {
    entryAt_(index);
    entries_.erase(entries_.begin() + index);
}

void AttributesImpl::setURI(
    ::jxx::lang::jint index,
    const ::jxx::Ptr<::jxx::lang::String>& uri) {
    entryAt_(index).uri = uri;
}

void AttributesImpl::setLocalName(
    ::jxx::lang::jint index,
    const ::jxx::Ptr<::jxx::lang::String>& localName) {
    entryAt_(index).localName = localName;
}

void AttributesImpl::setQName(
    ::jxx::lang::jint index,
    const ::jxx::Ptr<::jxx::lang::String>& qName) {
    entryAt_(index).qName = qName;
}

void AttributesImpl::setType(
    ::jxx::lang::jint index,
    const ::jxx::Ptr<::jxx::lang::String>& type) {
    entryAt_(index).type = type;
}

void AttributesImpl::setValue(
    ::jxx::lang::jint index,
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    entryAt_(index).value = value;
}

} // namespace jxx::org::xml::sax::helpers
