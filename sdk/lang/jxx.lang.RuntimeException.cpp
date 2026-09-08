#include "lang/jxx.lang.RuntimeException.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {

jxx::Ptr<ClassAny> RuntimeException::Class() {
    return JxxClassInfoMarker::Class();
}

RuntimeException::RuntimeException()
    : Exception() {
}

RuntimeException::RuntimeException(
    const jxx::Ptr<String>& message)
    : Exception(message) {
}

RuntimeException::RuntimeException(
    const jxx::Ptr<String>& message,
    const jxx::Ptr<Throwable>& cause)
    : Exception(message, cause) {
}

RuntimeException::RuntimeException(
    const jxx::Ptr<Throwable>& cause)
    : Exception(cause) {
}

RuntimeException::RuntimeException(
    const char* message)
    : Exception(message) {
}

RuntimeException::RuntimeException(
    const std::string& message)
    : Exception(message) {
}

RuntimeException::RuntimeException(
    const RuntimeException& other)
    : Exception(other) {
}

RuntimeException::RuntimeException(
    RuntimeException&& other) noexcept
    : Exception(std::move(other)) {
}

RuntimeException& RuntimeException::operator=(
    const RuntimeException& other) {

    if (this != &other) {
        Exception::operator=(other);
    }

    return *this;
}

RuntimeException& RuntimeException::operator=(
    RuntimeException&& other) noexcept {

    if (this != &other) {
        Exception::operator=(std::move(other));
    }

    return *this;
}

RuntimeException::~RuntimeException() = default;

jxx::Ptr<Object> RuntimeException::cloneImpl() const {
    return jxx::CAST<Object>(
        jxx::NEW<RuntimeException>(*this));
}

const char* RuntimeException::typeName() const noexcept {
    return "RuntimeException";
}

} // namespace jxx::lang
