#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::net
{
    class SocketOptions
    {
    public:
        virtual ~SocketOptions() = default;

    public:
        inline static constexpr jxx::lang::jint TCP_NODELAY_ = 0x0001;
        inline static constexpr jxx::lang::jint SO_BINDADDR_ = 0x000F;
        inline static constexpr jxx::lang::jint SO_REUSEADDR_ = 0x0004;
        inline static constexpr jxx::lang::jint SO_BROADCAST_ = 0x0020;
        inline static constexpr jxx::lang::jint IP_MULTICAST_IF_ = 0x0010;
        inline static constexpr jxx::lang::jint IP_MULTICAST_IF2_ = 0x001F;
        inline static constexpr jxx::lang::jint IP_MULTICAST_LOOP_ = 0x0012;
        inline static constexpr jxx::lang::jint IP_TOS_ = 0x0003;
        inline static constexpr jxx::lang::jint SO_LINGER_ = 0x0080;
        inline static constexpr jxx::lang::jint SO_TIMEOUT_ = 0x1006;
        inline static constexpr jxx::lang::jint SO_SNDBUF_ = 0x1001;
        inline static constexpr jxx::lang::jint SO_RCVBUF_ = 0x1002;
        inline static constexpr jxx::lang::jint SO_KEEPALIVE_ = 0x0008;
        inline static constexpr jxx::lang::jint SO_OOBINLINE_ = 0x1003;

        virtual void setOption(jxx::lang::jint optID,
                               jxx::Ptr<jxx::lang::Object> value) = 0;
        virtual jxx::Ptr<jxx::lang::Object> getOption(jxx::lang::jint optID) = 0;
    };
}
