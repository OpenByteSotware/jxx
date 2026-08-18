#include <cstddef>
#include <cstring>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
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

#if defined(__linux__)
#include <netpacket/packet.h>
#endif

#endif

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "net/jxx.net.Inet4Address.h"
#include "net/jxx.net.Inet6Address.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InterfaceAddress.h"
#include "net/jxx.net.NetworkInterface.h"

#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.VectorEnumeration.h"

namespace {

    /**
     * Creates a Java-style byte[] from native memory.
     *
     * ByteArray is:
     *
     *     jxx::Ptr<ByteArrayType>
     *
     * Therefore ByteArrayType is the type that must be allocated.
     */
    jxx::lang::ByteArray makeByteArray_(
        const void* source,
        std::size_t length) {

        if (source == nullptr && length != 0U) {
            throw jxx::lang::NullPointerException();
        }

        auto result =
            jxx::NEW<jxx::lang::ByteArrayType>(
                static_cast<
                jxx::lang::ByteArrayType::size_type>(
                    length));

        const auto* bytes =
            static_cast<const unsigned char*>(
                source);

        for (std::size_t i = 0;
            i < length;
            ++i) {

            (*result)[
                static_cast<jxx::lang::jint>(i)] =
                static_cast<jxx::lang::jbyte>(
                    bytes[i]);
        }

        return result;
    }

#if defined(_WIN32)

    /**
     * Converts a Windows SOCKET_ADDRESS to a JXX InetAddress.
     */
    jxx::Ptr<jxx::net::InetAddress>
        makeInetAddress_(
            const SOCKET_ADDRESS& address) {

        if (address.lpSockaddr == nullptr) {
            return nullptr;
        }

        if (address.lpSockaddr->sa_family == AF_INET) {
            const auto* socketAddress =
                reinterpret_cast<
                const SOCKADDR_IN*>(
                    address.lpSockaddr);

            auto addressBytes =
                makeByteArray_(
                    &socketAddress->sin_addr,
                    sizeof(socketAddress->sin_addr));

            char text[INET_ADDRSTRLEN] = {};

            if (::inet_ntop(
                AF_INET,
                &socketAddress->sin_addr,
                text,
                sizeof(text)) == nullptr) {

                return nullptr;
            }

            return jxx::NEW<
                jxx::net::Inet4Address>(
                    nullptr,
                    jxx::NEW<jxx::lang::String>(
                        text),
                    addressBytes);
        }

        if (address.lpSockaddr->sa_family == AF_INET6) {
            const auto* socketAddress =
                reinterpret_cast<
                const SOCKADDR_IN6*>(
                    address.lpSockaddr);

            auto addressBytes =
                makeByteArray_(
                    &socketAddress->sin6_addr,
                    sizeof(socketAddress->sin6_addr));

            char text[INET6_ADDRSTRLEN] = {};

            if (::inet_ntop(
                AF_INET6,
                &socketAddress->sin6_addr,
                text,
                sizeof(text)) == nullptr) {

                return nullptr;
            }

            return jxx::NEW<
                jxx::net::Inet6Address>(
                    nullptr,
                    jxx::NEW<jxx::lang::String>(
                        text),
                    addressBytes,
                    static_cast<jxx::lang::jint>(
                        socketAddress->sin6_scope_id),
                    nullptr);
        }

        return nullptr;
    }

    /**
     * Converts a Windows wide string to a narrow UTF-8-like string.
     *
     * Replace this with a JXX Unicode conversion helper if one is
     * available in your runtime.
     */
    std::string narrowString_(
        const wchar_t* value) {

        if (value == nullptr) {
            return {};
        }

        const int required =
            ::WideCharToMultiByte(
                CP_UTF8,
                0,
                value,
                -1,
                nullptr,
                0,
                nullptr,
                nullptr);

        if (required <= 1) {
            return {};
        }

        std::string result(
            static_cast<std::size_t>(
                required - 1),
            '\0');

        ::WideCharToMultiByte(
            CP_UTF8,
            0,
            value,
            -1,
            result.data(),
            required,
            nullptr,
            nullptr);

        return result;
    }

#else

    /**
     * Converts a POSIX sockaddr to a JXX InetAddress.
     */
    jxx::Ptr<jxx::net::InetAddress>
        makeInetAddress_(
            const sockaddr* address) {

        if (address == nullptr) {
            return nullptr;
        }

        if (address->sa_family == AF_INET) {
            const auto* socketAddress =
                reinterpret_cast<
                const sockaddr_in*>(
                    address);

            auto addressBytes =
                makeByteArray_(
                    &socketAddress->sin_addr,
                    sizeof(socketAddress->sin_addr));

            char text[INET_ADDRSTRLEN] = {};

            if (::inet_ntop(
                AF_INET,
                &socketAddress->sin_addr,
                text,
                sizeof(text)) == nullptr) {

                return nullptr;
            }

            return jxx::NEW<
                jxx::net::Inet4Address>(
                    nullptr,
                    jxx::NEW<jxx::lang::String>(
                        std::string(text)),
                    addressBytes);
        }

        if (address->sa_family == AF_INET6) {
            const auto* socketAddress =
                reinterpret_cast<
                const sockaddr_in6*>(
                    address);

            auto addressBytes =
                makeByteArray_(
                    &socketAddress->sin6_addr,
                    sizeof(socketAddress->sin6_addr));

            char text[INET6_ADDRSTRLEN] = {};

            if (::inet_ntop(
                AF_INET6,
                &socketAddress->sin6_addr,
                text,
                sizeof(text)) == nullptr) {

                return nullptr;
            }

            return jxx::NEW<
                jxx::net::Inet6Address>(
                    nullptr,
                    jxx::NEW<jxx::lang::String>(
                        std::string(text)),
                    addressBytes,
                    static_cast<jxx::lang::jint>(
                        socketAddress->sin6_scope_id),
                    nullptr);
        }

        return nullptr;
    }

    /**
     * Calculates the prefix length from a POSIX netmask.
     */
    jxx::lang::jshort prefixLength_(
        const sockaddr* netmask) {

        if (netmask == nullptr) {
            return 0;
        }

        jxx::lang::jshort prefix = 0;

        if (netmask->sa_family == AF_INET) {
            const auto* address =
                reinterpret_cast<
                const sockaddr_in*>(
                    netmask);

            const auto* bytes =
                reinterpret_cast<
                const unsigned char*>(
                    &address->sin_addr);

            for (std::size_t i = 0;
                i < sizeof(address->sin_addr);
                ++i) {

                unsigned char value = bytes[i];

                for (int bit = 7;
                    bit >= 0;
                    --bit) {

                    if ((value &
                        static_cast<unsigned char>(
                            1U << bit)) != 0U) {

                        ++prefix;
                    }
                    else {
                        return prefix;
                    }
                }
            }

            return prefix;
        }

        if (netmask->sa_family == AF_INET6) {
            const auto* address =
                reinterpret_cast<
                const sockaddr_in6*>(
                    netmask);

            const auto* bytes =
                reinterpret_cast<
                const unsigned char*>(
                    &address->sin6_addr);

            for (std::size_t i = 0;
                i < sizeof(address->sin6_addr);
                ++i) {

                unsigned char value = bytes[i];

                for (int bit = 7;
                    bit >= 0;
                    --bit) {

                    if ((value &
                        static_cast<unsigned char>(
                            1U << bit)) != 0U) {

                        ++prefix;
                    }
                    else {
                        return prefix;
                    }
                }
            }
        }

        return prefix;
    }

#endif

} // namespace

namespace jxx::net {

    jxx::Ptr<NetworkInterface>
        NetworkInterface::getParent() const {
        return nullptr;
    }

    jxx::Ptr<jxx::lang::String>
        NetworkInterface::getName() const {
        return name_;
    }

    jxx::Ptr<jxx::lang::String>
        NetworkInterface::getDisplayName() const {
        return displayName_;
    }

    jxx::lang::jint
        NetworkInterface::getIndex() const noexcept {
        return index_;
    }

    jxx::lang::jbool
        NetworkInterface::isUp() const {
        return isUp_;
    }

    jxx::lang::jbool
        NetworkInterface::isLoopback() const {
        return isLoopback_;
    }

    jxx::lang::jbool
        NetworkInterface::isPointToPoint() const {
        return isPtP_;
    }

    jxx::lang::jbool
        NetworkInterface::supportsMulticast() const {
        return supportsMulticast_;
    }

    jxx::lang::jbool
        NetworkInterface::isVirtual() const {
        return isVirtual_;
    }

    jxx::lang::jint
        NetworkInterface::getMTU() const {
        return mtu_;
    }

    jxx::lang::ByteArray
        NetworkInterface::getHardwareAddress() const {
        return hardwareAddr_;
    }

    jxx::Ptr<jxx::lang::String>
        NetworkInterface::toString() const {
        return jxx::NEW<jxx::lang::String>(
            name_ != nullptr
            ? name_->utf8()
            : std::string());
    }

    jxx::Ptr<NetworkInterface>
        NetworkInterface::fromName_(
            const std::string& name,
            jxx::lang::jint indexHint) {

        auto result =
            jxx::NEW<NetworkInterface>();

        result->name_ =
            jxx::NEW<jxx::lang::String>(
                name);

        result->displayName_ =
            result->name_;

        result->index_ =
            indexHint;

#if defined(_WIN32)

        internal::ensureNetworkInitialized();

        constexpr ULONG flags =
            GAA_FLAG_INCLUDE_PREFIX;

        constexpr ULONG family =
            AF_UNSPEC;

        ULONG bufferSize = 0;

        const ULONG initialResult =
            ::GetAdaptersAddresses(
                family,
                flags,
                nullptr,
                nullptr,
                &bufferSize);

        if (initialResult !=
            ERROR_BUFFER_OVERFLOW &&
            initialResult != NO_ERROR) {

            throw std::runtime_error(
                "GetAdaptersAddresses size query failed");
        }

        std::vector<unsigned char> buffer(
            static_cast<std::size_t>(
                bufferSize));

        auto* adapters =
            reinterpret_cast<
            IP_ADAPTER_ADDRESSES*>(
                buffer.data());

        const ULONG queryResult =
            ::GetAdaptersAddresses(
                family,
                flags,
                nullptr,
                adapters,
                &bufferSize);

        if (queryResult != NO_ERROR) {
            throw std::runtime_error(
                "GetAdaptersAddresses failed");
        }

        for (auto* adapter = adapters;
            adapter != nullptr;
            adapter = adapter->Next) {

            const std::string adapterName =
                adapter->AdapterName != nullptr
                ? std::string(
                    adapter->AdapterName)
                : std::string();

            const std::string friendlyName =
                narrowString_(
                    adapter->FriendlyName);

            if (adapterName != name &&
                friendlyName != name) {

                continue;
            }

            const ULONG nativeIndex =
                adapter->IfIndex != 0
                ? adapter->IfIndex
                : adapter->Ipv6IfIndex;

            result->index_ =
                indexHint > 0
                ? indexHint
                : static_cast<
                jxx::lang::jint>(
                    nativeIndex);

            if (!friendlyName.empty()) {
                result->displayName_ =
                    jxx::NEW<
                    jxx::lang::String>(
                        friendlyName);
            }

            result->isLoopback_ =
                static_cast<jxx::lang::jbool>(
                    adapter->IfType ==
                    IF_TYPE_SOFTWARE_LOOPBACK);

            result->supportsMulticast_ =
                static_cast<jxx::lang::jbool>(
                    adapter->NoMulticast == 0);

            result->isUp_ =
                static_cast<jxx::lang::jbool>(
                    adapter->OperStatus ==
                    IfOperStatusUp);

            result->mtu_ =
                static_cast<jxx::lang::jint>(
                    adapter->Mtu);

            if (adapter->PhysicalAddressLength > 0) {
                result->hardwareAddr_ =
                    makeByteArray_(
                        adapter->PhysicalAddress,
                        static_cast<std::size_t>(
                            adapter->
                            PhysicalAddressLength));
            }

            for (auto* unicast =
                adapter->FirstUnicastAddress;
                unicast != nullptr;
                unicast = unicast->Next) {

                auto inetAddress =
                    makeInetAddress_(
                        unicast->Address);

                if (inetAddress == nullptr) {
                    continue;
                }

                result->inetAddresses_.
                    push_back(inetAddress);

                result->interfaceAddresses_.
                    push_back(
                        jxx::NEW<
                        InterfaceAddress>(
                            inetAddress,
                            nullptr,
                            static_cast<
                            jxx::lang::jshort>(
                                unicast->
                                OnLinkPrefixLength)));
            }

            break;
        }

#else

        ifaddrs* addressList = nullptr;

        if (::getifaddrs(&addressList) != 0) {
            throw std::runtime_error(
                "getifaddrs failed");
        }

        for (auto* current = addressList;
            current != nullptr;
            current = current->ifa_next) {

            if (current->ifa_name == nullptr ||
                name != current->ifa_name) {

                continue;
            }

            result->index_ =
                indexHint > 0
                ? indexHint
                : static_cast<
                jxx::lang::jint>(
                    ::if_nametoindex(
                        current->ifa_name));

            result->isUp_ =
                static_cast<jxx::lang::jbool>(
                    (current->ifa_flags &
                        IFF_UP) != 0);

            result->isLoopback_ =
                static_cast<jxx::lang::jbool>(
                    (current->ifa_flags &
                        IFF_LOOPBACK) != 0);

            result->isPtP_ =
                static_cast<jxx::lang::jbool>(
                    (current->ifa_flags &
                        IFF_POINTOPOINT) != 0);

            result->supportsMulticast_ =
                static_cast<jxx::lang::jbool>(
                    (current->ifa_flags &
                        IFF_MULTICAST) != 0);

            auto inetAddress =
                makeInetAddress_(
                    current->ifa_addr);

            if (inetAddress != nullptr) {
                result->inetAddresses_.
                    push_back(inetAddress);
            }

            auto broadcastAddress =
                makeInetAddress_(
                    current->ifa_broadaddr);

            const jxx::lang::jshort prefix =
                prefixLength_(
                    current->ifa_netmask);

            if (inetAddress != nullptr) {
                result->interfaceAddresses_.
                    push_back(
                        jxx::NEW<
                        InterfaceAddress>(
                            inetAddress,
                            broadcastAddress,
                            prefix));
            }
        }

        ::freeifaddrs(addressList);

        const int socketDescriptor =
            ::socket(
                AF_INET,
                SOCK_DGRAM,
                0);

        if (socketDescriptor >= 0) {
            ifreq request{};

            std::strncpy(
                request.ifr_name,
                name.c_str(),
                IFNAMSIZ - 1);

            request.ifr_name[
                IFNAMSIZ - 1] = '\0';

            if (::ioctl(
                socketDescriptor,
                SIOCGIFMTU,
                &request) == 0) {

                result->mtu_ =
                    static_cast<jxx::lang::jint>(
                        request.ifr_mtu);
            }

#if defined(__linux__)

            if (::ioctl(
                socketDescriptor,
                SIOCGIFHWADDR,
                &request) == 0) {

                constexpr std::size_t
                    hardwareAddressLength = 6U;

                result->hardwareAddr_ =
                    makeByteArray_(
                        request.ifr_hwaddr.sa_data,
                        hardwareAddressLength);
            }

#endif

            ::close(socketDescriptor);
        }

#endif

        return result;
    }

    jxx::Ptr<
        jxx::util::Enumeration<InetAddress>>
        NetworkInterface::getInetAddresses() const {

        return jxx::NEW<
            jxx::util::VectorEnumeration<
            InetAddress>>(
                inetAddresses_);
    }

    jxx::Ptr<NetworkInterface>
        NetworkInterface::getByName(
            jxx::Ptr<jxx::lang::String> name) {

        if (name == nullptr) {
            throw jxx::lang::
                NullPointerException();
        }

#if defined(_WIN32)

        auto interfaces =
            getNetworkInterfaces();

        while (interfaces != nullptr &&
            interfaces->hasMoreElements()) {

            auto networkInterface =
                interfaces->nextElement();

            if (networkInterface == nullptr) {
                continue;
            }

            const auto interfaceName =
                networkInterface->getName();

            const auto displayName =
                networkInterface->
                getDisplayName();

            if ((interfaceName != nullptr &&
                interfaceName->equals(name)) ||
                (displayName != nullptr &&
                    displayName->equals(name))) {

                return networkInterface;
            }
        }

        return nullptr;

#else

        const unsigned int index =
            ::if_nametoindex(
                name->utf8().c_str());

        if (index == 0U) {
            return nullptr;
        }

        return fromName_(
            name->utf8(),
            static_cast<jxx::lang::jint>(
                index));

#endif
    }

    jxx::Ptr<NetworkInterface>
        NetworkInterface::getByIndex(
            jxx::lang::jint index) {

        if (index <= 0) {
            throw jxx::lang::
                IllegalArgumentException();
        }

#if defined(_WIN32)

        auto interfaces =
            getNetworkInterfaces();

        while (interfaces != nullptr &&
            interfaces->hasMoreElements()) {

            auto networkInterface =
                interfaces->nextElement();

            if (networkInterface != nullptr &&
                networkInterface->getIndex() ==
                index) {

                return networkInterface;
            }
        }

        return nullptr;

#else

        char nameBuffer[IF_NAMESIZE] = {};

        if (::if_indextoname(
            static_cast<unsigned int>(
                index),
            nameBuffer) == nullptr) {

            return nullptr;
        }

        return fromName_(
            std::string(nameBuffer),
            index);

#endif
    }

    jxx::Ptr<NetworkInterface>
        NetworkInterface::getByInetAddress(
            jxx::Ptr<InetAddress> address) {

        if (address == nullptr) {
            throw jxx::lang::
                NullPointerException();
        }

        auto interfaces =
            getNetworkInterfaces();

        while (interfaces != nullptr &&
            interfaces->hasMoreElements()) {

            auto networkInterface =
                interfaces->nextElement();

            if (networkInterface == nullptr) {
                continue;
            }

            auto addresses =
                networkInterface->
                getInetAddresses();

            while (addresses != nullptr &&
                addresses->hasMoreElements()) {

                auto candidate =
                    addresses->nextElement();

                if (candidate != nullptr &&
                    candidate->equals(address)) {

                    return networkInterface;
                }
            }
        }

        return nullptr;
    }

    jxx::Ptr<
        jxx::util::Enumeration<
        NetworkInterface>>
        NetworkInterface::getNetworkInterfaces() {

        std::map<
            std::string,
            jxx::Ptr<NetworkInterface>>
            interfacesByName;

#if defined(_WIN32)

        internal::ensureNetworkInitialized();

        constexpr ULONG flags =
            GAA_FLAG_INCLUDE_PREFIX;

        constexpr ULONG family =
            AF_UNSPEC;

        ULONG bufferSize = 0;

        const ULONG initialResult =
            ::GetAdaptersAddresses(
                family,
                flags,
                nullptr,
                nullptr,
                &bufferSize);

        if (initialResult !=
            ERROR_BUFFER_OVERFLOW &&
            initialResult != NO_ERROR) {

            throw std::runtime_error(
                "GetAdaptersAddresses size query failed");
        }

        std::vector<unsigned char> buffer(
            static_cast<std::size_t>(
                bufferSize));

        auto* adapters =
            reinterpret_cast<
            IP_ADAPTER_ADDRESSES*>(
                buffer.data());

        const ULONG queryResult =
            ::GetAdaptersAddresses(
                family,
                flags,
                nullptr,
                adapters,
                &bufferSize);

        if (queryResult != NO_ERROR) {
            throw std::runtime_error(
                "GetAdaptersAddresses failed");
        }

        for (auto* adapter = adapters;
            adapter != nullptr;
            adapter = adapter->Next) {

            const std::string adapterName =
                adapter->AdapterName != nullptr
                ? std::string(
                    adapter->AdapterName)
                : std::string();

            if (adapterName.empty()) {
                continue;
            }

            const ULONG nativeIndex =
                adapter->IfIndex != 0
                ? adapter->IfIndex
                : adapter->Ipv6IfIndex;

            auto networkInterface =
                fromName_(
                    adapterName,
                    static_cast<
                    jxx::lang::jint>(
                        nativeIndex));

            if (networkInterface != nullptr) {
                interfacesByName.emplace(
                    adapterName,
                    networkInterface);
            }
        }

#else

        ifaddrs* addressList = nullptr;

        if (::getifaddrs(&addressList) != 0) {
            throw std::runtime_error(
                "getifaddrs failed");
        }

        /*
         * Collect names first. Calling fromName_() while addressList is
         * active would cause nested getifaddrs() calls.
         */
        std::map<std::string, unsigned int>
            interfaceNames;

        for (auto* current = addressList;
            current != nullptr;
            current = current->ifa_next) {

            if (current->ifa_name == nullptr) {
                continue;
            }

            const std::string interfaceName(
                current->ifa_name);

            interfaceNames.emplace(
                interfaceName,
                ::if_nametoindex(
                    current->ifa_name));
        }

        ::freeifaddrs(addressList);

        for (const auto& item :
            interfaceNames) {

            auto networkInterface =
                fromName_(
                    item.first,
                    static_cast<
                    jxx::lang::jint>(
                        item.second));

            if (networkInterface != nullptr) {
                interfacesByName.emplace(
                    item.first,
                    networkInterface);
            }
        }

#endif

        std::vector<
            jxx::Ptr<NetworkInterface>>
            result;

        result.reserve(
            interfacesByName.size());

        for (const auto& item :
            interfacesByName) {

            result.push_back(item.second);
        }

        return jxx::NEW<
            jxx::util::VectorEnumeration<
            NetworkInterface>>(
                std::move(result));
    }

    jxx::Ptr<jxx::util::List<InterfaceAddress>> NetworkInterface::getInterfaceAddresses() const {

        auto result = jxx::NEW<jxx::util::ArrayList<InterfaceAddress>>();

        for (const auto& address :
            interfaceAddresses_) {

            result->add(address);
        }

        return jxx::CAST<jxx::util::List<InterfaceAddress>>(result);
    }

    jxx::Ptr<jxx::util::Enumeration<NetworkInterface>> NetworkInterface::getSubInterfaces() const {

        std::vector<
            jxx::Ptr<NetworkInterface>>
            empty;

        return jxx::NEW<
            jxx::util::VectorEnumeration<
            NetworkInterface>>(
                std::move(empty));
    }

    jxx::lang::jbool
        NetworkInterface::equals(
            jxx::Ptr<jxx::lang::Object> other)
        const {

        auto networkInterface =
            jxx::CAST<NetworkInterface>(
                other);

        if (networkInterface == nullptr) {
            return static_cast<
                jxx::lang::jbool>(false);
        }

        if (name_ == nullptr ||
            networkInterface->name_ == nullptr) {

            return static_cast<
                jxx::lang::jbool>(
                    name_ ==
                    networkInterface->name_);
        }

        return name_->equals(
            networkInterface->name_);
    }

    jxx::lang::jint
        NetworkInterface::hashCode() const {
        return name_ != nullptr
            ? name_->hashCode()
            : 0;
    }

} // namespace jxx::net