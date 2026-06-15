#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"

namespace jxx::net
{
    class SocketOptions
    {
    public:
        virtual ~SocketOptions() = default;

    public:
        static constexpr jxx::lang::jint TCP_NODELAY = 0x0001;
        static constexpr jxx::lang::jint SO_BINDADDR = 0x000F;
        static constexpr jxx::lang::jint SO_REUSEADDR = 0x0004;
        static constexpr jxx::lang::jint SO_BROADCAST = 0x0020;
        static constexpr jxx::lang::jint IP_MULTICAST_IF = 0x0010;
        static constexpr jxx::lang::jint IP_MULTICAST_IF2 = 0x001F;
        static constexpr jxx::lang::jint IP_MULTICAST_LOOP = 0x0012;
        static constexpr jxx::lang::jint IP_TOS = 0x0003;
        static constexpr jxx::lang::jint SO_LINGER = 0x0080;
        static constexpr jxx::lang::jint SO_TIMEOUT = 0x1006;
        static constexpr jxx::lang::jint SO_SNDBUF = 0x1001;
        static constexpr jxx::lang::jint SO_RCVBUF = 0x1002;
        static constexpr jxx::lang::jint SO_KEEPALIVE = 0x0008;
        static constexpr jxx::lang::jint SO_OOBINLINE = 0x1003;

        virtual void setOption(jxx::lang::jint optID,
                               jxx::Ptr<jxx::lang::Object> value) = 0;
        virtual jxx::Ptr<jxx::lang::Object> getOption(jxx::lang::jint optID) = 0;
    };
}
