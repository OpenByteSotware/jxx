#include "net/jxx.net.StandardSocketOptions.h"

namespace jxx::net
{
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_BROADCAST = std::make_shared<BasicSocketOption>(std::make_shared<jxx::lang::String>("SO_BROADCAST"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_KEEPALIVE = std::make_shared<BasicSocketOption>(std::make_shared<jxx::lang::String>("SO_KEEPALIVE"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_SNDBUF = std::make_shared<BasicSocketOption>(std::make_shared<jxx::lang::String>("SO_SNDBUF"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_RCVBUF = std::make_shared<BasicSocketOption>(std::make_shared<jxx::lang::String>("SO_RCVBUF"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_REUSEADDR = std::make_shared<BasicSocketOption>(std::make_shared<jxx::lang::String>("SO_REUSEADDR"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_LINGER = std::make_shared<BasicSocketOption>(std::make_shared<jxx::lang::String>("SO_LINGER"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::IP_TOS = std::make_shared<BasicSocketOption>(std::make_shared<jxx::lang::String>("IP_TOS"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::TCP_NODELAY = std::make_shared<BasicSocketOption>(std::make_shared<jxx::lang::String>("TCP_NODELAY"));
}
