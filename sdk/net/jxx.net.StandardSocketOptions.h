#pragma once

#include "lang/jxx_types.h"
#include "net/jxx.net.SocketOption.h"

namespace jxx::net {

class StandardSocketOptions final {
public:
    using Option = SocketOption<::jxx::lang::Object>;

    static ::jxx::Ptr<Option> SO_BROADCAST_;
    static ::jxx::Ptr<Option> SO_KEEPALIVE_;
    static ::jxx::Ptr<Option> SO_SNDBUF_;
    static ::jxx::Ptr<Option> SO_RCVBUF_;
    static ::jxx::Ptr<Option> SO_REUSEADDR_;
    static ::jxx::Ptr<Option> SO_LINGER_;
    static ::jxx::Ptr<Option> IP_TOS_;
    static ::jxx::Ptr<Option> IP_MULTICAST_IF_;
    static ::jxx::Ptr<Option> IP_MULTICAST_TTL_;
    static ::jxx::Ptr<Option> IP_MULTICAST_LOOP_;
    static ::jxx::Ptr<Option> TCP_NODELAY_;

    StandardSocketOptions() = delete;
};

} // namespace jxx::net
