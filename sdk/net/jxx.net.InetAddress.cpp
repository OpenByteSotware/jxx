#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "net/jxx.net.Inet4Address.h"
#include "net/jxx.net.Inet6Address.h"
#include "net/jxx.net.NetworkInterface.h"
#include "net/jxx.net.UnknownHostException.h"
#include "util/jxx.util.Enumeration.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"

namespace
{

	inline unsigned int inetByte_(::jxx::lang::jbyte value)
	{
		return static_cast<unsigned int>(static_cast<unsigned char>(value));
	}

	bool inetSockaddr_(const ::jxx::lang::ByteArray& bytes, unsigned short port,
		sockaddr_storage& storage, socklen_t& length)
	{
		std::memset(&storage, 0, sizeof(storage));
		if (bytes == nullptr) return false;
		if (bytes->length == 4) {
			auto* out = reinterpret_cast<sockaddr_in*>(&storage);
			out->sin_family = AF_INET;
			out->sin_port = htons(port);
			unsigned char raw[4]{};
			for (::jxx::lang::jint i = 0; i < 4; ++i) raw[i] = static_cast<unsigned char>((*bytes)[i]);
			std::memcpy(&out->sin_addr, raw, sizeof(raw));
			length = static_cast<socklen_t>(sizeof(sockaddr_in));
			return true;
		}
		if (bytes->length == 16) {
			auto* out = reinterpret_cast<sockaddr_in6*>(&storage);
			out->sin6_family = AF_INET6;
			out->sin6_port = htons(port);
			unsigned char raw[16]{};
			for (::jxx::lang::jint i = 0; i < 16; ++i) raw[i] = static_cast<unsigned char>((*bytes)[i]);
			std::memcpy(&out->sin6_addr, raw, sizeof(raw));
			length = static_cast<socklen_t>(sizeof(sockaddr_in6));
			return true;
		}
		return false;
	}

	int inetLastError_() noexcept
	{
#if defined(_WIN32)
		return ::WSAGetLastError();
#else
		return errno;
#endif
	}

	bool inetPending_(int error) noexcept
	{
#if defined(_WIN32)
		return error == WSAEWOULDBLOCK || error == WSAEINPROGRESS || error == WSAEALREADY;
#else
		return error == EINPROGRESS || error == EWOULDBLOCK || error == EAGAIN || error == EALREADY;
#endif
	}

	bool inetRefused_(int error) noexcept
	{
#if defined(_WIN32)
		return error == WSAECONNREFUSED;
#else
		return error == ECONNREFUSED;
#endif
	}

	bool inetNonBlocking_(::jxx::net::internal::NativeSocket socket) noexcept
	{
#if defined(_WIN32)
		u_long enabled = 1UL;
		return ::ioctlsocket(socket, FIONBIO, &enabled) == 0;
#else
		const int flags = ::fcntl(socket, F_GETFL, 0);
		return flags >= 0 && ::fcntl(socket, F_SETFL, flags | O_NONBLOCK) == 0;
#endif
	}

	bool inetBindInterface_(::jxx::net::internal::NativeSocket socket, int family,
		const ::jxx::Ptr<::jxx::net::NetworkInterface>& networkInterface)
	{
		if (networkInterface == nullptr) return true;
		const auto all = networkInterface->getInetAddresses();
		if (all == nullptr) return false;
		while (all->hasMoreElements()) {
			const auto current = all->nextElement();
			if (current == nullptr) continue;
			const auto bytes = current->getAddress();
			if (bytes == nullptr || (family == AF_INET && bytes->length != 4) ||
				(family == AF_INET6 && bytes->length != 16)) continue;
			sockaddr_storage source{};
			socklen_t sourceLength = 0;
			if (!inetSockaddr_(bytes, 0, source, sourceLength)) continue;
			return ::bind(socket, reinterpret_cast<const sockaddr*>(&source), sourceLength) == 0;
		}
		return false;
	}

	bool inetProbe_(const ::jxx::lang::ByteArray& bytes,
		const ::jxx::Ptr<::jxx::net::NetworkInterface>& networkInterface,
		::jxx::lang::jint ttl, ::jxx::lang::jint timeout)
	{
		sockaddr_storage target{};
		socklen_t targetLength = 0;
		if (!inetSockaddr_(bytes, 7, target, targetLength)) return false;
		const int family = bytes->length == 4 ? AF_INET : AF_INET6;
		::jxx::net::internal::ensureNetworkInitialized();
		const auto socket = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
		if (socket == ::jxx::net::internal::kInvalidSocket) return false;
		struct Closer final
		{
			::jxx::net::internal::NativeSocket socket;
			~Closer()
			{
				::jxx::net::internal::closeNativeSocket(socket);
			}
		} closer{ socket };
		if (ttl > 0) {
			const int nativeTtl = ttl > 255 ? 255 : static_cast<int>(ttl);
			const int level = family == AF_INET ? IPPROTO_IP : IPPROTO_IPV6;
			const int option = family == AF_INET ? IP_TTL : IPV6_UNICAST_HOPS;
			(void)::setsockopt(socket, level, option,
				reinterpret_cast<const char*>(&nativeTtl), sizeof(nativeTtl));
		}
		if (!inetBindInterface_(socket, family, networkInterface) || !inetNonBlocking_(socket)) return false;
		if (::connect(socket, reinterpret_cast<const sockaddr*>(&target), targetLength) == 0) return true;
		const int error = inetLastError_();
		if (inetRefused_(error)) return true;
		if (!inetPending_(error)) return false;
		fd_set writeSet;
		fd_set errorSet;
		FD_ZERO(&writeSet);
		FD_ZERO(&errorSet);
		FD_SET(socket, &writeSet);
		FD_SET(socket, &errorSet);
		timeval interval{};
		timeval* intervalPointer = nullptr;
		if (timeout > 0) {
			interval.tv_sec = timeout / 1000;
			interval.tv_usec = (timeout % 1000) * 1000;
			intervalPointer = &interval;
		}
#if defined(_WIN32)
		const int selected = ::select(0, nullptr, &writeSet, &errorSet, intervalPointer);
#else
		const int selected = ::select(socket + 1, nullptr, &writeSet, &errorSet, intervalPointer);
#endif
		if (selected <= 0) return false;
		int completion = 0;
		socklen_t completionLength = static_cast<socklen_t>(sizeof(completion));
		if (::getsockopt(socket, SOL_SOCKET, SO_ERROR,
			reinterpret_cast<char*>(&completion), &completionLength) != 0) return false;
		return completion == 0 || inetRefused_(completion);
	}

	inline bool isNumericAddress_(const std::string& value)
	{
		if (value.empty()) return false;
		in_addr ipv4{};
		if (::inet_pton(AF_INET, value.c_str(), &ipv4) == 1) return true;
		in6_addr ipv6{};
		return ::inet_pton(AF_INET6, value.c_str(), &ipv6) == 1;
	}

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
		char buf[INET6_ADDRSTRLEN] = { 0 };
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

	inline jxx::Ptr<jxx::net::InetAddress> createInet_(const jxx::Ptr<jxx::lang::String> host,
													   const std::vector<jxx::lang::jbyte>& bytes,
													   int family)
	{
		auto printable = jxx::NEW<jxx::lang::String>(toPrintable_(bytes, family));
		auto byteArray = toByteArray_(bytes);
		if (family == AF_INET)
			return jxx::NEW<jxx::net::Inet4Address>(host, printable, byteArray);
		if (family == AF_INET6)
			return jxx::NEW<jxx::net::Inet6Address>(host, printable, byteArray, 0, nullptr);
		throw jxx::net::UnknownHostException("unsupported address family");
	}
}

namespace jxx::net
{
	InetAddress::InetAddress(const jxx::Ptr<jxx::lang::String>& hostName,
		const jxx::Ptr<jxx::lang::String>& hostAddress,
		const jxx::lang::ByteArray& bytes,
		jxx::lang::jint family)
		: hostName_(hostName),
		hostAddress_(hostAddress),
		bytes_(copyAddressBytes_(bytes)),
		family_(family)
	{
	}

	jxx::Ptr<InetAddress> InetAddress::getByAddress(const jxx::lang::ByteArray& addr)
	{
		return getByAddress(nullptr, addr);
	}

	jxx::Ptr<InetAddress> InetAddress::getByAddress(const jxx::Ptr<jxx::lang::String>& host,
		const jxx::lang::ByteArray& addr)
	{
		if (addr == nullptr) {
			throw jxx::lang::NullPointerException();
		}

		auto bytes = fromByteArray_(addr);
		if (bytes.size() == 4)
			return jxx::NEW<Inet4Address>(host, jxx::NEW<jxx::lang::String>(toPrintable_(bytes, AF_INET)), addr);
		if (bytes.size() == 16)
			return jxx::NEW<Inet6Address>(host, jxx::NEW<jxx::lang::String>(toPrintable_(bytes, AF_INET6)), addr, 0, nullptr);
		throw UnknownHostException("invalid address length");
	}

	jxx::Ptr<InetAddress> InetAddress::getByName(const jxx::Ptr<jxx::lang::String>& host)
	{
		auto all = getAllByName(host);
		if (!all || all->size() == 0)
			throw UnknownHostException("host not found");
		return (*all)(0);
	}

	jxx::Ptr<jxx::JxxArray<jxx::Ptr<InetAddress>, 1U>> InetAddress::getAllByName(const jxx::Ptr<jxx::lang::String>& host)
	{
		internal::ensureNetworkInitialized();

		const std::string name = host ? host->utf8() : std::string();
		const auto resultHost = isNumericAddress_(name)
			? ::jxx::Ptr<::jxx::lang::String>()
			: host;
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
				addrs.push_back(createInet_(resultHost, bytes, AF_INET));
			}
			else if (p->ai_family == AF_INET6)
			{
				auto* sa = reinterpret_cast<sockaddr_in6*>(p->ai_addr);
				std::vector<jxx::lang::jbyte> bytes(16);
				std::memcpy(bytes.data(), &sa->sin6_addr, 16);
				addrs.push_back(createInet_(resultHost, bytes, AF_INET6));
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
		auto address =
			jxx::NEW<jxx::lang::ByteArrayType>(4);

		(*address)[0] = 127;
		(*address)[1] = 0;
		(*address)[2] = 0;
		(*address)[3] = 1;

		return getByAddress(
			jxx::NEW<jxx::lang::String>(
				"localhost"),
			address);
	}

	jxx::Ptr<InetAddress> InetAddress::getLocalHost()
	{
		internal::ensureNetworkInitialized();
		char name[256] = { 0 };
		if (::gethostname(name, sizeof(name) - 1) != 0)
			throw UnknownHostException("gethostname failed");
		return getByName(jxx::NEW<jxx::lang::String>(std::string(name)));
	}

	jxx::Ptr<jxx::lang::String> InetAddress::getHostName() const
	{
		return hostName_ ? hostName_ : hostAddress_;
	}
	jxx::Ptr<jxx::lang::String> InetAddress::getCanonicalHostName() const
	{
		return getHostName();
	}
	jxx::lang::ByteArray InetAddress::getAddress() const
	{
		return copyAddressBytes_(bytes_);
	}
	jxx::Ptr<jxx::lang::String> InetAddress::getHostAddress() const
	{
		return hostAddress_;
	}

	jxx::lang::jbool InetAddress::isMulticastAddress() const
	{
		return bytes_ != nullptr && ((bytes_->length == 4 && inetByte_((*bytes_)[0]) >= 224U && inetByte_((*bytes_)[0]) <= 239U) || (bytes_->length == 16 && inetByte_((*bytes_)[0]) == 0xFFU));
	}
	jxx::lang::jbool InetAddress::isAnyLocalAddress() const
	{
		if (bytes_ == nullptr || (bytes_->length != 4 && bytes_->length != 16)) return false; for (::jxx::lang::jint i = 0; i < bytes_->length; ++i) if ((*bytes_)[i] != 0) return false; return true;
	}
	jxx::lang::jbool InetAddress::isLoopbackAddress() const
	{
		if (bytes_ == nullptr) return false; if (bytes_->length == 4) return inetByte_((*bytes_)[0]) == 127U; if (bytes_->length != 16) return false; for (::jxx::lang::jint i = 0; i < 15; ++i) if ((*bytes_)[i] != 0) return false; return inetByte_((*bytes_)[15]) == 1U;
	}
	jxx::lang::jbool InetAddress::isLinkLocalAddress() const
	{
		return bytes_ != nullptr && ((bytes_->length == 4 && inetByte_((*bytes_)[0]) == 169U && inetByte_((*bytes_)[1]) == 254U) || (bytes_->length == 16 && inetByte_((*bytes_)[0]) == 0xFEU && (inetByte_((*bytes_)[1]) & 0xC0U) == 0x80U));
	}
	jxx::lang::jbool InetAddress::isSiteLocalAddress() const
	{
		if (bytes_ == nullptr) return false; if (bytes_->length == 16) return inetByte_((*bytes_)[0]) == 0xFEU && (inetByte_((*bytes_)[1]) & 0xC0U) == 0xC0U; if (bytes_->length != 4) return false; const auto a = inetByte_((*bytes_)[0]); const auto b = inetByte_((*bytes_)[1]); return a == 10U || (a == 172U && b >= 16U && b <= 31U) || (a == 192U && b == 168U);
	}
	jxx::lang::jbool InetAddress::isMCGlobal() const
	{
		if (!isMulticastAddress()) return false; if (bytes_->length == 16) return (inetByte_((*bytes_)[1]) & 0x0FU) == 0x0EU; const auto a = inetByte_((*bytes_)[0]); return a >= 224U && a <= 238U && !(a == 224U && inetByte_((*bytes_)[1]) == 0U && inetByte_((*bytes_)[2]) == 0U);
	}
	jxx::lang::jbool InetAddress::isMCNodeLocal() const
	{
		return isMulticastAddress() && bytes_->length == 16 && (inetByte_((*bytes_)[1]) & 0x0FU) == 1U;
	}
	jxx::lang::jbool InetAddress::isMCLinkLocal() const
	{
		if (!isMulticastAddress()) return false; return bytes_->length == 16 ? (inetByte_((*bytes_)[1]) & 0x0FU) == 2U : inetByte_((*bytes_)[0]) == 224U && inetByte_((*bytes_)[1]) == 0U && inetByte_((*bytes_)[2]) == 0U;
	}
	jxx::lang::jbool InetAddress::isMCSiteLocal() const
	{
		if (!isMulticastAddress()) return false; return bytes_->length == 16 ? (inetByte_((*bytes_)[1]) & 0x0FU) == 5U : inetByte_((*bytes_)[0]) == 239U && inetByte_((*bytes_)[1]) == 255U;
	}
	jxx::lang::jbool InetAddress::isMCOrgLocal() const
	{
		if (!isMulticastAddress()) return false; if (bytes_->length == 16) return (inetByte_((*bytes_)[1]) & 0x0FU) == 8U; const auto b = inetByte_((*bytes_)[1]); return inetByte_((*bytes_)[0]) == 239U && b >= 192U && b <= 195U;
	}

	jxx::lang::jbool InetAddress::isReachable(jxx::lang::jint timeout) const
	{
		return isReachable(nullptr, 0, timeout);
	}

	jxx::lang::jbool InetAddress::isReachable(
		const jxx::Ptr<NetworkInterface>& netif,
		jxx::lang::jint ttl,
		jxx::lang::jint timeout) const
	{
		if (ttl < 0 || timeout < 0) throw jxx::lang::IllegalArgumentException();
		if (isAnyLocalAddress() || isLoopbackAddress()) return true;
		return inetProbe_(bytes_, netif, ttl, timeout);
	}

	jxx::Ptr<jxx::lang::String> InetAddress::toString() const
	{
		const auto ha = getHostAddress();
		return jxx::NEW<jxx::lang::String>(
			(hostName_ ? hostName_->utf8() : std::string()) +
			"/" +
			(ha ? ha->utf8() : std::string()));
	}

	jxx::lang::jbool InetAddress::equals(const jxx::Ptr<jxx::lang::Object>& other) const
	{
		const auto address = jxx::CAST<InetAddress>(other);
		if (address == nullptr || family_ != address->family_ ||
			bytes_ == nullptr || address->bytes_ == nullptr ||
			bytes_->length != address->bytes_->length) {
			return false;
		}

		for (jxx::lang::jint i = 0; i < bytes_->length; ++i) {
			if ((*bytes_)[i] != (*address->bytes_)[i]) {
				return false;
			}
		}
		return true;
	}

	jxx::lang::jint InetAddress::hashCode() const
	{
		if (bytes_ == nullptr) {
			return 0;
		}

		jxx::lang::jint result = 0;
		for (jxx::lang::jint i = 0; i < bytes_->length; ++i) {
			result = 31 * result +
				static_cast<unsigned char>((*bytes_)[i]);
		}
		return result;
	}

	jxx::lang::jint InetAddress::familyValue_() const noexcept
	{
		return family_;
	}

	jxx::lang::jbyte InetAddress::byteAt_(jxx::lang::jint index) const
	{
		return bytes_->at(index);
	}

	jxx::lang::ByteArray InetAddress::rawBytes_() const noexcept
	{
		return bytes_;
	}

	jxx::lang::ByteArray InetAddress::copyAddressBytes_(
		jxx::lang::ByteArray bytes)
	{
		if (bytes == nullptr) {
			return nullptr;
		}

		auto copy = jxx::NEW<jxx::lang::ByteArrayType>(bytes->length);
		for (jxx::lang::jint i = 0; i < bytes->length; ++i) {
			(*copy)[i] = (*bytes)[i];
		}
		return copy;
	}
}
