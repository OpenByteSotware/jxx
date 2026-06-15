#pragma once

#include "jxx_types.h"
#include "net/jxx.net.SocketOption.h"

namespace jxx::net
{
    class StandardSocketOptions final
    {
    public:
        static jxx::Ptr<SocketOption<jxx::lang::Object>> SO_BROADCAST;
        static jxx::Ptr<SocketOption<jxx::lang::Object>> SO_KEEPALIVE;
        static jxx::Ptr<SocketOption<jxx::lang::Object>> SO_SNDBUF;
        static jxx::Ptr<SocketOption<jxx::lang::Object>> SO_RCVBUF;
        static jxx::Ptr<SocketOption<jxx::lang::Object>> SO_REUSEADDR;
        static jxx::Ptr<SocketOption<jxx::lang::Object>> SO_LINGER;
        static jxx::Ptr<SocketOption<jxx::lang::Object>> IP_TOS;
        static jxx::Ptr<SocketOption<jxx::lang::Object>> TCP_NODELAY;
    };
}
