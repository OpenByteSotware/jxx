
#include <cerrno>
#include <cstring>
#include <string>
#include <vector>

#if !defined(_WIN32)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#endif

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.Inet6Address.h"
#include "net/jxx.net.Inet4Address.h"
#include "util/jxx.util.HashSet.h"
#include "net/jxx.net.StandardSocketOptions.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Thread.h"
#include "nio/channels/jxx.nio.channels.ClosedByInterruptException.h"
#include "lang/jxx.lang.Boolean.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "net/internal/jxx.net.internal.NativeSocketState.h"
#include "nio/channels/jxx.nio.channels.AlreadyConnectedException.h"
#include "nio/channels/jxx.nio.channels.ConnectionPendingException.h"
#include "nio/channels/jxx.nio.channels.NoConnectionPendingException.h"
#include "nio/channels/jxx.nio.channels.NotYetConnectedException.h"
#include "nio/channels/jxx.nio.channels.AlreadyBoundException.h"
#include "nio/channels/jxx.nio.channels.UnsupportedAddressTypeException.h"
#include "nio/channels/jxx.nio.channels.UnresolvedAddressException.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "nio/channels/jxx.nio.channels.IllegalBlockingModeException.h"

namespace jxx::nio::channels
{
	namespace
	{

		int lastSocketError()
		{
#if defined(_WIN32)
			return WSAGetLastError();
#else
			return errno;
#endif
		}

		bool wouldBlock(int error)
		{
#if defined(_WIN32)
			return error == WSAEWOULDBLOCK || error == WSAEINPROGRESS ||
				error == WSAEALREADY;
#else
			return error == EWOULDBLOCK || error == EAGAIN ||
				error == EINPROGRESS || error == EALREADY;
#endif
		}

		void wakeBlockedSocket(
			const std::weak_ptr<::jxx::net::internal::NativeSocketState>& weakState) noexcept {
			const auto state = weakState.lock();
			if (state == nullptr) return;
			::jxx::net::internal::NativeSocket socket = ::jxx::net::internal::kInvalidSocket;
			{
				std::lock_guard<std::mutex> lock(state->m);
				if (state->socket == ::jxx::net::internal::kInvalidSocket) return;
				socket = state->socket;
				state->socket = ::jxx::net::internal::kInvalidSocket;
				state->closed = true;
				state->inputShutdown = true;
				state->outputShutdown = true;
			}
#if defined(_WIN32)
			::shutdown(socket, SD_BOTH);
#else
			::shutdown(socket, SHUT_RDWR);
#endif
			::jxx::net::internal::closeNativeSocket(socket);
		}

		class InterruptRegistration final {
		public:
			explicit InterruptRegistration(
				const std::shared_ptr<::jxx::net::internal::NativeSocketState>& state)
				: thread_(::jxx::lang::Thread::currentThread()) {
				if (thread_ == nullptr) return;
				const std::weak_ptr<::jxx::net::internal::NativeSocketState> weakState(state);
				if (thread_->isInterrupted()) {
					wakeBlockedSocket(weakState);
					throw ::jxx::nio::channels::ClosedByInterruptException();
				}
				thread_->setParkWakeup_([weakState] { wakeBlockedSocket(weakState); });
			}
			~InterruptRegistration() { if (thread_ != nullptr) thread_->clearParkWakeup_(); }
			::jxx::lang::jbool interrupted() const { return thread_ != nullptr && thread_->isInterrupted(); }
		private:
			::jxx::Ptr<::jxx::lang::Thread> thread_;
		};

		std::unique_ptr<InterruptRegistration> registerInterrupt(
			const std::shared_ptr<::jxx::net::internal::NativeSocketState>& state,
			SocketChannel& channel) {
			try {
				return std::unique_ptr<InterruptRegistration>(new InterruptRegistration(state));
			} catch (const ::jxx::nio::channels::ClosedByInterruptException&) {
				channel.close();
				throw;
			}
		}

		void setBlocking(::jxx::net::internal::NativeSocket socket, bool blocking)
		{
#if defined(_WIN32)
			u_long mode = blocking ? 0UL : 1UL;
			if (ioctlsocket(socket, FIONBIO, &mode) != 0)
				throw ::jxx::io::IOException("could not configure socket blocking mode");
#else
			const int flags = fcntl(socket, F_GETFL, 0);
			if (flags < 0 || fcntl(socket, F_SETFL,
				blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK)) < 0)
				throw ::jxx::io::IOException("could not configure socket blocking mode");
#endif
		}

		addrinfo* resolve(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& remote)
		{
			if (remote == nullptr)
				throw ::jxx::nio::channels::UnsupportedAddressTypeException();
			if (remote->isUnresolved())
				throw ::jxx::nio::channels::UnresolvedAddressException();
			const auto host = remote->getHostString();
			if (host == nullptr || host->utf8().empty())
				throw ::jxx::lang::IllegalArgumentException("remote host is empty");
			addrinfo hints{};
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			addrinfo* result = nullptr;
			const auto service = std::to_string(remote->getPort());
			if (getaddrinfo(host->utf8().c_str(), service.c_str(), &hints, &result) != 0 ||
				result == nullptr)
				throw ::jxx::io::IOException("could not resolve remote address");
			return result;
		}


		::jxx::Ptr<::jxx::net::InetAddress> addressFromNative(
			const sockaddr_storage& storage)
		{
			if (storage.ss_family == AF_INET6) {
				const auto* address =
					reinterpret_cast<const sockaddr_in6*>(&storage);
				std::vector<::jxx::lang::jbyte> bytes(16U);
				std::memcpy(bytes.data(), &address->sin6_addr, bytes.size());
				char text[INET6_ADDRSTRLEN]{};
				::inet_ntop(AF_INET6, &address->sin6_addr, text, sizeof(text));
				return ::jxx::NEW<::jxx::net::Inet6Address>(
					nullptr,
					::jxx::NEW<::jxx::lang::String>(text),
					::jxx::NEW<::jxx::lang::ByteArrayType>(bytes),
					static_cast<::jxx::lang::jint>(address->sin6_scope_id),
					nullptr);
			}
			const auto* address =
				reinterpret_cast<const sockaddr_in*>(&storage);
			std::vector<::jxx::lang::jbyte> bytes(4U);
			std::memcpy(bytes.data(), &address->sin_addr, bytes.size());
			char text[INET_ADDRSTRLEN]{};
			::inet_ntop(AF_INET, &address->sin_addr, text, sizeof(text));
			return ::jxx::NEW<::jxx::net::Inet4Address>(
				nullptr,
				::jxx::NEW<::jxx::lang::String>(text),
				::jxx::NEW<::jxx::lang::ByteArrayType>(bytes));
		}

		::jxx::lang::jint portFromNative(
			const sockaddr_storage& storage)
		{
			return storage.ss_family == AF_INET6
				? static_cast<::jxx::lang::jint>(ntohs(
					reinterpret_cast<const sockaddr_in6*>(&storage)->sin6_port))
				: static_cast<::jxx::lang::jint>(ntohs(
					reinterpret_cast<const sockaddr_in*>(&storage)->sin_port));
		}

	} // namespace

	SocketChannel::SocketChannel()
		: state_(std::make_shared<::jxx::net::internal::NativeSocketState>())
	{
		::jxx::net::internal::ensureNetworkInitialized();
	}

	SocketChannel::SocketChannel(const ::jxx::Ptr<::jxx::net::Socket>& socket)
		: state_(socket == nullptr ? std::make_shared<::jxx::net::internal::NativeSocketState>() : socket->sharedNativeSocketState()), socketView_(socket), connected_(socket != nullptr && socket->isConnected()), bound_(socket != nullptr && socket->isBound()) {
		if (socket == nullptr) throw ::jxx::lang::NullPointerException();
	}


	::jxx::Ptr<SocketChannel> SocketChannel::open()
	{
		return ::jxx::nio::channels::spi::SelectorProvider::provider()
			->openSocketChannel();
	}

	::jxx::Ptr<SocketChannel> SocketChannel::open(
		const ::jxx::Ptr<::jxx::net::SocketAddress>& remote)
	{
		const auto channel = open();
		channel->connect(remote);
		return channel;
	}

	void SocketChannel::ensureSocket()
	{
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (state_->socket == ::jxx::net::internal::kInvalidSocket) {
			state_->socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (state_->socket == ::jxx::net::internal::kInvalidSocket)
				throw ::jxx::io::IOException("could not create socket channel");
			setBlocking(state_->socket, blocking_);
			applyPendingOptions(state_->socket);
		}
	}

	void SocketChannel::applyPendingOptions(
		::jxx::net::internal::NativeSocket socket) const
	{
		auto setInteger = [socket](int level, int option, int value) {
			if (::setsockopt(socket, level, option,
				reinterpret_cast<const char*>(&value), sizeof(value)) != 0)
				throw ::jxx::io::IOException("setOption failed");
		};
		if (keepAliveSet_) setInteger(SOL_SOCKET, SO_KEEPALIVE, keepAlive_ ? 1 : 0);
		if (reuseAddressSet_) setInteger(SOL_SOCKET, SO_REUSEADDR, reuseAddress_ ? 1 : 0);
		if (tcpNoDelaySet_) setInteger(IPPROTO_TCP, TCP_NODELAY, tcpNoDelay_ ? 1 : 0);
		if (sendBufferSizeSet_) setInteger(SOL_SOCKET, SO_SNDBUF, sendBufferSize_);
		if (receiveBufferSizeSet_) setInteger(SOL_SOCKET, SO_RCVBUF, receiveBufferSize_);
		if (trafficClassSet_) setInteger(IPPROTO_IP, IP_TOS, trafficClass_);
		if (lingerSet_) {
			linger value{};
			value.l_onoff = linger_ >= 0 ? 1 : 0;
			value.l_linger = linger_ >= 0 ? linger_ : 0;
			if (::setsockopt(socket, SOL_SOCKET, SO_LINGER,
				reinterpret_cast<const char*>(&value), sizeof(value)) != 0)
				throw ::jxx::io::IOException("setOption failed");
		}
	}

	void SocketChannel::implConfigureBlocking(::jxx::lang::jbool block)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (state_ != nullptr &&
			state_->socket != ::jxx::net::internal::kInvalidSocket)
			setBlocking(state_->socket, block);
		blocking_ = block;
	}




	::jxx::lang::jbool SocketChannel::connect(
		const ::jxx::Ptr<::jxx::net::SocketAddress>& remoteAddress)
	{
		if (remoteAddress == nullptr) throw ::jxx::lang::NullPointerException();
		const auto remote = ::jxx::CAST<::jxx::net::InetSocketAddress>(remoteAddress);
		if (remote == nullptr) throw ::jxx::nio::channels::UnsupportedAddressTypeException();
		::jxx::lang::jbool blocking;
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
			if (connected_) throw ::jxx::nio::channels::AlreadyConnectedException();
			if (pending_) throw ::jxx::nio::channels::ConnectionPendingException();
			blocking = blocking_;
		}
		addrinfo* addresses = resolve(remote);
		const std::shared_ptr<void> addressGuard(addresses, [](void* value) {
			freeaddrinfo(static_cast<addrinfo*>(value));
		});
		for (addrinfo* current = addresses; current != nullptr; current = current->ai_next) {
			const auto native = ::socket(current->ai_family, current->ai_socktype, current->ai_protocol);
			if (native == ::jxx::net::internal::kInvalidSocket) continue;
			{
				std::lock_guard<std::mutex> stateLock(state_->m);
				if (state_->socket != ::jxx::net::internal::kInvalidSocket)
					::jxx::net::internal::closeNativeSocket(state_->socket);
				state_->socket = native;
				state_->closed = false;
				state_->inputShutdown = false;
				state_->outputShutdown = false;
			}
			setBlocking(native, blocking);
			applyPendingOptions(native);
			std::unique_ptr<InterruptRegistration> interruptRegistration;
			if (blocking) interruptRegistration = registerInterrupt(state_, *this);
			const int result = ::connect(native, current->ai_addr,
				static_cast<socklen_t>(current->ai_addrlen));
			if (interruptRegistration != nullptr && interruptRegistration->interrupted()) {
				close();
				throw ::jxx::nio::channels::ClosedByInterruptException();
			}
			{
				std::lock_guard<std::mutex> lock(mutex_);
				if (!open_ || state_->closed) throw ::jxx::nio::channels::AsynchronousCloseException();
				if (result == 0) { connected_ = true; bound_ = true; pending_ = false; return true; }
				const int error = lastSocketError();
				if (!blocking && wouldBlock(error)) { pending_ = true; return false; }
			}
		}
		close();
		throw ::jxx::io::IOException("socket channel connect failed");
	}

	::jxx::lang::jbool SocketChannel::finishConnect()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (connected_) return true;
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (!pending_) throw ::jxx::nio::channels::NoConnectionPendingException();
		int error = 0;
#if defined(_WIN32)
		int length = sizeof(error);
		if (getsockopt(state_->socket, SOL_SOCKET, SO_ERROR,
			reinterpret_cast<char*>(&error), &length) != 0)
			throw ::jxx::io::IOException("finishConnect failed");
#else
		socklen_t length = sizeof(error);
		if (getsockopt(state_->socket, SOL_SOCKET, SO_ERROR, &error, &length) != 0)
			throw ::jxx::io::IOException("finishConnect failed");
#endif
		if (error == 0) {
			pending_ = false;
			connected_ = true;
			bound_ = true;
			return true;
		}
		if (wouldBlock(error)) return false;
		pending_ = false;
		throw ::jxx::io::IOException("socket channel connection failed");
	}

	::jxx::lang::jbool SocketChannel::isConnectionPending() const noexcept
	{
		return pending_;
	}
	::jxx::lang::jbool SocketChannel::isConnected() const noexcept
	{
		return connected_ && state_ != nullptr && !state_->closed;
	}


	::jxx::lang::jint SocketChannel::read(
		const ::jxx::Ptr<::jxx::nio::ByteBuffer> destination)
	{
		if (destination == nullptr) throw ::jxx::lang::NullPointerException();
		::jxx::net::internal::NativeSocket native;
		::jxx::lang::jbool blocking;
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
			if (!connected_) throw ::jxx::nio::channels::NotYetConnectedException();
			if (inputShutdown_ || state_->inputShutdown) return -1;
			native = state_->socket;
			blocking = blocking_;
		}
		const auto remaining = destination->remaining();
		if (remaining == 0) return 0;
		std::vector<unsigned char> bytes(static_cast<std::size_t>(remaining));
		std::unique_ptr<InterruptRegistration> interruptRegistration;
		if (blocking) interruptRegistration = registerInterrupt(state_, *this);
#if defined(_WIN32)
		const int count = ::recv(native, reinterpret_cast<char*>(bytes.data()), remaining, 0);
#else
		const int count = static_cast<int>(::recv(native, bytes.data(), remaining, 0));
#endif
		if (interruptRegistration != nullptr && interruptRegistration->interrupted()) {
			close();
			throw ::jxx::nio::channels::ClosedByInterruptException();
		}
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (!open_) throw ::jxx::nio::channels::AsynchronousCloseException();
		}
		if (count > 0) {
			for (int index = 0; index < count; ++index) destination->put(static_cast<::jxx::lang::jbyte>(bytes[index]));
			return count;
		}
		if (count == 0) return -1;
		const int error = lastSocketError();
		if (!blocking && wouldBlock(error)) return 0;
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (!open_) throw ::jxx::nio::channels::AsynchronousCloseException();
		}
		throw ::jxx::io::IOException("socket channel read failed");
	}

	::jxx::lang::jint SocketChannel::write(
		const ::jxx::Ptr<::jxx::nio::ByteBuffer> source)
	{
		if (source == nullptr) throw ::jxx::lang::NullPointerException();
		::jxx::net::internal::NativeSocket native;
		::jxx::lang::jbool blocking;
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
			if (!connected_) throw ::jxx::nio::channels::NotYetConnectedException();
			if (outputShutdown_ || state_->outputShutdown) throw ::jxx::nio::channels::ClosedChannelException();
			native = state_->socket;
			blocking = blocking_;
		}
		const auto remaining = source->remaining();
		if (remaining == 0) return 0;
		const auto position = source->position();
		std::vector<unsigned char> bytes(static_cast<std::size_t>(remaining));
		for (::jxx::lang::jint index = 0; index < remaining; ++index) bytes[static_cast<std::size_t>(index)] = static_cast<unsigned char>(source->get(position + index));
		std::unique_ptr<InterruptRegistration> interruptRegistration;
		if (blocking) interruptRegistration = registerInterrupt(state_, *this);
#if defined(_WIN32)
		const int count = ::send(native, reinterpret_cast<const char*>(bytes.data()), remaining, 0);
#else
		const int count = static_cast<int>(::send(native, bytes.data(), remaining, 0));
#endif
		if (interruptRegistration != nullptr && interruptRegistration->interrupted()) {
			close();
			throw ::jxx::nio::channels::ClosedByInterruptException();
		}
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (!open_) throw ::jxx::nio::channels::AsynchronousCloseException();
		}
		if (count > 0) { source->position(position + count); return count; }
		const int error = lastSocketError();
		if (!blocking && wouldBlock(error)) return 0;
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (!open_ || state_->closed || outputShutdown_ || state_->outputShutdown)
				throw ::jxx::nio::channels::AsynchronousCloseException();
		}
		throw ::jxx::io::IOException("socket channel write failed");
	}

	void SocketChannel::implCloseSelectableChannel()
	{
		::jxx::net::internal::NativeSocket native =
			::jxx::net::internal::kInvalidSocket;

		{
			std::lock_guard<std::mutex> lock(mutex_);

			// Publish the channel-close state before waking a blocked native I/O
			// operation. This lets the blocked operation classify the wakeup as
			// an asynchronous close instead of exposing a platform socket error.
			open_ = false;
			connected_ = false;
			pending_ = false;

			if (state_ == nullptr) return;

			std::lock_guard<std::mutex> stateLock(state_->m);
			if (state_->closed) return;

			state_->closed = true;
			state_->inputShutdown = true;
			state_->outputShutdown = true;
			native = state_->socket;
			state_->socket = ::jxx::net::internal::kInvalidSocket;
		}

		// Never hold the channel mutex across native shutdown/close. A blocked
		// read or write must be able to reacquire it and observe open_ == false.
		if (native != ::jxx::net::internal::kInvalidSocket) {
#if defined(_WIN32)
			(void)::shutdown(native, SD_BOTH);
#else
			(void)::shutdown(native, SHUT_RDWR);
#endif
			::jxx::net::internal::closeNativeSocket(native);
		}
	}
	::jxx::Ptr<SocketChannel::NetworkChannel> SocketChannel::bind(const ::jxx::Ptr<::jxx::net::SocketAddress>& local)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (bound_) throw ::jxx::nio::channels::AlreadyBoundException();
		int family = AF_INET;
		::jxx::Ptr<::jxx::net::InetSocketAddress> value;
		if (local != nullptr) {
			value = ::jxx::CAST<::jxx::net::InetSocketAddress>(local);
			if (value == nullptr) throw ::jxx::nio::channels::UnsupportedAddressTypeException();
			if (value->isUnresolved()) throw ::jxx::nio::channels::UnresolvedAddressException();
			const auto inet = value->getAddress();
			if (inet != nullptr && inet->getAddress() != nullptr && inet->getAddress()->length == 16) family = AF_INET6;
		}
		if (state_->socket == ::jxx::net::internal::kInvalidSocket) {
			state_->socket = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
			if (state_->socket == ::jxx::net::internal::kInvalidSocket) throw ::jxx::io::IOException("could not create socket channel");
			setBlocking(state_->socket, blocking_);
			applyPendingOptions(state_->socket);
		}
		sockaddr_storage storage{};
		socklen_t length = 0;
		if (family == AF_INET6) {
			auto* address = reinterpret_cast<sockaddr_in6*>(&storage);
			address->sin6_family = AF_INET6; address->sin6_addr = in6addr_any;
			address->sin6_port = htons(static_cast<unsigned short>(value == nullptr ? 0 : value->getPort()));
			if (value != nullptr && value->getAddress() != nullptr) {
				const auto bytes = value->getAddress()->getAddress();
				std::memcpy(&address->sin6_addr, bytes->data(), 16U);
			}
			length = sizeof(sockaddr_in6);
		} else {
			auto* address = reinterpret_cast<sockaddr_in*>(&storage);
			address->sin_family = AF_INET; address->sin_addr.s_addr = htonl(INADDR_ANY);
			address->sin_port = htons(static_cast<unsigned short>(value == nullptr ? 0 : value->getPort()));
			if (value != nullptr && value->getAddress() != nullptr) {
				const auto bytes = value->getAddress()->getAddress();
				std::memcpy(&address->sin_addr, bytes->data(), 4U);
			}
			length = sizeof(sockaddr_in);
		}
		if (::bind(state_->socket, reinterpret_cast<sockaddr*>(&storage), length) != 0) throw ::jxx::io::IOException("socket channel bind failed");
		bound_ = true;
		return ::jxx::CAST<NetworkChannel>(thisPtr());
	}
	::jxx::Ptr<::jxx::net::SocketAddress> SocketChannel::getLocalAddress() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (state_->socket == ::jxx::net::internal::kInvalidSocket) return nullptr;
		sockaddr_storage storage{}; socklen_t length = sizeof(storage);
		if (::getsockname(state_->socket, reinterpret_cast<sockaddr*>(&storage), &length) != 0) return nullptr;
		return ::jxx::NEW<::jxx::net::InetSocketAddress>(addressFromNative(storage), portFromNative(storage));
	}
	::jxx::Ptr<::jxx::net::SocketAddress> SocketChannel::getRemoteAddress() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (!connected_) return nullptr;
		sockaddr_storage storage{}; socklen_t length = sizeof(storage);
		if (::getpeername(state_->socket, reinterpret_cast<sockaddr*>(&storage), &length) != 0) return nullptr;
		return ::jxx::NEW<::jxx::net::InetSocketAddress>(addressFromNative(storage), portFromNative(storage));
	}
	::jxx::Ptr<::jxx::net::Socket> SocketChannel::socket()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (socketView_ != nullptr) return socketView_;

		::jxx::Ptr<::jxx::net::InetAddress> remoteAddress;
		::jxx::Ptr<::jxx::net::InetAddress> localAddress;
		::jxx::lang::jint remotePort = 0;
		::jxx::lang::jint localPort = 0;

		if (connected_ && state_->socket !=
				::jxx::net::internal::kInvalidSocket) {
			sockaddr_storage nativeRemote{};
			socklen_t remoteLength = sizeof(nativeRemote);
			if (::getpeername(state_->socket,
				reinterpret_cast<sockaddr*>(&nativeRemote),
				&remoteLength) == 0) {
				remoteAddress = addressFromNative(nativeRemote);
				remotePort = portFromNative(nativeRemote);
			}
		}

		if (state_->socket != ::jxx::net::internal::kInvalidSocket) {
			sockaddr_storage nativeLocal{};
			socklen_t localLength = sizeof(nativeLocal);
			if (::getsockname(state_->socket,
				reinterpret_cast<sockaddr*>(&nativeLocal),
				&localLength) == 0) {
				localAddress = addressFromNative(nativeLocal);
				localPort = portFromNative(nativeLocal);
			}
		}

		socketView_ = ::jxx::NEW<::jxx::net::Socket>(
			state_, remoteAddress, remotePort, localAddress, localPort,
			::jxx::CAST<SocketChannel>(thisPtr()), connected_, bound_);
		return socketView_;
	}
	::jxx::Ptr<SocketChannel> SocketChannel::shutdownInput()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (!connected_) throw ::jxx::nio::channels::NotYetConnectedException();
		if (!inputShutdown_) {
#if defined(_WIN32)
			const int result = ::shutdown(state_->socket, SD_RECEIVE);
#else
			const int result = ::shutdown(state_->socket, SHUT_RD);
#endif
			if (result != 0) throw ::jxx::io::IOException("channel input shutdown failed");
			inputShutdown_ = true;
			state_->inputShutdown = true;
		}
		return ::jxx::CAST<SocketChannel>(thisPtr());
	}

	::jxx::Ptr<SocketChannel> SocketChannel::shutdownOutput()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (!connected_) throw ::jxx::nio::channels::NotYetConnectedException();
		if (!outputShutdown_) {
#if defined(_WIN32)
			const int result = ::shutdown(state_->socket, SD_SEND);
#else
			const int result = ::shutdown(state_->socket, SHUT_WR);
#endif
			if (result != 0) throw ::jxx::io::IOException("channel output shutdown failed");
			outputShutdown_ = true;
			state_->outputShutdown = true;
		}
		return ::jxx::CAST<SocketChannel>(thisPtr());
	}
	::jxx::lang::jint SocketChannel::validOps()const noexcept
	{
		return 1 | 4 | 8;
	}
	::jxx::lang::jlong SocketChannel::read(const ::jxx::Ptr<BufferArray>& buffers)
	{
		if (buffers == nullptr)throw ::jxx::lang::NullPointerException(); return read(buffers, 0, buffers->length);
	}
	::jxx::lang::jlong SocketChannel::read(const ::jxx::Ptr<BufferArray>& buffers, ::jxx::lang::jint offset, ::jxx::lang::jint length)
	{
		if (buffers == nullptr)throw ::jxx::lang::NullPointerException(); if (offset < 0 || length<0 || offset>buffers->length - length)throw ::jxx::lang::IndexOutOfBoundsException(); ::jxx::lang::jlong total = 0; for (::jxx::lang::jint i = offset; i < offset + length; ++i) {
			if ((*buffers)[i] == nullptr)throw ::jxx::lang::NullPointerException(); const auto count = read((*buffers)[i]); if (count < 0)return total == 0 ? -1 : total; total += count; if (count == 0 || (*buffers)[i]->hasRemaining())break;
		}return total;
	}
	::jxx::lang::jlong SocketChannel::write(const ::jxx::Ptr<BufferArray>& buffers)
	{
		if (buffers == nullptr)throw ::jxx::lang::NullPointerException(); return write(buffers, 0, buffers->length);
	}
	::jxx::lang::jlong SocketChannel::write(const ::jxx::Ptr<BufferArray>& buffers, ::jxx::lang::jint offset, ::jxx::lang::jint length)
	{
		if (buffers == nullptr)throw ::jxx::lang::NullPointerException(); if (offset < 0 || length<0 || offset>buffers->length - length)throw ::jxx::lang::IndexOutOfBoundsException(); ::jxx::lang::jlong total = 0; for (::jxx::lang::jint i = offset; i < offset + length; ++i) {
			if ((*buffers)[i] == nullptr)throw ::jxx::lang::NullPointerException(); const auto count = write((*buffers)[i]); total += count; if (count == 0 || (*buffers)[i]->hasRemaining())break;
		}return total;
	}
	::jxx::Ptr<SocketChannel::NetworkChannel> SocketChannel::setOption(
		const ::jxx::Ptr<Option>& name,
		const ::jxx::Ptr<::jxx::lang::Object>& value)
	{
		if (name == nullptr || value == nullptr)
			throw ::jxx::lang::NullPointerException();
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		const auto text = name->name()->utf8();
		if (text == "SO_KEEPALIVE" || text == "SO_REUSEADDR" ||
			text == "TCP_NODELAY") {
			const auto boolean = ::jxx::CAST<::jxx::lang::Boolean>(value);
			if (boolean == nullptr) throw ::jxx::lang::IllegalArgumentException();
			const auto flag = boolean->booleanValue();
			if (text == "SO_KEEPALIVE") { keepAlive_ = flag; keepAliveSet_ = true; }
			else if (text == "SO_REUSEADDR") { reuseAddress_ = flag; reuseAddressSet_ = true; }
			else { tcpNoDelay_ = flag; tcpNoDelaySet_ = true; }
		} else if (text == "SO_SNDBUF" || text == "SO_RCVBUF" ||
			text == "SO_LINGER" || text == "IP_TOS") {
			const auto number = ::jxx::CAST<::jxx::lang::Integer>(value);
			if (number == nullptr) throw ::jxx::lang::IllegalArgumentException();
			const auto integer = number->intValue();
			if ((text == "SO_SNDBUF" || text == "SO_RCVBUF") && integer <= 0)
				throw ::jxx::lang::IllegalArgumentException();
			if (text == "SO_LINGER" && integer < -1)
				throw ::jxx::lang::IllegalArgumentException();
			if (text == "IP_TOS" && (integer < 0 || integer > 255))
				throw ::jxx::lang::IllegalArgumentException();
			if (text == "SO_SNDBUF") { sendBufferSize_ = integer; sendBufferSizeSet_ = true; }
			else if (text == "SO_RCVBUF") { receiveBufferSize_ = integer; receiveBufferSizeSet_ = true; }
			else if (text == "SO_LINGER") { linger_ = integer > 65535 ? 65535 : integer; lingerSet_ = true; }
			else { trafficClass_ = integer; trafficClassSet_ = true; }
		} else {
			throw ::jxx::lang::UnsupportedOperationException();
		}
		if (state_->socket != ::jxx::net::internal::kInvalidSocket)
			applyPendingOptions(state_->socket);
		return ::jxx::CAST<NetworkChannel>(thisPtr());
	}

	::jxx::Ptr<::jxx::lang::Object> SocketChannel::getOption(
		const ::jxx::Ptr<Option>& name) const
	{
		if (name == nullptr) throw ::jxx::lang::NullPointerException();
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		const auto text = name->name()->utf8();
		if (text != "SO_KEEPALIVE" && text != "SO_REUSEADDR" &&
			text != "SO_SNDBUF" && text != "SO_RCVBUF" &&
			text != "SO_LINGER" && text != "IP_TOS" &&
			text != "TCP_NODELAY")
			throw ::jxx::lang::UnsupportedOperationException();
		if (state_->socket == ::jxx::net::internal::kInvalidSocket) {
			if (text == "SO_KEEPALIVE") return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(keepAlive_));
			if (text == "SO_REUSEADDR") return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(reuseAddress_));
			if (text == "TCP_NODELAY") return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(tcpNoDelay_));
			if (text == "SO_SNDBUF") return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(sendBufferSize_));
			if (text == "SO_RCVBUF") return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(receiveBufferSize_));
			if (text == "SO_LINGER") return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(linger_));
			return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(trafficClass_));
		}
		int level = SOL_SOCKET;
		int option = 0;
		bool boolean = false;
		if (text == "SO_KEEPALIVE") { option = SO_KEEPALIVE; boolean = true; }
		else if (text == "SO_REUSEADDR") { option = SO_REUSEADDR; boolean = true; }
		else if (text == "SO_SNDBUF") option = SO_SNDBUF;
		else if (text == "SO_RCVBUF") option = SO_RCVBUF;
		else if (text == "TCP_NODELAY") { level = IPPROTO_TCP; option = TCP_NODELAY; boolean = true; }
		else if (text == "IP_TOS") { level = IPPROTO_IP; option = IP_TOS; }
		if (text == "SO_LINGER") {
			linger value{}; socklen_t length = sizeof(value);
			if (::getsockopt(state_->socket, SOL_SOCKET, SO_LINGER,
				reinterpret_cast<char*>(&value), &length) != 0)
				throw ::jxx::io::IOException("getOption failed");
			return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(
				value.l_onoff == 0 ? -1 : value.l_linger));
		}
		int value = 0; socklen_t length = sizeof(value);
		if (::getsockopt(state_->socket, level, option,
			reinterpret_cast<char*>(&value), &length) != 0)
			throw ::jxx::io::IOException("getOption failed");
		return boolean
			? ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(value != 0))
			: ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(value));
	}

	::jxx::Ptr<::jxx::util::Set<SocketChannel::Option>>
	SocketChannel::supportedOptions() const
	{
		const auto result = ::jxx::NEW<::jxx::util::HashSet<Option>>();
		result->add(::jxx::net::StandardSocketOptions::SO_KEEPALIVE_);
		result->add(::jxx::net::StandardSocketOptions::SO_REUSEADDR_);
		result->add(::jxx::net::StandardSocketOptions::SO_SNDBUF_);
		result->add(::jxx::net::StandardSocketOptions::SO_RCVBUF_);
		result->add(::jxx::net::StandardSocketOptions::SO_LINGER_);
		result->add(::jxx::net::StandardSocketOptions::IP_TOS_);
		result->add(::jxx::net::StandardSocketOptions::TCP_NODELAY_);
		return ::jxx::CAST<::jxx::util::Set<Option>>(result);
	}








} // namespace jxx::nio::channels
