#include "lang/jxx.lang.InvocationTargetException.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {

jxx::Ptr<ClassAny> InvocationTargetException::Class() {
    return JxxClassInfoMarker::Class();
}

InvocationTargetException::InvocationTargetException(
    const jxx::Ptr<Throwable>& cause)
    : Exception(cause) {
}

InvocationTargetException::InvocationTargetException(
    const jxx::Ptr<String>& message,
    const jxx::Ptr<Throwable>& cause)
    : Exception(message, cause) {
}

InvocationTargetException::InvocationTargetException(
    const InvocationTargetException& other)
    : Exception(other) {
}

InvocationTargetException::InvocationTargetException(
    InvocationTargetException&& other) noexcept
    : Exception(std::move(other)) {
}

InvocationTargetException&
InvocationTargetException::operator=(
    const InvocationTargetException& other) {

    if (this != &other) {
        Exception::operator=(other);
    }

    return *this;
}

InvocationTargetException&
InvocationTargetException::operator=(
    InvocationTargetException&& other) noexcept {

    if (this != &other) {
        Exception::operator=(std::move(other));
    }

    return *this;
}

InvocationTargetException::~InvocationTargetException() = default;

jxx::Ptr<Throwable>
InvocationTargetException::getTargetException() const {
    return getCause();
}

jxx::Ptr<Object>
InvocationTargetException::cloneImpl() const {
    return jxx::CAST<Object>(
        jxx::NEW<InvocationTargetException>(*this));
}

const char*
InvocationTargetException::typeName() const noexcept {
    return "InvocationTargetException";
}

} // namespace jxx::lang
