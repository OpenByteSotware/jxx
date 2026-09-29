#include "net/jxx.net.StandardSocketOptions.h"

#include <typeindex>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Integer.h"
#include "net/jxx.net.NetworkInterface.h"

namespace jxx::net {
namespace {

::jxx::Ptr<StandardSocketOptions::Option> makeOption(
    const char* name,
    const std::type_index& type) {
    return ::jxx::CAST<StandardSocketOptions::Option>(
        ::jxx::NEW<BasicSocketOption>(
            ::jxx::NEW<::jxx::lang::String>(name),
            ::jxx::lang::ClassAny::forType(type)));
}

} // namespace

::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_BROADCAST_ =
    makeOption("SO_BROADCAST", typeid(::jxx::lang::Boolean));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_KEEPALIVE_ =
    makeOption("SO_KEEPALIVE", typeid(::jxx::lang::Boolean));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_SNDBUF_ =
    makeOption("SO_SNDBUF", typeid(::jxx::lang::Integer));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_RCVBUF_ =
    makeOption("SO_RCVBUF", typeid(::jxx::lang::Integer));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_REUSEADDR_ =
    makeOption("SO_REUSEADDR", typeid(::jxx::lang::Boolean));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_LINGER_ =
    makeOption("SO_LINGER", typeid(::jxx::lang::Integer));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::IP_TOS_ =
    makeOption("IP_TOS", typeid(::jxx::lang::Integer));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::IP_MULTICAST_IF_ =
    makeOption("IP_MULTICAST_IF", typeid(::jxx::net::NetworkInterface));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::IP_MULTICAST_TTL_ =
    makeOption("IP_MULTICAST_TTL", typeid(::jxx::lang::Integer));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::IP_MULTICAST_LOOP_ =
    makeOption("IP_MULTICAST_LOOP", typeid(::jxx::lang::Boolean));
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::TCP_NODELAY_ =
    makeOption("TCP_NODELAY", typeid(::jxx::lang::Boolean));

} // namespace jxx::net
