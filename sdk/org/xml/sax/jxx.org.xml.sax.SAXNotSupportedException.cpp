#include "org/xml/sax/jxx.org.xml.sax.SAXNotSupportedException.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::xml::sax {

::jxx::Ptr<::jxx::lang::ClassAny>
SAXNotSupportedException::Class() {
    return JxxClassInfoMarker::Class();
}

SAXNotSupportedException::SAXNotSupportedException()
    : JxxSuper() {
}

SAXNotSupportedException::SAXNotSupportedException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message) {
}

SAXNotSupportedException::SAXNotSupportedException(
    const SAXNotSupportedException& other)
    : JxxSuper(other) {
}

SAXNotSupportedException::SAXNotSupportedException(
    SAXNotSupportedException&& other) noexcept
    : JxxSuper(std::move(other)) {
}

SAXNotSupportedException&
SAXNotSupportedException::operator=(
    const SAXNotSupportedException& other) {
    JxxSuper::operator=(other);
    return *this;
}

SAXNotSupportedException&
SAXNotSupportedException::operator=(
    SAXNotSupportedException&& other) noexcept {
    JxxSuper::operator=(std::move(other));
    return *this;
}

SAXNotSupportedException::~SAXNotSupportedException() = default;

::jxx::Ptr<::jxx::lang::Object>
SAXNotSupportedException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<SAXNotSupportedException>(*this));
}

const char*
SAXNotSupportedException::typeName() const noexcept {
    return "SAXNotSupportedException";
}

} // namespace jxx::org::xml::sax
