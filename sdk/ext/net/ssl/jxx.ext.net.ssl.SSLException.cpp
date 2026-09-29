#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"

#include "lang/jxx.lang.Throwable.h"

namespace jxx::ext::net::ssl {
namespace {

::jxx::Ptr<::jxx::lang::String>
messageFromCause(
    const ::jxx::Ptr<
        ::jxx::lang::Throwable>& cause)
{
    return cause == nullptr
        ? nullptr
        : cause->toString();
}

} // namespace

SSLException::SSLException()
    : Super() {
}

SSLException::SSLException(
    const ::jxx::Ptr<
        ::jxx::lang::String>& message)
    : Super(message) {
}

SSLException::SSLException(
    const ::jxx::Ptr<
        ::jxx::lang::Throwable>& cause)
    : Super(
          messageFromCause(cause))
{
    initCause(cause);
}

SSLException::SSLException(
    const ::jxx::Ptr<
        ::jxx::lang::String>& message,
    const ::jxx::Ptr<
        ::jxx::lang::Throwable>& cause)
    : Super(message)
{
    initCause(cause);
}

SSLException::SSLException(
    const char* message)
    : Super(message) {
}

SSLException::SSLException(
    const std::string& message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object>
SSLException::cloneImpl() const {
    return ::jxx::NEW<SSLException>(
        *this);
}

const char*
SSLException::typeName()
    const noexcept {
    return "jxx.ext.net.ssl.SSLException";
}

} // namespace jxx::ext::net::ssl
