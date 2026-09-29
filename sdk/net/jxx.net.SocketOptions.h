#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::net {

class SocketOptions
    : public ::jxx::lang::InterfaceBase<SocketOptions> {
public:
    static constexpr ::jxx::lang::jint TCP_NODELAY_ = 0x0001;
    static constexpr ::jxx::lang::jint SO_REUSEADDR_ = 0x0004;
    static constexpr ::jxx::lang::jint SO_KEEPALIVE_ = 0x0008;
    static constexpr ::jxx::lang::jint SO_BINDADDR_ = 0x000F;
    static constexpr ::jxx::lang::jint IP_MULTICAST_IF_ = 0x0010;
    static constexpr ::jxx::lang::jint IP_MULTICAST_LOOP_ = 0x0012;
    static constexpr ::jxx::lang::jint SO_BROADCAST_ = 0x0020;
    static constexpr ::jxx::lang::jint IP_MULTICAST_IF2_ = 0x001F;
    static constexpr ::jxx::lang::jint IP_TOS_ = 0x0003;
    static constexpr ::jxx::lang::jint SO_LINGER_ = 0x0080;
    static constexpr ::jxx::lang::jint SO_SNDBUF_ = 0x1001;
    static constexpr ::jxx::lang::jint SO_RCVBUF_ = 0x1002;
    static constexpr ::jxx::lang::jint SO_OOBINLINE_ = 0x1003;
    static constexpr ::jxx::lang::jint SO_TIMEOUT_ = 0x1006;

    ~SocketOptions() override = default;

    virtual void setOption(
        ::jxx::lang::jint optionId,
        const ::jxx::Ptr<::jxx::lang::Object>& value) = 0;

    virtual ::jxx::Ptr<::jxx::lang::Object> getOption(
        ::jxx::lang::jint optionId) = 0;
};

} // namespace jxx::net
