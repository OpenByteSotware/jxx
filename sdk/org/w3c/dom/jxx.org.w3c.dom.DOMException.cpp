#include "org/w3c/dom/jxx.org.w3c.dom.DOMException.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::w3c::dom {

::jxx::Ptr<::jxx::lang::ClassAny>
DOMException::Class() {
    return JxxClassInfoMarker::Class();
}

DOMException::DOMException(
    ::jxx::lang::jshort exceptionCode,
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message)
    , code(exceptionCode) {
}

DOMException::DOMException(
    const DOMException& other)
    : JxxSuper(other)
    , code(other.code) {
}

DOMException::DOMException(
    DOMException&& other) noexcept
    : JxxSuper(std::move(other))
    , code(other.code) {
}

DOMException& DOMException::operator=(
    const DOMException& other) {

    if (this == &other) {
        return *this;
    }

    JxxSuper::operator=(other);
    code = other.code;

    return *this;
}

DOMException& DOMException::operator=(
    DOMException&& other) noexcept {

    if (this == &other) {
        return *this;
    }

    JxxSuper::operator=(std::move(other));
    code = other.code;

    return *this;
}

DOMException::~DOMException() = default;

::jxx::Ptr<::jxx::lang::Object>
DOMException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<DOMException>(*this));
}

const char*
DOMException::typeName() const noexcept {
    return "DOMException";
}

} // namespace jxx::org::w3c::dom
