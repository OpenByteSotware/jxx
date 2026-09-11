#include "lang/jxx.lang.EmptyStackException.h"

#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {

::jxx::Ptr<ClassAny> EmptyStackException::Class() {
    return JxxClassInfoMarker::Class();
}

EmptyStackException::EmptyStackException()
    : JxxSuper() {
}

EmptyStackException::EmptyStackException(
    const EmptyStackException& other)
    : JxxSuper(other) {
}

EmptyStackException::EmptyStackException(
    EmptyStackException&& other) noexcept
    : JxxSuper(std::move(other)) {
}

EmptyStackException& EmptyStackException::operator=(
    const EmptyStackException& other) {
    JxxSuper::operator=(other);
    return *this;
}

EmptyStackException& EmptyStackException::operator=(
    EmptyStackException&& other) noexcept {
    JxxSuper::operator=(std::move(other));
    return *this;
}

EmptyStackException::~EmptyStackException() = default;

::jxx::Ptr<Object> EmptyStackException::cloneImpl() const {
    return ::jxx::CAST<Object>(
        ::jxx::NEW<EmptyStackException>(*this));
}

const char* EmptyStackException::typeName() const noexcept {
    return "EmptyStackException";
}

} // namespace jxx::lang
