#include "net/jxx.net.SocketTimeoutException.h"

namespace jxx::net {

SocketTimeoutException::
SocketTimeoutException()
    : Super(
          "SocketTimeoutException") {
}

SocketTimeoutException::
SocketTimeoutException(
    const ::jxx::Ptr<
        ::jxx::lang::String>& message)
    : Super(message) {
}

SocketTimeoutException::
SocketTimeoutException(
    const char* message)
    : Super(
          message == nullptr
              ? "SocketTimeoutException"
              : message) {
}

SocketTimeoutException::
SocketTimeoutException(
    const std::string& message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object>
SocketTimeoutException::cloneImpl()
    const {
    return ::jxx::NEW<
        SocketTimeoutException>(
            *this);
}

const char*
SocketTimeoutException::typeName()
    const noexcept {
    return "jxx.net.SocketTimeoutException";
}

} // namespace jxx::net
