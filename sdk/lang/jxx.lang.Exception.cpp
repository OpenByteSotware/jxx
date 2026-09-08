#include "lang/jxx.lang.Exception.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {

namespace {

jxx::Ptr<String> messageFromCause(
    const jxx::Ptr<Throwable>& cause) {

    return cause == nullptr
        ? nullptr
        : cause->toString();
}

} // namespace

jxx::Ptr<ClassAny> Exception::Class() {
    return JxxClassInfoMarker::Class();
}

Exception::Exception()
    : Throwable() {
}

Exception::Exception(
    const jxx::Ptr<String>& message)
    : Throwable(message) {
}

Exception::Exception(
    const jxx::Ptr<String>& message,
    const jxx::Ptr<Throwable>& cause)
    : Throwable(message, cause) {
}

Exception::Exception(
    const jxx::Ptr<Throwable>& cause)
    : Throwable(messageFromCause(cause), cause) {
}

Exception::Exception(
    const char* message)
    : Throwable(message) {
}

Exception::Exception(
    const std::string& message)
    : Throwable(message) {
}

Exception::Exception(
    const Exception& other)
    : Throwable(other) {
}

Exception::Exception(
    Exception&& other) noexcept
    : Throwable(std::move(other)) {
}

Exception& Exception::operator=(
    const Exception& other) {

    if (this != &other) {
        Throwable::operator=(other);
    }

    return *this;
}

Exception& Exception::operator=(
    Exception&& other) noexcept {

    if (this != &other) {
        Throwable::operator=(std::move(other));
    }

    return *this;
}

Exception::~Exception() = default;

jxx::Ptr<Object> Exception::cloneImpl() const {
    return jxx::CAST<Object>(
        jxx::NEW<Exception>(*this));
}

const char* Exception::typeName() const noexcept {
    return "Exception";
}

} // namespace jxx::lang
