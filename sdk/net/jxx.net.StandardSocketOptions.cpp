#include "net/jxx.net.StandardSocketOptions.h"

namespace jxx::net
{
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_BROADCAST = jxx::NEW<BasicSocketOption>(jxx::NEW<jxx::lang::String>("SO_BROADCAST"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_KEEPALIVE = jxx::NEW<BasicSocketOption>(jxx::NEW<jxx::lang::String>("SO_KEEPALIVE"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_SNDBUF = jxx::NEW<BasicSocketOption>(jxx::NEW<jxx::lang::String>("SO_SNDBUF"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_RCVBUF = jxx::NEW<BasicSocketOption>(jxx::NEW<jxx::lang::String>("SO_RCVBUF"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_REUSEADDR = jxx::NEW<BasicSocketOption>(jxx::NEW<jxx::lang::String>("SO_REUSEADDR"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::SO_LINGER = jxx::NEW<BasicSocketOption>(jxx::NEW<jxx::lang::String>("SO_LINGER"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::IP_TOS = jxx::NEW<BasicSocketOption>(jxx::NEW<jxx::lang::String>("IP_TOS"));
    jxx::Ptr<SocketOption<jxx::lang::Object>> StandardSocketOptions::TCP_NODELAY = jxx::NEW<BasicSocketOption>(jxx::NEW<jxx::lang::String>("TCP_NODELAY"));
}
