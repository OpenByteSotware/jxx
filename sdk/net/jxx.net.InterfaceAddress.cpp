#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InterfaceAddress.h"

namespace jxx::net
{
    InterfaceAddress::InterfaceAddress(jxx::Ptr<InetAddress> address,
                                       jxx::Ptr<InetAddress> broadcast,
                                       jxx::lang::jshort prefixLength)
        : address_(std::move(address)),
          broadcast_(std::move(broadcast)),
          prefixLength_(prefixLength)
    {
    }

    jxx::Ptr<InetAddress> InterfaceAddress::getAddress() const { return address_; }
    jxx::Ptr<InetAddress> InterfaceAddress::getBroadcast() const { return broadcast_; }
    jxx::lang::jshort InterfaceAddress::getNetworkPrefixLength() const noexcept { return prefixLength_; }

    jxx::Ptr<jxx::lang::String> InterfaceAddress::toString() const
    {
        return jxx::NEW<jxx::lang::String>(
            (address_ ? address_->getHostAddress()->utf8() : std::string()) + "/" + std::to_string(prefixLength_));
    }
}
