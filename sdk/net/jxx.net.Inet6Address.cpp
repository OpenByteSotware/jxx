#include "net/jxx.net.Inet6Address.h"

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
#endif

#include <stdexcept>

namespace
{
    inline unsigned char u_(jxx::lang::jbyte b) { return static_cast<unsigned char>(b); }

    inline std::vector<jxx::lang::jbyte> fromByteArray_(const jxx::lang::ByteArray arr)
    {
        std::vector<jxx::lang::jbyte> out;
        if (!arr)
            return out;
        out.reserve(static_cast<std::size_t>(arr->length));
        for (jxx::lang::jint i = 0; i < arr->length; ++i)
            out.push_back((*arr)[i]);
        return out;
    }

    inline std::string toPrintable6_(const std::vector<jxx::lang::jbyte>& bytes)
    {
        char buf[INET6_ADDRSTRLEN] = {0};
        ::inet_ntop(AF_INET6, bytes.data(), buf, sizeof(buf));
        return std::string(buf);
    }
}

namespace jxx::net
{
    Inet6Address::Inet6Address(jxx::Ptr<jxx::lang::String> hostName,
                               jxx::Ptr<jxx::lang::String> hostAddress,
                               const std::vector<jxx::lang::jbyte>& bytes,
                               jxx::lang::jint scopeId,
                               jxx::Ptr<NetworkInterface> scopedIf)
        : InetAddress(std::move(hostName), std::move(hostAddress), bytes, AF_INET6),
          scopeId_(scopeId),
          scopedIf_(std::move(scopedIf))
    {
    }

    jxx::Ptr<Inet6Address> Inet6Address::getByAddress(jxx::Ptr<jxx::lang::String> host,
                                                      const jxx::lang::ByteArray addr,
                                                      jxx::lang::jint scopeId)
    {
        auto bytes = fromByteArray_(addr);
        if (bytes.size() != 16)
            throw std::invalid_argument("invalid IPv6 address length");
        return jxx::NEW<Inet6Address>(host, jxx::NEW<jxx::lang::String>(toPrintable6_(bytes)), bytes, scopeId, nullptr);
    }

    jxx::Ptr<Inet6Address> Inet6Address::getByAddress(jxx::Ptr<jxx::lang::String> host,
                                                      const jxx::lang::ByteArray addr,
                                                      jxx::Ptr<NetworkInterface> nif)
    {
        auto bytes = fromByteArray_(addr);
        if (bytes.size() != 16)
            throw std::invalid_argument("invalid IPv6 address length");
        return jxx::NEW<Inet6Address>(host, jxx::NEW<jxx::lang::String>(toPrintable6_(bytes)), bytes, 0, std::move(nif));
    }

    jxx::lang::jint Inet6Address::getScopeId() const noexcept { return scopeId_; }
    jxx::Ptr<NetworkInterface> Inet6Address::getScopedInterface() const { return scopedIf_; }

    jxx::lang::jbool Inet6Address::isIPv4CompatibleAddress() const
    {
        if (bytes_.size() != 16)
            return false;
        for (int i = 0; i < 12; ++i)
        {
            if (bytes_[i] != 0)
                return false;
        }
        return true;
    }

    jxx::lang::jbool Inet6Address::isMulticastAddress() const { return bytes_.size() == 16 && u_(bytes_[0]) == 0xFFU; }

    jxx::lang::jbool Inet6Address::isAnyLocalAddress() const
    {
        if (bytes_.size() != 16)
            return false;
        for (auto b : bytes_)
            if (b != 0)
                return false;
        return true;
    }

    jxx::lang::jbool Inet6Address::isLoopbackAddress() const
    {
        if (bytes_.size() != 16)
            return false;
        for (int i = 0; i < 15; ++i)
            if (bytes_[i] != 0)
                return false;
        return bytes_[15] == 1;
    }

    jxx::lang::jbool Inet6Address::isLinkLocalAddress() const { return bytes_.size() == 16 && u_(bytes_[0]) == 0xFEU && ((u_(bytes_[1]) & 0xC0U) == 0x80U); }
    jxx::lang::jbool Inet6Address::isSiteLocalAddress() const { return bytes_.size() == 16 && u_(bytes_[0]) == 0xFEU && ((u_(bytes_[1]) & 0xC0U) == 0xC0U); }
    jxx::lang::jbool Inet6Address::isMCGlobal() const { return isMulticastAddress() && ((u_(bytes_[1]) & 0x0FU) == 0x0EU); }
    jxx::lang::jbool Inet6Address::isMCNodeLocal() const { return isMulticastAddress() && ((u_(bytes_[1]) & 0x0FU) == 0x01U); }
    jxx::lang::jbool Inet6Address::isMCLinkLocal() const { return isMulticastAddress() && ((u_(bytes_[1]) & 0x0FU) == 0x02U); }
    jxx::lang::jbool Inet6Address::isMCSiteLocal() const { return isMulticastAddress() && ((u_(bytes_[1]) & 0x0FU) == 0x05U); }
    jxx::lang::jbool Inet6Address::isMCOrgLocal() const { return isMulticastAddress() && ((u_(bytes_[1]) & 0x0FU) == 0x08U); }
}
