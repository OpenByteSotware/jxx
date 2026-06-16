#include "net/jxx.net.InetAddress.h"

#if defined(_WIN32)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "net/jxx.net.Inet4Address.h"
#include "net/jxx.net.Inet6Address.h"
#include "net/jxx.net.UnknownHostException.h"

namespace
{
    inline jxx::lang::ByteArray toByteArray_(const std::vector<jxx::lang::jbyte>& bytes)
    {
        auto out = jxx::NEW<jxx::lang::ByteArrayType>(static_cast<jxx::lang::jint>(bytes.size()));
        for (std::size_t i = 0; i < bytes.size(); ++i)
            (*out)[static_cast<jxx::lang::jint>(i)] = bytes[i];
        return out;
    }

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

    inline std::string toPrintable_(const std::vector<jxx::lang::jbyte>& bytes, int family)
    {
        char buf[INET6_ADDRSTRLEN] = {0};
        if (family == AF_INET && bytes.size() == 4)
        {
            ::inet_ntop(AF_INET, bytes.data(), buf, sizeof(buf));
            return std::string(buf);
        }
        if (family == AF_INET6 && bytes.size() == 16)
        {
            ::inet_ntop(AF_INET6, bytes.data(), buf, sizeof(buf));
            return std::string(buf);
        }
        return {};
    }

    inline jxx::Ptr<jxx::net::InetAddress> createInet_(jxx::Ptr<jxx::lang::String> host,
                                                       const std::vector<jxx::lang::jbyte>& bytes,
                                                       int family)
    {
        auto printable = jxx::NEW<jxx::lang::String>(toPrintable_(bytes, family));
        if (family == AF_INET)
            return jxx::NEW<jxx::net::Inet4Address>(host, printable, bytes);
        if (family == AF_INET6)
            return jxx::NEW<jxx::net::Inet6Address>(host, printable, bytes, 0, nullptr);
        throw jxx::net::UnknownHostException("unsupported address family");
    }
}

namespace jxx::net
{
    InetAddress::InetAddress(jxx::Ptr<jxx::lang::String> hostName,
                             jxx::Ptr<jxx::lang::String> hostAddress,
                             const std::vector<jxx::lang::jbyte>& bytes,
                             jxx::lang::jint family)
        : hostName_(std::move(hostName)),
          hostAddress_(std::move(hostAddress)),
          bytes_(bytes),
          family_(family)
    {
    }

    jxx::Ptr<InetAddress> InetAddress::getByAddress(const jxx::lang::ByteArray addr)
    {
        return getByAddress(nullptr, addr);
    }

    jxx::Ptr<InetAddress> InetAddress::getByAddress(jxx::Ptr<jxx::lang::String> host,
                                                    const jxx::lang::ByteArray addr)
    {
        auto bytes = fromByteArray_(addr);
        if (bytes.size() == 4)
            return jxx::NEW<Inet4Address>(host, jxx::NEW<jxx::lang::String>(toPrintable_(bytes, AF_INET)), bytes);
        if (bytes.size() == 16)
            return jxx::NEW<Inet6Address>(host, jxx::NEW<jxx::lang::String>(toPrintable_(bytes, AF_INET6)), bytes, 0, nullptr);
        throw UnknownHostException("invalid address length");
    }

    jxx::Ptr<InetAddress> InetAddress::getByName(jxx::Ptr<jxx::lang::String> host)
    {
        auto all = getAllByName(host);
        if (!all || all->size() == 0)
            throw UnknownHostException("host not found");
        return (*all)(0);
    }

    jxx::Ptr<jxx::JxxArray<jxx::Ptr<InetAddress>, 1U>> InetAddress::getAllByName(jxx::Ptr<jxx::lang::String> host)
    {
        internal::ensureNetworkInitialized();

        const std::string name = host ? host->utf8() : std::string();
        addrinfo hints{};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        addrinfo* result = nullptr;

        const int rc = ::getaddrinfo(name.empty() ? nullptr : name.c_str(), nullptr, &hints, &result);
        if (rc != 0 || !result)
        {
        #if defined(_WIN32)
            throw UnknownHostException("getaddrinfo failed");
        #else
            throw UnknownHostException(gai_strerror(rc));
        #endif
        }

        std::vector<jxx::Ptr<InetAddress>> addrs;
        for (auto* p = result; p; p = p->ai_next)
        {
            if (p->ai_family == AF_INET)
            {
                auto* sa = reinterpret_cast<sockaddr_in*>(p->ai_addr);
                std::vector<jxx::lang::jbyte> bytes(4);
                std::memcpy(bytes.data(), &sa->sin_addr, 4);
                addrs.push_back(createInet_(host, bytes, AF_INET));
            }
            else if (p->ai_family == AF_INET6)
            {
                auto* sa = reinterpret_cast<sockaddr_in6*>(p->ai_addr);
                std::vector<jxx::lang::jbyte> bytes(16);
                std::memcpy(bytes.data(), &sa->sin6_addr, 16);
                addrs.push_back(createInet_(host, bytes, AF_INET6));
            }
        }
        ::freeaddrinfo(result);

        auto out = jxx::NEW<jxx::JxxArray<jxx::Ptr<InetAddress>, 1U>>(static_cast<jxx::lang::jint>(addrs.size()));
        for (std::size_t i = 0; i < addrs.size(); ++i)
            (*out)(static_cast<jxx::lang::jint>(i)) = addrs[i];
        return out;
    }

    jxx::Ptr<InetAddress> InetAddress::getLoopbackAddress()
    {
        try
        {
            return getByName(jxx::NEW<jxx::lang::String>("localhost"));
        }
        catch (...)
        {
            auto arr = jxx::NEW<jxx::lang::ByteArrayType>(4);
            (*arr)[0] = 127; (*arr)[1] = 0; (*arr)[2] = 0; (*arr)[3] = 1;
            return getByAddress(jxx::NEW<jxx::lang::String>("localhost"), arr);
        }
    }

    jxx::Ptr<InetAddress> InetAddress::getLocalHost()
    {
        internal::ensureNetworkInitialized();
        char name[256] = {0};
        if (::gethostname(name, sizeof(name) - 1) != 0)
            throw UnknownHostException("gethostname failed");
        return getByName(jxx::NEW<jxx::lang::String>(std::string(name)));
    }

    jxx::Ptr<jxx::lang::String> InetAddress::getHostName() const { return hostName_ ? hostName_ : hostAddress_; }
    jxx::Ptr<jxx::lang::String> InetAddress::getCanonicalHostName() const { return getHostName(); }
    jxx::lang::ByteArray InetAddress::getAddress() const { return toByteArray_(bytes_); }
    jxx::Ptr<jxx::lang::String> InetAddress::getHostAddress() const { return hostAddress_; }

    jxx::lang::jbool InetAddress::isMulticastAddress() const { return false; }
    jxx::lang::jbool InetAddress::isAnyLocalAddress() const { return false; }
    jxx::lang::jbool InetAddress::isLoopbackAddress() const { return false; }
    jxx::lang::jbool InetAddress::isLinkLocalAddress() const { return false; }
    jxx::lang::jbool InetAddress::isSiteLocalAddress() const { return false; }
    jxx::lang::jbool InetAddress::isMCGlobal() const { return false; }
    jxx::lang::jbool InetAddress::isMCNodeLocal() const { return false; }
    jxx::lang::jbool InetAddress::isMCLinkLocal() const { return false; }
    jxx::lang::jbool InetAddress::isMCSiteLocal() const { return false; }
    jxx::lang::jbool InetAddress::isMCOrgLocal() const { return false; }

    jxx::lang::jbool InetAddress::isReachable(jxx::lang::jint /*timeout*/) const { return true; }
    jxx::lang::jbool InetAddress::isReachable(jxx::Ptr<NetworkInterface> /*netif*/, jxx::lang::jint /*ttl*/, jxx::lang::jint /*timeout*/) const { return true; }

    jxx::Ptr<jxx::lang::String> InetAddress::toString() const
    {
        const auto hn = getHostName();
        const auto ha = getHostAddress();
        return jxx::NEW<jxx::lang::String>((hn ? hn->utf8() : std::string()) + "/" + (ha ? ha->utf8() : std::string()));
    }

    jxx::lang::jbool InetAddress::equals(jxx::Ptr<jxx::lang::Object> other) const
    {
        auto a = std::dynamic_pointer_cast<InetAddress>(other);
        return a && bytes_ == a->bytes_ && family_ == a->family_;
    }

    jxx::lang::jint InetAddress::hashCode() const
    {
        jxx::lang::jint h = 1;
        for (auto b : bytes_)
            h = 31 * h + b;
        return h + family_;
    }

    jxx::lang::jint InetAddress::familyValue_() const noexcept { return family_; }
    const std::vector<jxx::lang::jbyte>& InetAddress::rawBytes_() const noexcept { return bytes_; }
}
