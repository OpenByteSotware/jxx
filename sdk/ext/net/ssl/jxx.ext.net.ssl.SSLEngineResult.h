#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::net::ssl {

class SSLEngineResult final
    : public ::jxx::lang::ClassBase<
          SSLEngineResult,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SSLEngineResult, JxxSuper>;

    enum class Status {
        BUFFER_UNDERFLOW,
        BUFFER_OVERFLOW,
        OK,
        CLOSED
    };

    enum class HandshakeStatus {
        NOT_HANDSHAKING,
        FINISHED,
        NEED_TASK,
        NEED_WRAP,
        NEED_UNWRAP
    };

    SSLEngineResult(
        Status status,
        HandshakeStatus handshakeStatus,
        ::jxx::lang::jint bytesConsumed,
        ::jxx::lang::jint bytesProduced);

    Status getStatus() const noexcept;
    HandshakeStatus getHandshakeStatus() const noexcept;
    ::jxx::lang::jint bytesConsumed() const noexcept;
    ::jxx::lang::jint bytesProduced() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

private:
    Status status_;
    HandshakeStatus handshakeStatus_;
    ::jxx::lang::jint bytesConsumed_;
    ::jxx::lang::jint bytesProduced_;
};

} // namespace jxx::ext::net::ssl
