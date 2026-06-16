#include "net/jxx.net.InetSocketAddress.h"

#include <stdexcept>

#include "net/jxx.net.InetAddress.h"

namespace jxx::net
{
    InetSocketAddress::InetSocketAddress(jxx::lang::jint port)
        : InetSocketAddress(InetAddress::getLoopbackAddress(), nullptr, port, false)
    {
    }

    InetSocketAddress::InetSocketAddress(jxx::Ptr<InetAddress> addr,
                                         jxx::lang::jint port)
        : InetSocketAddress(std::move(addr), nullptr, port, false)
    {
    }

    InetSocketAddress::InetSocketAddress(jxx::Ptr<jxx::lang::String> hostname,
                                         jxx::lang::jint port)
        : InetSocketAddress(nullptr, std::move(hostname), port, false)
    {
        if (host_)
        {
            try
            {
                addr_ = InetAddress::getByName(host_);
            }
            catch (...)
            {
                unresolved_ = true;
            }
        }
    }

    InetSocketAddress::InetSocketAddress(jxx::Ptr<InetAddress> addr,
                                         jxx::Ptr<jxx::lang::String> host,
                                         jxx::lang::jint port,
                                         jxx::lang::jbool unresolved)
        : addr_(std::move(addr)),
          host_(std::move(host)),
          port_(port),
          unresolved_(unresolved)
    {
        if (port < 0 || port > 65535)
            throw std::invalid_argument("port out of range");
    }

    jxx::Ptr<InetSocketAddress> InetSocketAddress::createUnresolved(jxx::Ptr<jxx::lang::String> host,
                                                                    jxx::lang::jint port)
    {
        return std::shared_ptr<InetSocketAddress>(new InetSocketAddress(nullptr, std::move(host), port, true));
    }

    jxx::lang::jint InetSocketAddress::getPort() const noexcept { return port_; }
    jxx::Ptr<InetAddress> InetSocketAddress::getAddress() const { return addr_; }
    jxx::Ptr<jxx::lang::String> InetSocketAddress::getHostName() const { return host_ ? host_ : (addr_ ? addr_->getHostName() : nullptr); }
    jxx::Ptr<jxx::lang::String> InetSocketAddress::getHostString() const { return getHostName(); }
    jxx::lang::jbool InetSocketAddress::isUnresolved() const noexcept { return unresolved_; }

    jxx::Ptr<jxx::lang::String> InetSocketAddress::toString() const
    {
        const auto host = getHostString();
        return jxx::NEW<jxx::lang::String>((host ? host->utf8() : std::string()) + ":" + std::to_string(port_));
    }

    jxx::lang::jbool InetSocketAddress::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto o = std::dynamic_pointer_cast<InetSocketAddress>(other);
        if (!o || port_ != o->port_ || unresolved_ != o->unresolved_)
            return false;
        if (addr_ && o->addr_)
            return addr_->equals(o->addr_);
        if (!addr_ && !o->addr_)
            return ((!host_ && !o->host_) || (host_ && o->host_ && host_->equals(o->host_)));
        return false;
    }

    jxx::lang::jint InetSocketAddress::hashCode() const
    {
        return port_ + (host_ ? host_->hashCode() : 0) + (addr_ ? addr_->hashCode() : 0);
    }
}
