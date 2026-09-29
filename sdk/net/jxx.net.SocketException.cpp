#include "net/jxx.net.SocketException.h"

namespace jxx::net {

SocketException::SocketException()
    : Super("SocketException") {
}

SocketException::SocketException(
    const ::jxx::Ptr<
        ::jxx::lang::String>& message)
    : Super(message) {
}

SocketException::SocketException(
    const char* message)
    : Super(
          message == nullptr
              ? "SocketException"
              : message) {
}

SocketException::SocketException(
    const std::string& message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object>
SocketException::cloneImpl() const {
    return ::jxx::NEW<SocketException>(
        *this);
}

const char*
SocketException::typeName()
    const noexcept {
    return "jxx.net.SocketException";
}

} // namespace jxx::net
