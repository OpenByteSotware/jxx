#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <fcntl.h>
#include <sys/socket.h>
#endif
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "net/internal/jxx.net.internal.NativeSocketState.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.StandardSocketOptions.h"
#include "net/jxx.net.Socket.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.IllegalBlockingModeException.h"
#include "nio/channels/jxx.nio.channels.NotYetBoundException.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "nio/channels/jxx.nio.channels.ClosedByInterruptException.h"
#include "net/jxx.net.SocketException.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "util/jxx.util.HashSet.h"
namespace jxx::nio::channels {
namespace {
void interruptAccept(
    const std::weak_ptr<::jxx::net::internal::NativeSocketState>& weakState) noexcept {
    const auto state = weakState.lock();
    if (state == nullptr) return;
    ::jxx::net::internal::NativeSocket native =
        ::jxx::net::internal::kInvalidSocket;
    {
        std::lock_guard<std::mutex> lock(state->m);
        if (state->socket == ::jxx::net::internal::kInvalidSocket) return;
        native = state->socket;
        state->socket = ::jxx::net::internal::kInvalidSocket;
        state->closed = true;
    }
#if defined(_WIN32)
    ::shutdown(native, SD_BOTH);
#else
    ::shutdown(native, SHUT_RDWR);
#endif
    ::jxx::net::internal::closeNativeSocket(native);
}
class AcceptInterruptRegistration final {
public:
    explicit AcceptInterruptRegistration(
        const std::shared_ptr<::jxx::net::internal::NativeSocketState>& state)
        : thread_(::jxx::lang::Thread::currentThread()) {
        if (thread_ == nullptr) return;
        const std::weak_ptr<::jxx::net::internal::NativeSocketState> weakState(state);
        if (thread_->isInterrupted()) {
            interruptAccept(weakState);
        }
        thread_->setParkWakeup_([weakState] { interruptAccept(weakState); });
    }
    ~AcceptInterruptRegistration() {
        if (thread_ != nullptr) thread_->clearParkWakeup_();
    }
    ::jxx::lang::jbool interrupted() const {
        return thread_ != nullptr && thread_->isInterrupted();
    }
private:
    ::jxx::Ptr<::jxx::lang::Thread> thread_;
};
} // namespace

::jxx::Ptr<ServerSocketChannel> ServerSocketChannel::open(){return ::jxx::nio::channels::spi::SelectorProvider::provider()->openServerSocketChannel();}
ServerSocketChannel::ServerSocketChannel():state_(::jxx::NEW<::jxx::net::internal::NativeSocketState>()),socket_(::jxx::NEW<::jxx::net::ServerSocket>(state_)){}
ServerSocketChannel::~ServerSocketChannel(){try{close();}catch(...){}}
void ServerSocketChannel::setBlocking_(::jxx::lang::jbool b){if(state_->socket==::jxx::net::internal::kInvalidSocket)return;
#if defined(_WIN32)
u_long m=b?0UL:1UL;if(::ioctlsocket(state_->socket,FIONBIO,&m)!=0)throw ::jxx::io::IOException();
#else
int f=::fcntl(state_->socket,F_GETFL,0);if(f<0||::fcntl(state_->socket,F_SETFL,b?(f&~O_NONBLOCK):(f|O_NONBLOCK))!=0)throw ::jxx::io::IOException();
#endif
}


::jxx::Ptr<NetworkChannel> ServerSocketChannel::bind(
    const ::jxx::Ptr<::jxx::net::SocketAddress>& address) {
    return bind(address, 0);
}

::jxx::Ptr<NetworkChannel> ServerSocketChannel::bind(
    const ::jxx::Ptr<::jxx::net::SocketAddress>& address,
    ::jxx::lang::jint backlog) {
    if (!isOpen()) throw ::jxx::nio::channels::ClosedChannelException();
    socket_->bind(address, backlog);
    applyPendingOptions_();
    setBlocking_(blocking_);
    return ::jxx::CAST<NetworkChannel>(thisPtr());
}
::jxx::Ptr<::jxx::net::ServerSocket> ServerSocketChannel::socket(){return socket_;}
::jxx::Ptr<SocketChannel> ServerSocketChannel::accept(){
    if(!isOpen())throw ClosedChannelException();
    if(socket_ == nullptr || !socket_->isBound())throw NotYetBoundException();
    AcceptInterruptRegistration interruptRegistration(state_);
    try {
        auto s=socket_->accept();
        if(interruptRegistration.interrupted()) {
            close();
            throw ClosedByInterruptException();
        }
        if(!isOpen())throw AsynchronousCloseException();
        if(!s)return nullptr;
        auto c=::jxx::NEW<SocketChannel>(s);
        s->sharedNativeSocketState()->channel=c;
        return c;
    } catch(const ClosedByInterruptException&) {
        throw;
    } catch(const ::jxx::net::SocketException&) {
        if(interruptRegistration.interrupted()) {
            close();
            throw ClosedByInterruptException();
        }
        if(!isOpen())throw AsynchronousCloseException();
        throw;
    }
}
::jxx::lang::jint ServerSocketChannel::validOps()const noexcept{return SelectionKey::OP_ACCEPT_;}


::jxx::Ptr<::jxx::net::SocketAddress> ServerSocketChannel::getLocalAddress()const{return socket_->getLocalSocketAddress();}
void ServerSocketChannel::applyPendingOptions_() {
    if (reuseAddressSet_) socket_->setReuseAddress(reuseAddress_);
    if (receiveBufferSizeSet_)
        socket_->setReceiveBufferSize(receiveBufferSize_);
}

::jxx::Ptr<NetworkChannel> ServerSocketChannel::setOption(
    const ::jxx::Ptr<Option>& name,
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    if (name == nullptr || value == nullptr)
        throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    if (!isOpen()) throw ::jxx::nio::channels::ClosedChannelException();
    const auto text = name->name()->utf8();
    if (text == "SO_REUSEADDR") {
        const auto boolean = ::jxx::CAST<::jxx::lang::Boolean>(value);
        if (boolean == nullptr) throw ::jxx::lang::IllegalArgumentException();
        reuseAddress_ = boolean->booleanValue();
        reuseAddressSet_ = true;
        if (state_->socket != ::jxx::net::internal::kInvalidSocket)
            socket_->setReuseAddress(reuseAddress_);
    } else if (text == "SO_RCVBUF") {
        const auto integer = ::jxx::CAST<::jxx::lang::Integer>(value);
        if (integer == nullptr || integer->intValue() <= 0)
            throw ::jxx::lang::IllegalArgumentException();
        receiveBufferSize_ = integer->intValue();
        receiveBufferSizeSet_ = true;
        if (state_->socket != ::jxx::net::internal::kInvalidSocket)
            socket_->setReceiveBufferSize(receiveBufferSize_);
    } else {
        throw ::jxx::lang::UnsupportedOperationException();
    }
    return ::jxx::CAST<NetworkChannel>(thisPtr());
}

::jxx::Ptr<::jxx::lang::Object> ServerSocketChannel::getOption(
    const ::jxx::Ptr<Option>& name) const {
    if (name == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    if (!isOpen()) throw ::jxx::nio::channels::ClosedChannelException();
    const auto text = name->name()->utf8();
    if (text == "SO_REUSEADDR") {
        const auto value = state_->socket == ::jxx::net::internal::kInvalidSocket
            ? reuseAddress_
            : socket_->getReuseAddress();
        return ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::lang::Boolean::valueOf(value));
    }
    if (text == "SO_RCVBUF") {
        const auto value = state_->socket == ::jxx::net::internal::kInvalidSocket
            ? receiveBufferSize_
            : socket_->getReceiveBufferSize();
        return ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::lang::Integer::valueOf(value));
    }
    throw ::jxx::lang::UnsupportedOperationException();
}

::jxx::Ptr<::jxx::util::Set<ServerSocketChannel::Option>>
ServerSocketChannel::supportedOptions() const {
    const auto result = ::jxx::NEW<::jxx::util::HashSet<Option>>();
    result->add(::jxx::net::StandardSocketOptions::SO_RCVBUF_);
    result->add(::jxx::net::StandardSocketOptions::SO_REUSEADDR_);
    return ::jxx::CAST<::jxx::util::Set<Option>>(result);
}



void ServerSocketChannel::implConfigureBlocking(::jxx::lang::jbool block){setBlocking_(block);blocking_=block;}
void ServerSocketChannel::implCloseSelectableChannel(){if(socket_)socket_->close();}
}
