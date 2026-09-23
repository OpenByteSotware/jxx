#include "net/jxx.net.InetSocketAddress.h"

#include <stdexcept>

#include "net/jxx.net.InetAddress.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::net
{
    InetSocketAddress::InetSocketAddress(jxx::lang::jint port)
        : InetSocketAddress(InetAddress::getByAddress(
              ::jxx::NEW<::jxx::lang::ByteArrayType>(4)), nullptr, port, false)
    {
    }

    InetSocketAddress::InetSocketAddress(const jxx::Ptr<InetAddress>& addr,
                                         jxx::lang::jint port)
        : InetSocketAddress(addr != nullptr
              ? addr
              : InetAddress::getByAddress(::jxx::NEW<::jxx::lang::ByteArrayType>(4)),
          nullptr, port, false)
    {
    }

    InetSocketAddress::InetSocketAddress(const jxx::Ptr<jxx::lang::String>& hostname,
                                         jxx::lang::jint port)
        : InetSocketAddress(nullptr, std::move(hostname), port, false)
    {
        if (host_ == nullptr) {
            throw jxx::lang::NullPointerException();
        }
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

    InetSocketAddress::InetSocketAddress(const jxx::Ptr<InetAddress>& addr,
                                         const jxx::Ptr<jxx::lang::String>& host,
                                         jxx::lang::jint port,
                                         jxx::lang::jbool unresolved)
        : addr_(std::move(addr)),
          host_(std::move(host)),
          port_(port),
          unresolved_(unresolved)
    {
        if (port < 0 || port > 65535)
            throw jxx::lang::IllegalArgumentException("port out of range");
    }

    jxx::Ptr<InetSocketAddress> InetSocketAddress::createUnresolved(const jxx::Ptr<jxx::lang::String>& host,
                                                                    jxx::lang::jint port)
    {
        if (host == nullptr) {
            throw jxx::lang::NullPointerException();
        }
        return std::shared_ptr<InetSocketAddress>(
            new InetSocketAddress(nullptr, host, port, true));
    }

    jxx::lang::jint InetSocketAddress::getPort() const noexcept { return port_; }
    jxx::Ptr<InetAddress> InetSocketAddress::getAddress() const { return addr_; }
    jxx::Ptr<jxx::lang::String> InetSocketAddress::getHostName() const { return host_ ? host_ : (addr_ ? addr_->getHostName() : nullptr); }
    jxx::Ptr<jxx::lang::String> InetSocketAddress::getHostString() const { return getHostName(); }
    jxx::lang::jbool InetSocketAddress::isUnresolved() const noexcept { return unresolved_; }

    jxx::Ptr<jxx::lang::String> InetSocketAddress::toString() const
    {
        const auto host = getHostString();
        std::string value = host != nullptr ? host->utf8() : std::string();
        if (addr_ != nullptr && value.find(':') != std::string::npos &&
            (value.empty() || value.front() != '[')) {
            value = "[" + value + "]";
        }
        return jxx::NEW<jxx::lang::String>(value + ":" + std::to_string(port_));
    }

    jxx::lang::jbool InetSocketAddress::equals(const jxx::Ptr<jxx::lang::Object>& other) const
    {
        auto o = std::dynamic_pointer_cast<InetSocketAddress>(other);
        if (!o || port_ != o->port_ || unresolved_ != o->unresolved_)
            return false;
        if (addr_ && o->addr_)
            return addr_->equals(o->addr_);
        if (!addr_ && !o->addr_)
            return ((!host_ && !o->host_) || (host_ && o->host_ && host_->equalsIgnoreCase(o->host_)));
        return false;
    }

    jxx::lang::jint InetSocketAddress::hashCode() const
    {
        jxx::lang::jint hostHash = 0;
        if (host_ != nullptr) {
            hostHash = host_->toLowerCase()->hashCode();
        }
        return port_ + hostHash + (addr_ != nullptr ? addr_->hashCode() : 0);
    }
}
