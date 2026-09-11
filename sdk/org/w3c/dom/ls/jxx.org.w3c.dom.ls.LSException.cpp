#include "org/w3c/dom/ls/jxx.org.w3c.dom.ls.LSException.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::w3c::dom::ls {

::jxx::Ptr<::jxx::lang::ClassAny> LSException::Class() {
    return JxxClassInfoMarker::Class();
}

LSException::LSException(
    ::jxx::lang::jshort exceptionCode,
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message)
    , code(exceptionCode) {
}

LSException::LSException(const LSException& other)
    : JxxSuper(other)
    , code(other.code) {
}

LSException::LSException(LSException&& other) noexcept
    : JxxSuper(std::move(other))
    , code(other.code) {
}

LSException& LSException::operator=(const LSException& other) {
    if (this != &other) {
        JxxSuper::operator=(other);
        code = other.code;
    }
    return *this;
}

LSException& LSException::operator=(LSException&& other) noexcept {
    if (this != &other) {
        JxxSuper::operator=(std::move(other));
        code = other.code;
    }
    return *this;
}

LSException::~LSException() = default;

::jxx::Ptr<::jxx::lang::Object> LSException::cloneImpl() const {
    return ::jxx::NEW<LSException>(*this);
}

const char* LSException::typeName() const noexcept {
    return "org.w3c.dom.ls.LSException";
}

} // namespace jxx::org::w3c::dom::ls
