#include "org/xml/sax/ext/jxx.org.xml.sax.ext.Attributes2Impl.h"

#include <cstddef>

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::xml::sax::ext {

Attributes2Impl::Attributes2Impl()
    : Super() {
}

Attributes2Impl::Attributes2Impl(
    const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes)
    : Super() {
    setAttributes(attributes);
}

Attributes2Impl::~Attributes2Impl() = default;

void Attributes2Impl::checkIndex_(::jxx::lang::jint index) const {
    if (index < 0 || index >= getLength()) {
        throw ::jxx::lang::ArrayIndexOutOfBoundsException();
    }
}

::jxx::lang::jint Attributes2Impl::requireIndex_(
    const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) const {
    const auto index = getIndex(qualifiedName);
    if (index < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    return index;
}

::jxx::lang::jint Attributes2Impl::requireIndex_(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName) const {
    const auto index = getIndex(uri, localName);
    if (index < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    return index;
}

::jxx::lang::jbool Attributes2Impl::isDeclared(
    ::jxx::lang::jint index) const {
    checkIndex_(index);
    return declared_[static_cast<std::size_t>(index)];
}

::jxx::lang::jbool Attributes2Impl::isDeclared(
    const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) const {
    return isDeclared(requireIndex_(qualifiedName));
}

::jxx::lang::jbool Attributes2Impl::isDeclared(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName) const {
    return isDeclared(requireIndex_(uri, localName));
}

::jxx::lang::jbool Attributes2Impl::isSpecified(
    ::jxx::lang::jint index) const {
    checkIndex_(index);
    return specified_[static_cast<std::size_t>(index)];
}

::jxx::lang::jbool Attributes2Impl::isSpecified(
    const ::jxx::Ptr<::jxx::lang::String>& qualifiedName) const {
    return isSpecified(requireIndex_(qualifiedName));
}

::jxx::lang::jbool Attributes2Impl::isSpecified(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName) const {
    return isSpecified(requireIndex_(uri, localName));
}

void Attributes2Impl::setAttributes(
    const ::jxx::Ptr<::jxx::org::xml::sax::Attributes>& attributes) {
    JxxSuper::setAttributes(attributes);

    declared_.assign(
        static_cast<std::size_t>(getLength()),
        false);
    specified_.assign(
        static_cast<std::size_t>(getLength()),
        true);

    const auto extended = ::jxx::CAST<Attributes2>(attributes);

    for (::jxx::lang::jint index = 0;
         index < getLength();
         ++index) {
        if (extended != nullptr) {
            declared_[static_cast<std::size_t>(index)] =
                extended->isDeclared(index);
            specified_[static_cast<std::size_t>(index)] =
                extended->isSpecified(index);
        }
        else {
            const auto type = getType(index);
            declared_[static_cast<std::size_t>(index)] =
                type != nullptr && type->utf8() != "CDATA";
        }
    }
}

void Attributes2Impl::addAttribute(
    const ::jxx::Ptr<::jxx::lang::String>& uri,
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& qualifiedName,
    const ::jxx::Ptr<::jxx::lang::String>& type,
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    JxxSuper::addAttribute(
        uri,
        localName,
        qualifiedName,
        type,
        value);

    declared_.push_back(
        type != nullptr && type->utf8() != "CDATA");
    specified_.push_back(true);
}

void Attributes2Impl::removeAttribute(::jxx::lang::jint index) {
    checkIndex_(index);
    JxxSuper::removeAttribute(index);
    declared_.erase(declared_.begin() + index);
    specified_.erase(specified_.begin() + index);
}

void Attributes2Impl::setDeclared(
    ::jxx::lang::jint index,
    ::jxx::lang::jbool value) {
    checkIndex_(index);
    declared_[static_cast<std::size_t>(index)] = value;
}

void Attributes2Impl::setSpecified(
    ::jxx::lang::jint index,
    ::jxx::lang::jbool value) {
    checkIndex_(index);
    specified_[static_cast<std::size_t>(index)] = value;
}

} // namespace jxx::org::xml::sax::ext
