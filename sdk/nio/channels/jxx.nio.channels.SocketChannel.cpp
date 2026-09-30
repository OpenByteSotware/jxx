
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
	{
		::jxx::net::internal::ensureNetworkInitialized();
	}

	SocketChannel::SocketChannel(const ::jxx::Ptr<::jxx::net::Socket>& socket)
		: state_(socket == nullptr ? std::make_shared<::jxx::net::internal::NativeSocketState>() : socket->sharedNativeSocketState()), socketView_(socket), connected_(socket != nullptr && socket->isConnected()), bound_(socket != nullptr && socket->isBound()) {
		if (socket == nullptr) throw ::jxx::lang::NullPointerException();
	}


	::jxx::Ptr<SocketChannel> SocketChannel::open()
	{
		return ::jxx::NEW<SocketChannel>();
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
		}
	}

	::jxx::Ptr<SocketChannel> SocketChannel::configureBlocking(
		::jxx::lang::jbool block)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (pending_)
			throw ::jxx::lang::IllegalStateException(
				"cannot change blocking mode while connection is pending");
		if (state_->socket != ::jxx::net::internal::kInvalidSocket)
			setBlocking(state_->socket, block);
		blocking_ = block;
		return ::jxx::CAST<SocketChannel>(thisPtr());
	}

	::jxx::lang::jbool SocketChannel::isBlocking() const noexcept
	{
		return blocking_;
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
			InterruptRegistration interruptRegistration(state_);
			const int result = ::connect(native, current->ai_addr,
				static_cast<socklen_t>(current->ai_addrlen));
			if (interruptRegistration.interrupted()) {
				std::lock_guard<std::mutex> lock(mutex_); open_ = false; pending_ = false;
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
	::jxx::lang::jbool SocketChannel::isOpen() const
	{
		return open_ && state_ != nullptr && !state_->closed;
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
		InterruptRegistration interruptRegistration(state_);
#if defined(_WIN32)
		const int count = ::recv(native, reinterpret_cast<char*>(bytes.data()), remaining, 0);
#else
		const int count = static_cast<int>(::recv(native, bytes.data(), remaining, 0));
#endif
		if (interruptRegistration.interrupted()) {
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
		if (count == 0) {
			std::lock_guard<std::mutex> lock(mutex_);
			connected_ = false;
			return -1;
		}
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
		InterruptRegistration interruptRegistration(state_);
#if defined(_WIN32)
		const int count = ::send(native, reinterpret_cast<const char*>(bytes.data()), remaining, 0);
#else
		const int count = static_cast<int>(::send(native, bytes.data(), remaining, 0));
#endif
		if (interruptRegistration.interrupted()) {
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
			if (!open_) throw ::jxx::nio::channels::AsynchronousCloseException();
		}
		throw ::jxx::io::IOException("socket channel write failed");
	}

	void SocketChannel::close()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (state_ == nullptr || state_->closed) {
			open_ = false;
			connected_ = false;
			pending_ = false;
			return;
		}
		::jxx::net::internal::NativeSocket native =
			::jxx::net::internal::kInvalidSocket;
		{
			std::lock_guard<std::mutex> stateLock(state_->m);
			state_->closed = true;
			state_->inputShutdown = true;
			state_->outputShutdown = true;
			native = state_->socket;
			state_->socket = ::jxx::net::internal::kInvalidSocket;
		}
		if (native != ::jxx::net::internal::kInvalidSocket) {
#if defined(_WIN32)
			::shutdown(native, SD_BOTH);
#else
			::shutdown(native, SHUT_RDWR);
#endif
			::jxx::net::internal::closeNativeSocket(native);
		}
		open_ = false;
		connected_ = false;
		pending_ = false;
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
			::jxx::CAST<SocketChannel>(thisPtr()));
		return socketView_;
	}
	::jxx::Ptr<SocketChannel> SocketChannel::shutdownInput()
	{
		std::lock_guard<std::mutex>lock(mutex_); if (!connected_)throw ::jxx::io::IOException("channel is not connected"); if (!inputShutdown_) {
			::shutdown(state_->socket,
#if defined(_WIN32)
SD_RECEIVE
#else
SHUT_RD
#endif
); inputShutdown_ = true;
		}return ::jxx::CAST<SocketChannel>(thisPtr());
	}
	::jxx::Ptr<SocketChannel> SocketChannel::shutdownOutput()
	{
		std::lock_guard<std::mutex>lock(mutex_);
		if (!connected_)throw ::jxx::io::IOException("channel is not connected"); 
		if (!outputShutdown_) {
			::shutdown(state_->socket,
#if defined(_WIN32)
SD_SEND
#else
SHUT_WR
#endif
); outputShutdown_ = true;
		}return ::jxx::CAST<SocketChannel>(thisPtr());
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
	::jxx::Ptr<SocketChannel::NetworkChannel> SocketChannel::setOption(const ::jxx::Ptr<Option>& name, const ::jxx::Ptr<::jxx::lang::Object>& value)
	{
		if (name == nullptr || value == nullptr)throw ::jxx::lang::NullPointerException(); ensureSocket(); const auto text = name->name()->utf8(); int level = SOL_SOCKET, option = 0, integer = 0; if (text == "SO_KEEPALIVE")option = SO_KEEPALIVE; else if (text == "SO_REUSEADDR")option = SO_REUSEADDR; else if (text == "SO_SNDBUF")option = SO_SNDBUF; else if (text == "SO_RCVBUF")option = SO_RCVBUF; else if (text == "TCP_NODELAY") {
			level = IPPROTO_TCP; option = TCP_NODELAY;
		}
		else throw ::jxx::lang::UnsupportedOperationException(); const auto boolean = ::jxx::CAST<::jxx::lang::Boolean>(value); const auto number = ::jxx::CAST<::jxx::lang::Integer>(value); integer = boolean != nullptr ? (boolean->booleanValue() ? 1 : 0) : (number != nullptr ? number->intValue() : 0); if (setsockopt(state_->socket, level, option, reinterpret_cast<const char*>(&integer), sizeof(integer)) != 0)throw ::jxx::io::IOException("setOption failed"); return ::jxx::CAST<NetworkChannel>(thisPtr());
	}
	::jxx::Ptr<::jxx::lang::Object> SocketChannel::getOption(const ::jxx::Ptr<Option>& name)const
	{
		if (name == nullptr)throw ::jxx::lang::NullPointerException(); if (state_->socket == ::jxx::net::internal::kInvalidSocket)throw ::jxx::io::IOException("channel socket is not created"); const auto text = name->name()->utf8(); int level = SOL_SOCKET, option = 0, value = 0; socklen_t length = sizeof(value); bool flag = true; if (text == "SO_KEEPALIVE")option = SO_KEEPALIVE; else if (text == "SO_REUSEADDR")option = SO_REUSEADDR; else if (text == "SO_SNDBUF") {
			option = SO_SNDBUF; flag = false;
		}
		else if (text == "SO_RCVBUF") {
			option = SO_RCVBUF; flag = false;
		}
		else if (text == "TCP_NODELAY") {
			level = IPPROTO_TCP; option = TCP_NODELAY;
		}
		else throw ::jxx::lang::UnsupportedOperationException(); if (getsockopt(state_->socket, level, option, reinterpret_cast<char*>(&value), &length) != 0)throw ::jxx::io::IOException("getOption failed"); return flag ? ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(value != 0)): ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(value));
	}
	::jxx::Ptr<::jxx::util::Set<SocketChannel::Option>> SocketChannel::supportedOptions()const
	{
		const auto result = ::jxx::NEW<::jxx::util::HashSet<Option>>();
		result->add(::jxx::net::StandardSocketOptions::SO_KEEPALIVE_); 
		result->add(::jxx::net::StandardSocketOptions::SO_REUSEADDR_);
		result->add(::jxx::net::StandardSocketOptions::SO_SNDBUF_);
		result->add(::jxx::net::StandardSocketOptions::SO_RCVBUF_);
		result->add(::jxx::net::StandardSocketOptions::TCP_NODELAY_); 
		return ::jxx::CAST<::jxx::util::Set<Option>>(result);
	}

::jxx::Ptr<SelectionKey> SocketChannel::register_(
    const ::jxx::Ptr<Selector>& selector,
    ::jxx::lang::jint operations) {
    return register_(selector, operations, nullptr);
}
::jxx::Ptr<SelectionKey> SocketChannel::register_(
    const ::jxx::Ptr<Selector>& selector,
    ::jxx::lang::jint ops,
    const ::jxx::Ptr<::jxx::lang::Object>& attachment) {
    if (selector == nullptr) throw ::jxx::lang::NullPointerException();
    if (blocking_) throw ::jxx::nio::channels::IllegalBlockingModeException();
    const auto key = selector->registerChannel(
        ::jxx::CAST<::jxx::lang::Object>(thisPtr()), ops, attachment);
    setRegistered_(key != nullptr);
    return key;
}
::jxx::lang::jbool SocketChannel::isRegistered() const {
    return AbstractSelectableChannel::isRegistered();
}
::jxx::Ptr<SelectionKey> SocketChannel::keyFor(
    const ::jxx::Ptr<Selector>& selector) const {
    return selector == nullptr ? nullptr : selector->keyFor(
        ::jxx::CAST<::jxx::lang::Object>(
            const_cast<SocketChannel*>(this)->thisPtr()));
}

void SocketChannel::implCloseChannel() {
    close();
}

} // namespace jxx::nio::channels
