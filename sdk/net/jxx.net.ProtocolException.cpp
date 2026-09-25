#include "net/jxx.net.ProtocolException.h"

namespace jxx::net {

::jxx::Ptr<::jxx::lang::ClassAny>
ProtocolException::Class() {
    return JxxClassInfoMarker::Class();
}

ProtocolException::ProtocolException()
    : Super() {
}

ProtocolException::ProtocolException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

ProtocolException::ProtocolException(const char* message)
    : Super(message != nullptr ? message : "ProtocolException") {
}

ProtocolException::ProtocolException(const std::string& message)
    : Super(message) {
}

::jxx::Ptr<::jxx::lang::Object>
ProtocolException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<ProtocolException>(*this));
}

const char* ProtocolException::typeName() const noexcept {
    return "java.net.ProtocolException";
}

} // namespace jxx::net
