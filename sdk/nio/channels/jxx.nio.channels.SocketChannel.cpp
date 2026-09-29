
#include <cerrno>
#include <cstring>
#include <string>
#include <vector>

#if !defined(_WIN32)
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#endif

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "util/jxx.util.HashSet.h"
#include "net/jxx.net.StandardSocketOptions.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Boolean.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/channels/jxx.nio.channels.AlreadyConnectedException.h"
#include "nio/channels/jxx.nio.channels.ConnectionPendingException.h"
#include "nio/channels/jxx.nio.channels.NoConnectionPendingException.h"
#include "nio/channels/jxx.nio.channels.NotYetConnectedException.h"
#include "nio/channels/jxx.nio.channels.AlreadyBoundException.h"
#include "nio/channels/jxx.nio.channels.UnsupportedAddressTypeException.h"
#include "nio/channels/jxx.nio.channels.UnresolvedAddressException.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"

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

	} // namespace

	SocketChannel::SocketChannel()
	{
		::jxx::net::internal::ensureNetworkInitialized();
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
		if (socket_ == ::jxx::net::internal::kInvalidSocket) {
			socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (socket_ == ::jxx::net::internal::kInvalidSocket)
				throw ::jxx::io::IOException("could not create socket channel");
			setBlocking(socket_, blocking_);
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
		if (socket_ != ::jxx::net::internal::kInvalidSocket)
			setBlocking(socket_, block);
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
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (connected_) throw ::jxx::nio::channels::AlreadyConnectedException();
		if (pending_) throw ::jxx::nio::channels::ConnectionPendingException();
		const auto remote = ::jxx::CAST<::jxx::net::InetSocketAddress>(remoteAddress);
		if (remote == nullptr)
			throw ::jxx::nio::channels::UnsupportedAddressTypeException();
		addrinfo* addresses = resolve(remote);
		const std::shared_ptr<void> addressGuard(addresses, [](void* value)
	 {
			 freeaddrinfo(static_cast<addrinfo*>(value));
		});

		for (addrinfo* current = addresses; current != nullptr; current = current->ai_next) {
			if (socket_ != ::jxx::net::internal::kInvalidSocket)
				::jxx::net::internal::closeNativeSocket(socket_);
			socket_ = ::socket(current->ai_family, current->ai_socktype, current->ai_protocol);
			if (socket_ == ::jxx::net::internal::kInvalidSocket) continue;
			setBlocking(socket_, blocking_);
			const int result = ::connect(socket_, current->ai_addr,
				static_cast<socklen_t>(current->ai_addrlen));
			if (result == 0) {
				connected_ = true; pending_ = false; return true;
			}
			const int error = lastSocketError();
			if (!blocking_ && wouldBlock(error)) {
				pending_ = true; return false;
			}
		}
		::jxx::net::internal::closeNativeSocket(socket_);
		socket_ = ::jxx::net::internal::kInvalidSocket;
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
		if (getsockopt(socket_, SOL_SOCKET, SO_ERROR,
			reinterpret_cast<char*>(&error), &length) != 0)
			throw ::jxx::io::IOException("finishConnect failed");
#else
		socklen_t length = sizeof(error);
		if (getsockopt(socket_, SOL_SOCKET, SO_ERROR, &error, &length) != 0)
			throw ::jxx::io::IOException("finishConnect failed");
#endif
		if (error == 0) {
			pending_ = false; connected_ = true; return true;
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
		return connected_;
	}
	::jxx::lang::jbool SocketChannel::isOpen() const
	{
		return open_;
	}

	::jxx::lang::jint SocketChannel::read(
		const ::jxx::Ptr<::jxx::nio::ByteBuffer> destination)
	{
		if (destination == nullptr) throw ::jxx::lang::NullPointerException();
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (!connected_) throw ::jxx::nio::channels::NotYetConnectedException();
		if (inputShutdown_) return -1;
		const auto remaining = destination->remaining();
		if (remaining == 0) return 0;
		std::vector<unsigned char> bytes(static_cast<std::size_t>(remaining));
#if defined(_WIN32)
		const int count = ::recv(socket_, reinterpret_cast<char*>(bytes.data()), remaining, 0);
#else
		const int count = static_cast<int>(::recv(socket_, bytes.data(), remaining, 0));
#endif
		if (count > 0) {
			for (int index = 0; index < count; ++index)
				destination->put(static_cast<::jxx::lang::jbyte>(bytes[index]));
			return count;
		}
		if (count == 0) {
			connected_ = false; return -1;
		}
		if (!blocking_ && wouldBlock(lastSocketError())) return 0;
		throw ::jxx::io::IOException("socket channel read failed");
	}

	::jxx::lang::jint SocketChannel::write(
		const ::jxx::Ptr<::jxx::nio::ByteBuffer> source)
	{
		if (source == nullptr) throw ::jxx::lang::NullPointerException();
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) throw ::jxx::nio::channels::ClosedChannelException();
		if (!connected_) throw ::jxx::nio::channels::NotYetConnectedException();
		if (outputShutdown_) throw ::jxx::nio::channels::ClosedChannelException();
		const auto remaining = source->remaining();
		if (remaining == 0) return 0;
		const auto position = source->position();
		std::vector<unsigned char> bytes(static_cast<std::size_t>(remaining));
		for (::jxx::lang::jint index = 0; index < remaining; ++index)
			bytes[static_cast<std::size_t>(index)] =
			static_cast<unsigned char>(source->get(position + index));
#if defined(_WIN32)
		const int count = ::send(socket_, reinterpret_cast<const char*>(bytes.data()), remaining, 0);
#else
		const int count = static_cast<int>(::send(socket_, bytes.data(), remaining, 0));
#endif
		if (count > 0) {
			source->position(position + count); return count;
		}
		if (!blocking_ && wouldBlock(lastSocketError())) return 0;
		throw ::jxx::io::IOException("socket channel write failed");
	}

	void SocketChannel::close()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (!open_) return;
		if (socket_ != ::jxx::net::internal::kInvalidSocket)
			::jxx::net::internal::closeNativeSocket(socket_);
		socket_ = ::jxx::net::internal::kInvalidSocket;
		open_ = false;
		connected_ = false;
		pending_ = false;
	}


	::jxx::Ptr<SocketChannel::NetworkChannel> SocketChannel::bind(const ::jxx::Ptr<::jxx::net::SocketAddress>& local)
	{
		std::lock_guard<std::mutex>lock(mutex_); if (bound_)throw ::jxx::lang::IllegalStateException("channel is already bound"); ensureSocket(); sockaddr_in address{}; address.sin_family = AF_INET; address.sin_addr.s_addr = htonl(INADDR_ANY); address.sin_port = 0; if (local != nullptr) {
			const auto value = ::jxx::CAST<::jxx::net::InetSocketAddress>(local); if (value == nullptr)throw ::jxx::lang::IllegalArgumentException(); address.sin_port = htons(static_cast<unsigned short>(value->getPort()));
		}if (::bind(socket_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0)throw ::jxx::io::IOException("socket channel bind failed"); bound_ = true; return ::jxx::CAST<NetworkChannel>(thisPtr());
	}
	::jxx::Ptr<::jxx::net::SocketAddress> SocketChannel::getLocalAddress()const
	{
		if (!open_)throw ::jxx::nio::channels::ClosedChannelException(); sockaddr_in address{}; socklen_t length = sizeof(address); if (socket_ == ::jxx::net::internal::kInvalidSocket || getsockname(socket_, reinterpret_cast<sockaddr*>(&address), &length) != 0)return nullptr; return ::jxx::NEW<::jxx::net::InetSocketAddress>(ntohs(address.sin_port));
	}
	::jxx::Ptr<::jxx::net::SocketAddress> SocketChannel::getRemoteAddress()const
	{
		if (!open_)throw ::jxx::nio::channels::ClosedChannelException(); if (!connected_)return nullptr; sockaddr_in address{}; socklen_t length = sizeof(address); if (getpeername(socket_, reinterpret_cast<sockaddr*>(&address), &length) != 0)return nullptr; return ::jxx::NEW<::jxx::net::InetSocketAddress>(ntohs(address.sin_port));
	}
	::jxx::Ptr<::jxx::net::Socket> SocketChannel::socket()
	{
		throw ::jxx::lang::UnsupportedOperationException("Socket facade is not available for native channel ownership");
	}
	::jxx::Ptr<SocketChannel> SocketChannel::shutdownInput()
	{
		std::lock_guard<std::mutex>lock(mutex_); if (!connected_)throw ::jxx::io::IOException("channel is not connected"); if (!inputShutdown_) {
			::shutdown(socket_,
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
			::shutdown(socket_,
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
		else throw ::jxx::lang::UnsupportedOperationException(); const auto boolean = ::jxx::CAST<::jxx::lang::Boolean>(value); const auto number = ::jxx::CAST<::jxx::lang::Integer>(value); integer = boolean != nullptr ? (boolean->booleanValue() ? 1 : 0) : (number != nullptr ? number->intValue() : 0); if (setsockopt(socket_, level, option, reinterpret_cast<const char*>(&integer), sizeof(integer)) != 0)throw ::jxx::io::IOException("setOption failed"); return ::jxx::CAST<NetworkChannel>(thisPtr());
	}
	::jxx::Ptr<::jxx::lang::Object> SocketChannel::getOption(const ::jxx::Ptr<Option>& name)const
	{
		if (name == nullptr)throw ::jxx::lang::NullPointerException(); if (socket_ == ::jxx::net::internal::kInvalidSocket)throw ::jxx::io::IOException("channel socket is not created"); const auto text = name->name()->utf8(); int level = SOL_SOCKET, option = 0, value = 0; socklen_t length = sizeof(value); bool flag = true; if (text == "SO_KEEPALIVE")option = SO_KEEPALIVE; else if (text == "SO_REUSEADDR")option = SO_REUSEADDR; else if (text == "SO_SNDBUF") {
			option = SO_SNDBUF; flag = false;
		}
		else if (text == "SO_RCVBUF") {
			option = SO_RCVBUF; flag = false;
		}
		else if (text == "TCP_NODELAY") {
			level = IPPROTO_TCP; option = TCP_NODELAY;
		}
		else throw ::jxx::lang::UnsupportedOperationException(); if (getsockopt(socket_, level, option, reinterpret_cast<char*>(&value), &length) != 0)throw ::jxx::io::IOException("getOption failed"); return flag ? ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(value != 0)): ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(value));
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

} // namespace jxx::nio::channels
