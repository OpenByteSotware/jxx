#include "net/jxx.net.StandardSocketOptions.h"

#include <utility>

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.Integer.h"
#include "net/jxx.net.NetworkInterface.h"

namespace jxx::net {
namespace {

using Option = ::jxx::net::StandardSocketOptions::Option;
using Resolver = ::jxx::net::BasicSocketOption::TypeResolver;

::jxx::Ptr<Option> makeOption(
    const char* name,
    Resolver resolver) {
    return ::jxx::CAST<Option>(
        ::jxx::NEW<::jxx::net::BasicSocketOption>(
            ::jxx::NEW<::jxx::lang::String>(name),
            std::move(resolver)));
}

Resolver booleanType() {
    return []() { return ::jxx::lang::Boolean::Class(); };
}

Resolver integerType() {
    return []() { return ::jxx::lang::Integer::Class(); };
}

Resolver networkInterfaceType() {
    return []() {
        return ::jxx::lang::ClassInfo<
            ::jxx::net::NetworkInterface,
            ::jxx::lang::Object>::Class();
    };
}

} // namespace

::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_BROADCAST_ =
    makeOption("SO_BROADCAST", booleanType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_KEEPALIVE_ =
    makeOption("SO_KEEPALIVE", booleanType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_SNDBUF_ =
    makeOption("SO_SNDBUF", integerType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_RCVBUF_ =
    makeOption("SO_RCVBUF", integerType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_REUSEADDR_ =
    makeOption("SO_REUSEADDR", booleanType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::SO_LINGER_ =
    makeOption("SO_LINGER", integerType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::IP_TOS_ =
    makeOption("IP_TOS", integerType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::IP_MULTICAST_IF_ =
    makeOption("IP_MULTICAST_IF", networkInterfaceType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::IP_MULTICAST_TTL_ =
    makeOption("IP_MULTICAST_TTL", integerType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::IP_MULTICAST_LOOP_ =
    makeOption("IP_MULTICAST_LOOP", booleanType());
::jxx::Ptr<StandardSocketOptions::Option>
StandardSocketOptions::TCP_NODELAY_ =
    makeOption("TCP_NODELAY", booleanType());

} // namespace jxx::net
