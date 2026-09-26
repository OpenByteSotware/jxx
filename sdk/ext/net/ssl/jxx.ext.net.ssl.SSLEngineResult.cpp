#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngineResult.h"

#include <string>

#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl {
namespace {

const char* statusName(SSLEngineResult::Status status) noexcept {
    switch (status) {
    case SSLEngineResult::Status::BUFFER_UNDERFLOW: return "BUFFER_UNDERFLOW";
    case SSLEngineResult::Status::BUFFER_OVERFLOW: return "BUFFER_OVERFLOW";
    case SSLEngineResult::Status::OK: return "OK";
    case SSLEngineResult::Status::CLOSED: return "CLOSED";
    }
    return "UNKNOWN";
}

const char* handshakeName(
    SSLEngineResult::HandshakeStatus status) noexcept {
    switch (status) {
    case SSLEngineResult::HandshakeStatus::NOT_HANDSHAKING:
        return "NOT_HANDSHAKING";
    case SSLEngineResult::HandshakeStatus::FINISHED: return "FINISHED";
    case SSLEngineResult::HandshakeStatus::NEED_TASK: return "NEED_TASK";
    case SSLEngineResult::HandshakeStatus::NEED_WRAP: return "NEED_WRAP";
    case SSLEngineResult::HandshakeStatus::NEED_UNWRAP: return "NEED_UNWRAP";
    }
    return "UNKNOWN";
}

} // namespace

SSLEngineResult::SSLEngineResult(
    Status status,
    HandshakeStatus handshakeStatus,
    ::jxx::lang::jint bytesConsumed,
    ::jxx::lang::jint bytesProduced)
    : status_(status)
    , handshakeStatus_(handshakeStatus)
    , bytesConsumed_(bytesConsumed)
    , bytesProduced_(bytesProduced) {
    if (bytesConsumed < 0 || bytesProduced < 0)
        throw ::jxx::lang::IllegalArgumentException();
}

SSLEngineResult::Status SSLEngineResult::getStatus() const noexcept {
    return status_;
}

SSLEngineResult::HandshakeStatus
SSLEngineResult::getHandshakeStatus() const noexcept {
    return handshakeStatus_;
}

::jxx::lang::jint SSLEngineResult::bytesConsumed() const noexcept {
    return bytesConsumed_;
}

::jxx::lang::jint SSLEngineResult::bytesProduced() const noexcept {
    return bytesProduced_;
}

::jxx::Ptr<::jxx::lang::String> SSLEngineResult::toString() const {
    return ::jxx::NEW<::jxx::lang::String>(
        std::string("Status = ") + statusName(status_) +
        " HandshakeStatus = " + handshakeName(handshakeStatus_) +
        " bytesConsumed = " + std::to_string(bytesConsumed_) +
        " bytesProduced = " + std::to_string(bytesProduced_));
}

} // namespace jxx::ext::net::ssl
