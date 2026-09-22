#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::namespace_ {
namespace {
::jxx::Ptr<::jxx::lang::String> emptyString() {
    return ::jxx::NEW<::jxx::lang::String>("");
}
}

QName::QName(
    const ::jxx::Ptr<::jxx::lang::String>& localPart)
    : QName(emptyString(), localPart, emptyString()) {}

QName::QName(
    const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
    const ::jxx::Ptr<::jxx::lang::String>& localPart)
    : QName(namespaceURI, localPart, emptyString()) {}

QName::QName(
    const ::jxx::Ptr<::jxx::lang::String>& namespaceURI,
    const ::jxx::Ptr<::jxx::lang::String>& localPart,
    const ::jxx::Ptr<::jxx::lang::String>& prefix)
    : namespaceURI_(namespaceURI == nullptr ? emptyString() : namespaceURI),
      localPart_(localPart), prefix_(prefix) {
    if (localPart_ == nullptr || prefix_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String> QName::getNamespaceURI() const { return namespaceURI_; }
::jxx::Ptr<::jxx::lang::String> QName::getLocalPart() const { return localPart_; }
::jxx::Ptr<::jxx::lang::String> QName::getPrefix() const { return prefix_; }

::jxx::lang::jbool QName::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    const auto name = std::dynamic_pointer_cast<QName>(other);
    return name != nullptr &&
        namespaceURI_->equals(::jxx::CAST<::jxx::lang::Object>(name->namespaceURI_)) &&
        localPart_->equals(::jxx::CAST<::jxx::lang::Object>(name->localPart_));
}

::jxx::lang::jint QName::hashCode() const {
    return namespaceURI_->hashCode() ^ localPart_->hashCode();
}

::jxx::Ptr<::jxx::lang::String> QName::toString() const {
    if (namespaceURI_->isEmpty()) return localPart_;
    return ::jxx::NEW<::jxx::lang::String>(
        "{" + namespaceURI_->utf8() + "}" + localPart_->utf8());
}

::jxx::Ptr<QName> QName::valueOf(
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    const auto text = value->utf8();
    if (text.empty() || text.front() != '{') {
        return ::jxx::NEW<QName>(value);
    }
    const auto close = text.find('}');
    if (close == std::string::npos || close + 1U >= text.size()) {
        throw ::jxx::lang::IllegalArgumentException(value);
    }
    return ::jxx::NEW<QName>(
        ::jxx::NEW<::jxx::lang::String>(text.substr(1U, close - 1U)),
        ::jxx::NEW<::jxx::lang::String>(text.substr(close + 1U)));
}

} // namespace jxx::ext::xml::namespace_
