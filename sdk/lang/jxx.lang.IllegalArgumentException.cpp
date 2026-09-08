#include "lang/jxx.lang.IllegalArgumentException.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {

jxx::Ptr<ClassAny> IllegalArgumentException::Class() {
    return JxxClassInfoMarker::Class();
}

IllegalArgumentException::IllegalArgumentException()
    : RuntimeException() {
}

IllegalArgumentException::IllegalArgumentException(
    const jxx::Ptr<String>& message)
    : RuntimeException(message) {
}

IllegalArgumentException::IllegalArgumentException(
    const jxx::Ptr<String>& message,
    const jxx::Ptr<Throwable>& cause)
    : RuntimeException(message, cause) {
}

IllegalArgumentException::IllegalArgumentException(
    const jxx::Ptr<Throwable>& cause)
    : RuntimeException(cause) {
}

IllegalArgumentException::IllegalArgumentException(
    const char* message)
    : RuntimeException(message) {
}

IllegalArgumentException::IllegalArgumentException(
    const std::string& message)
    : RuntimeException(message) {
}

IllegalArgumentException::IllegalArgumentException(
    const IllegalArgumentException& other)
    : RuntimeException(other) {
}

IllegalArgumentException::IllegalArgumentException(
    IllegalArgumentException&& other) noexcept
    : RuntimeException(std::move(other)) {
}

IllegalArgumentException&
IllegalArgumentException::operator=(
    const IllegalArgumentException& other) {

    if (this != &other) {
        RuntimeException::operator=(other);
    }

    return *this;
}

IllegalArgumentException&
IllegalArgumentException::operator=(
    IllegalArgumentException&& other) noexcept {

    if (this != &other) {
        RuntimeException::operator=(std::move(other));
    }

    return *this;
}

IllegalArgumentException::~IllegalArgumentException() = default;

jxx::Ptr<Object>
IllegalArgumentException::cloneImpl() const {
    return jxx::CAST<Object>(
        jxx::NEW<IllegalArgumentException>(*this));
}

const char*
IllegalArgumentException::typeName() const noexcept {
    return "IllegalArgumentException";
}

} // namespace jxx::lang
