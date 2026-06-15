#include "net/jxx.net.NetworkInterface.h"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(_WIN32)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <ifaddrs.h>
    #include <net/if.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <cstring>
    #if defined(__linux__)
        #include <netpacket/packet.h>
    #endif
#endif

#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "net/jxx.net.Inet4Address.h"
#include "net/jxx.net.Inet6Address.h"
#include "net/jxx.net.InterfaceAddress.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.VectorEnumeration.h"

namespace
{
#if defined(_WIN32)
    inline std::vector<jxx::lang::jbyte> copyAddr4_(const SOCKADDR_IN* sa)
    {
        std::vector<jxx::lang::jbyte> bytes(4);
        std::memcpy(bytes.data(), &sa->sin_addr, 4);
        return bytes;
    }

    inline std::vector<jxx::lang::jbyte> copyAddr6_(const SOCKADDR_IN6* sa)
    {
        std::vector<jxx::lang::jbyte> bytes(16);
        std::memcpy(bytes.data(), &sa->sin6_addr, 16);
        return bytes;
    }

    inline jxx::Ptr<jxx::net::InetAddress> mkInetWin_(SOCKET_ADDRESS addr)
    {
        if (!addr.lpSockaddr)
            return nullptr;
        if (addr.lpSockaddr->sa_family == AF_INET)
        {
            auto* sa = reinterpret_cast<SOCKADDR_IN*>(addr.lpSockaddr);
            auto bytes = copyAddr4_(sa);
            char buf[INET_ADDRSTRLEN] = {0};
            ::inet_ntop(AF_INET, bytes.data(), buf, sizeof(buf));
            return std::make_shared<jxx::net::Inet4Address>(nullptr, std::make_shared<jxx::lang::String>(std::string(buf)), bytes);
        }
        if (addr.lpSockaddr->sa_family == AF_INET6)
        {
            auto* sa = reinterpret_cast<SOCKADDR_IN6*>(addr.lpSockaddr);
            auto bytes = copyAddr6_(sa);
            char buf[INET6_ADDRSTRLEN] = {0};
            ::inet_ntop(AF_INET6, bytes.data(), buf, sizeof(buf));
            return std::make_shared<jxx::net::Inet6Address>(nullptr, std::make_shared<jxx::lang::String>(std::string(buf)), bytes, static_cast<jxx::lang::jint>(sa->sin6_scope_id), nullptr);
        }
        return nullptr;
    }
#else
    inline std::vector<jxx::lang::jbyte> copyAddr4_(sockaddr_in* sa)
    {
        std::vector<jxx::lang::jbyte> bytes(4);
        std::memcpy(bytes.data(), &sa->sin_addr, 4);
        return bytes;
    }

    inline std::vector<jxx::lang::jbyte> copyAddr6_(sockaddr_in6* sa)
    {
        std::vector<jxx::lang::jbyte> bytes(16);
        std::memcpy(bytes.data(), &sa->sin6_addr, 16);
        return bytes;
    }

    inline jxx::Ptr<jxx::net::InetAddress> mkInetPosix_(sockaddr* sa)
    {
        if (!sa)
            return nullptr;
        if (sa->sa_family == AF_INET)
        {
            auto bytes = copyAddr4_(reinterpret_cast<sockaddr_in*>(sa));
            char buf[INET_ADDRSTRLEN] = {0};
            ::inet_ntop(AF_INET, bytes.data(), buf, sizeof(buf));
            return std::make_shared<jxx::net::Inet4Address>(nullptr, std::make_shared<jxx::lang::String>(std::string(buf)), bytes);
        }
        if (sa->sa_family == AF_INET6)
        {
            auto* sa6 = reinterpret_cast<sockaddr_in6*>(sa);
            auto bytes = copyAddr6_(sa6);
            char buf[INET6_ADDRSTRLEN] = {0};
            ::inet_ntop(AF_INET6, bytes.data(), buf, sizeof(buf));
            return std::make_shared<jxx::net::Inet6Address>(nullptr, std::make_shared<jxx::lang::String>(std::string(buf)), bytes, static_cast<jxx::lang::jint>(sa6->sin6_scope_id), nullptr);
        }
        return nullptr;
    }
#endif
}

namespace jxx::net
{
    jxx::Ptr<NetworkInterface> NetworkInterface::fromName_(const std::string& name,
                                                           jxx::lang::jint indexHint)
    {
        auto out = std::shared_ptr<NetworkInterface>(new NetworkInterface());
        out->name_ = std::make_shared<jxx::lang::String>(name);
        out->displayName_ = out->name_;
        out->index_ = indexHint;

    #if defined(_WIN32)
        internal::ensureNetworkInitialized();

        ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
        ULONG family = AF_UNSPEC;
        ULONG size = 0;
        ::GetAdaptersAddresses(family, flags, nullptr, nullptr, &size);
        std::vector<unsigned char> buf(size);
        auto* addrs = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buf.data());

        if (::GetAdaptersAddresses(family, flags, nullptr, addrs, &size) != NO_ERROR)
            throw std::runtime_error("GetAdaptersAddresses failed");

        for (auto* p = addrs; p; p = p->Next)
        {
            std::string adapterName = p->AdapterName ? std::string(p->AdapterName) : std::string();
            std::string friendlyName;
            if (p->FriendlyName)
            {
                std::wstring ws(p->FriendlyName);
                friendlyName.assign(ws.begin(), ws.end());
            }

            if (adapterName != name && friendlyName != name)
                continue;

            out->index_ = indexHint > 0 ? indexHint : static_cast<jxx::lang::jint>(p->IfIndex ? p->IfIndex : p->Ipv6IfIndex);
            if (!friendlyName.empty())
                out->displayName_ = std::make_shared<jxx::lang::String>(friendlyName);

            out->isLoopback_ = (p->IfType == IF_TYPE_SOFTWARE_LOOPBACK);
            out->supportsMulticast_ = !p->NoMulticast;
            out->isUp_ = (p->OperStatus == IfOperStatusUp);
            out->mtu_ = static_cast<jxx::lang::jint>(p->Mtu);

            if (p->PhysicalAddressLength > 0)
            {
                out->hardwareAddr_ = std::make_shared<jxx::JxxArray<jxx::lang::jbyte, 1U>>(static_cast<jxx::lang::jint>(p->PhysicalAddressLength));
                for (ULONG i = 0; i < p->PhysicalAddressLength; ++i)
                    (*out->hardwareAddr_)[static_cast<jxx::lang::jint>(i)] = static_cast<jxx::lang::jbyte>(p->PhysicalAddress[i]);
            }

            for (auto* ua = p->FirstUnicastAddress; ua; ua = ua->Next)
            {
                auto inet = mkInetWin_(ua->Address);
                if (inet)
                {
                    out->inetAddresses_.push_back(inet);
                    out->interfaceAddresses_.push_back(std::make_shared<InterfaceAddress>(inet, nullptr, static_cast<jxx::lang::jshort>(ua->OnLinkPrefixLength)));
                }
            }
            break;
        }
    #else
        ifaddrs* list = nullptr;
        if (::getifaddrs(&list) != 0)
            throw std::runtime_error("getifaddrs failed");

        for (auto* p = list; p; p = p->ifa_next)
        {
            if (!p->ifa_name || name != std::string(p->ifa_name))
                continue;

            out->index_ = indexHint > 0 ? indexHint : static_cast<jxx::lang::jint>(::if_nametoindex(p->ifa_name));
            out->isUp_ = (p->ifa_flags & IFF_UP) != 0;
            out->isLoopback_ = (p->ifa_flags & IFF_LOOPBACK) != 0;
            out->isPtP_ = (p->ifa_flags & IFF_POINTOPOINT) != 0;
            out->supportsMulticast_ = (p->ifa_flags & IFF_MULTICAST) != 0;

            auto inet = mkInetPosix_(p->ifa_addr);
            if (inet)
                out->inetAddresses_.push_back(inet);

            auto addr = mkInetPosix_(p->ifa_addr);
            auto bcast = mkInetPosix_(p->ifa_broadaddr);
            jxx::lang::jshort prefix = 0;
            if (p->ifa_netmask)
            {
                if (p->ifa_netmask->sa_family == AF_INET)
                {
                    auto* n = reinterpret_cast<sockaddr_in*>(p->ifa_netmask);
                    unsigned int mask = ntohl(n->sin_addr.s_addr);
                    while (mask)
                    {
                        prefix += static_cast<jxx::lang::jshort>(mask & 1U);
                        mask >>= 1U;
                    }
                }
                else if (p->ifa_netmask->sa_family == AF_INET6)
                {
                    auto* n = reinterpret_cast<sockaddr_in6*>(p->ifa_netmask);
                    const unsigned char* b = reinterpret_cast<const unsigned char*>(&n->sin6_addr);
                    for (int i = 0; i < 16; ++i)
                    {
                        unsigned char v = b[i];
                        while (v)
                        {
                            prefix += static_cast<jxx::lang::jshort>(v & 1U);
                            v >>= 1U;
                        }
                    }
                }
            }
            if (addr)
                out->interfaceAddresses_.push_back(std::make_shared<InterfaceAddress>(addr, bcast, prefix));
        }
        ::freeifaddrs(list);

        int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
        if (fd >= 0)
        {
            ifreq ifr{};
            std::strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ - 1);
            if (::ioctl(fd, SIOCGIFMTU, &ifr) == 0)
                out->mtu_ = ifr.ifr_mtu;

        #if defined(__linux__)
            if (::ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
            {
                out->hardwareAddr_ = std::make_shared<jxx::JxxArray<jxx::lang::jbyte, 1U>>(6);
                for (int i = 0; i < 6; ++i)
                    (*out->hardwareAddr_)[i] = static_cast<jxx::lang::jbyte>(ifr.ifr_hwaddr.sa_data[i]);
            }
        #endif
            ::close(fd);
        }
    #endif

        return out;
    }

    jxx::Ptr<NetworkInterface> NetworkInterface::getByName(jxx::Ptr<jxx::lang::String> name)
    {
        if (!name)
            return nullptr;
    #if defined(_WIN32)
        auto all = getNetworkInterfaces();
        while (all && all->hasMoreElements())
        {
            auto ni = all->nextElement();
            if ((ni->getName() && ni->getName()->equals(name)) ||
                (ni->getDisplayName() && ni->getDisplayName()->equals(name)))
                return ni;
        }
        return nullptr;
    #else
        const auto idx = ::if_nametoindex(name->utf8().c_str());
        if (idx == 0)
            return nullptr;
        return fromName_(name->utf8(), static_cast<jxx::lang::jint>(idx));
    #endif
    }

    jxx::Ptr<NetworkInterface> NetworkInterface::getByIndex(jxx::lang::jint index)
    {
        if (index <= 0)
            throw std::invalid_argument("index <= 0");
    #if defined(_WIN32)
        auto all = getNetworkInterfaces();
        while (all && all->hasMoreElements())
        {
            auto ni = all->nextElement();
            if (ni->getIndex() == index)
                return ni;
        }
        return nullptr;
    #else
        char buf[IF_NAMESIZE] = {0};
        if (!::if_indextoname(static_cast<unsigned int>(index), buf))
            return nullptr;
        return fromName_(buf, index);
    #endif
    }

    jxx::Ptr<NetworkInterface> NetworkInterface::getByInetAddress(jxx::Ptr<InetAddress> addr)
    {
        auto all = getNetworkInterfaces();
        while (all && all->hasMoreElements())
        {
            auto ni = all->nextElement();
            auto adds = ni->getInetAddresses();
            while (adds && adds->hasMoreElements())
            {
                auto a = adds->nextElement();
                if (addr && a && a->equals(addr))
                    return ni;
            }
        }
        return nullptr;
    }

    jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<NetworkInterface>>> NetworkInterface::getNetworkInterfaces()
    {
        std::map<std::string, jxx::Ptr<NetworkInterface>> map;

    #if defined(_WIN32)
        internal::ensureNetworkInitialized();
        ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
        ULONG family = AF_UNSPEC;
        ULONG size = 0;
        ::GetAdaptersAddresses(family, flags, nullptr, nullptr, &size);
        std::vector<unsigned char> buf(size);
        auto* addrs = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buf.data());

        if (::GetAdaptersAddresses(family, flags, nullptr, addrs, &size) != NO_ERROR)
            throw std::runtime_error("GetAdaptersAddresses failed");

        for (auto* p = addrs; p; p = p->Next)
        {
            std::string adapterName = p->AdapterName ? std::string(p->AdapterName) : std::string();
            if (adapterName.empty())
                continue;
            map.emplace(adapterName, fromName_(adapterName, static_cast<jxx::lang::jint>(p->IfIndex ? p->IfIndex : p->Ipv6IfIndex)));
        }
    #else
        ifaddrs* list = nullptr;
        if (::getifaddrs(&list) != 0)
            throw std::runtime_error("getifaddrs failed");

        for (auto* p = list; p; p = p->ifa_next)
        {
            if (!p->ifa_name)
                continue;
            const std::string name(p->ifa_name);
            if (map.find(name) == map.end())
                map.emplace(name, fromName_(name));
        }
        ::freeifaddrs(list);
    #endif

        std::vector<jxx::Ptr<NetworkInterface>> items;
        for (const auto& kv : map)
            items.push_back(kv.second);
        return std::make_shared<jxx::util::VectorEnumeration<jxx::Ptr<NetworkInterface>>>(std::move(items));
    }

    jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<InetAddress>>> NetworkInterface::getInetAddresses() const
    {
        return std::make_shared<jxx::util::VectorEnumeration<jxx::Ptr<InetAddress>>>(inetAddresses_);
    }

    jxx::Ptr<jxx::util::List<jxx::Ptr<InterfaceAddress>>> NetworkInterface::getInterfaceAddresses() const
    {
        auto list = std::make_shared<jxx::util::ArrayList<jxx::Ptr<InterfaceAddress>>>();
        for (const auto& x : interfaceAddresses_)
            list->add(x);
        return list;
    }

    jxx::Ptr<jxx::util::Enumeration<jxx::Ptr<NetworkInterface>>> NetworkInterface::getSubInterfaces() const
    {
        return std::make_shared<jxx::util::VectorEnumeration<jxx::Ptr<NetworkInterface>>>(std::vector<jxx::Ptr<NetworkInterface>>{});
    }

    jxx::Ptr<NetworkInterface> NetworkInterface::getParent() const { return nullptr; }
    jxx::Ptr<jxx::lang::String> NetworkInterface::getName() const { return name_; }
    jxx::Ptr<jxx::lang::String> NetworkInterface::getDisplayName() const { return displayName_; }
    jxx::lang::jint NetworkInterface::getIndex() const noexcept { return index_; }
    jxx::lang::jbool NetworkInterface::isUp() const { return isUp_; }
    jxx::lang::jbool NetworkInterface::isLoopback() const { return isLoopback_; }
    jxx::lang::jbool NetworkInterface::isPointToPoint() const { return isPtP_; }
    jxx::lang::jbool NetworkInterface::supportsMulticast() const { return supportsMulticast_; }
    jxx::lang::jbool NetworkInterface::isVirtual() const { return isVirtual_; }
    jxx::lang::jint NetworkInterface::getMTU() const { return mtu_; }
    jxx::lang::ByteArray NetworkInterface::getHardwareAddress() const { return hardwareAddr_; }

    jxx::Ptr<jxx::lang::String> NetworkInterface::toString() const
    {
        return std::make_shared<jxx::lang::String>(name_ ? name_->utf8() : std::string());
    }

    jxx::lang::jbool NetworkInterface::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto o = std::dynamic_pointer_cast<NetworkInterface>(other);
        return o && name_ && o->name_ && name_->equals(o->name_);
    }

    jxx::lang::jint NetworkInterface::hashCode() const
    {
        return name_ ? name_->hashCode() : 0;
    }
}
