#include "org/xml/sax/jxx.org.xml.sax.SAXNotRecognizedException.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::org::xml::sax {

::jxx::Ptr<::jxx::lang::ClassAny>
SAXNotRecognizedException::Class() {
    return JxxClassInfoMarker::Class();
}

SAXNotRecognizedException::SAXNotRecognizedException()
    : JxxSuper() {
}

SAXNotRecognizedException::SAXNotRecognizedException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message) {
}

SAXNotRecognizedException::SAXNotRecognizedException(
    const SAXNotRecognizedException& other)
    : JxxSuper(other) {
}

SAXNotRecognizedException::SAXNotRecognizedException(
    SAXNotRecognizedException&& other) noexcept
    : JxxSuper(std::move(other)) {
}

SAXNotRecognizedException&
SAXNotRecognizedException::operator=(
    const SAXNotRecognizedException& other) {
    JxxSuper::operator=(other);
    return *this;
}

SAXNotRecognizedException&
SAXNotRecognizedException::operator=(
    SAXNotRecognizedException&& other) noexcept {
    JxxSuper::operator=(std::move(other));
    return *this;
}

SAXNotRecognizedException::~SAXNotRecognizedException() = default;

::jxx::Ptr<::jxx::lang::Object>
SAXNotRecognizedException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<SAXNotRecognizedException>(*this));
}

const char*
SAXNotRecognizedException::typeName() const noexcept {
    return "SAXNotRecognizedException";
}

} // namespace jxx::org::xml::sax
