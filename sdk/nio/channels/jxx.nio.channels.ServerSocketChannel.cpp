#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <fcntl.h>
#endif
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "net/internal/jxx.net.internal.NativeSocketState.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.StandardSocketOptions.h"
#include "net/jxx.net.Socket.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "util/jxx.util.HashSet.h"
namespace jxx::nio::channels {
::jxx::Ptr<ServerSocketChannel> ServerSocketChannel::open(){auto c=::jxx::NEW<ServerSocketChannel>();c->state_->serverChannel=c;return c;}
ServerSocketChannel::ServerSocketChannel():state_(::jxx::NEW<::jxx::net::internal::NativeSocketState>()){socketView_=::jxx::NEW<::jxx::net::ServerSocket>(state_);}
ServerSocketChannel::~ServerSocketChannel(){try{close();}catch(...){}}
void ServerSocketChannel::setNativeBlocking_(::jxx::lang::jbool block){if(state_->socket==::jxx::net::internal::kInvalidSocket)return;
#if defined(_WIN32)
u_long mode=block?0UL:1UL;if(::ioctlsocket(state_->socket,FIONBIO,&mode)!=0)throw ::jxx::io::IOException("configure blocking failed");
#else
int f=::fcntl(state_->socket,F_GETFL,0);if(f<0||::fcntl(state_->socket,F_SETFL,block?(f&~O_NONBLOCK):(f|O_NONBLOCK))!=0)throw ::jxx::io::IOException("configure blocking failed");
#endif
}
::jxx::Ptr<ServerSocketChannel> ServerSocketChannel::configureBlocking(::jxx::lang::jbool b){std::lock_guard<std::mutex>l(mutex_);if(!isOpen())throw ClosedChannelException();setNativeBlocking_(b);blocking_=b;return ::jxx::CAST<ServerSocketChannel>(thisPtr());}
::jxx::lang::jbool ServerSocketChannel::isBlocking()const noexcept{return blocking_;}
::jxx::Ptr<NetworkChannel> ServerSocketChannel::bind(const ::jxx::Ptr<::jxx::net::SocketAddress>&a){return bind(a,0);}
::jxx::Ptr<NetworkChannel> ServerSocketChannel::bind(const ::jxx::Ptr<::jxx::net::SocketAddress>&a,::jxx::lang::jint b){std::lock_guard<std::mutex>l(mutex_);if(!isOpen())throw ClosedChannelException();socketView_->bind(a,b);setNativeBlocking_(blocking_);return ::jxx::CAST<NetworkChannel>(thisPtr());}
::jxx::Ptr<::jxx::net::ServerSocket> ServerSocketChannel::socket(){return socketView_;}
::jxx::Ptr<SocketChannel> ServerSocketChannel::accept(){std::lock_guard<std::mutex>l(mutex_);if(!isOpen())throw ClosedChannelException();auto s=socketView_->accept();if(!s)return nullptr;auto c=::jxx::NEW<SocketChannel>(s);s->sharedNativeSocketState()->channel=c;return c;}
::jxx::Ptr<::jxx::net::SocketAddress> ServerSocketChannel::getLocalAddress()const{if(!isOpen())throw ClosedChannelException();return socketView_->getLocalSocketAddress();}
::jxx::Ptr<NetworkChannel> ServerSocketChannel::setOption(const ::jxx::Ptr<Option>&n,const ::jxx::Ptr<::jxx::lang::Object>&v){if(!n||!v)throw ::jxx::lang::NullPointerException();auto t=n->name()->utf8();if(t=="SO_REUSEADDR"){auto x=::jxx::CAST<::jxx::lang::Boolean>(v);if(!x)throw ::jxx::lang::IllegalArgumentException();socketView_->setReuseAddress(x->booleanValue());}else if(t=="SO_RCVBUF"){auto x=::jxx::CAST<::jxx::lang::Integer>(v);if(!x)throw ::jxx::lang::IllegalArgumentException();socketView_->setReceiveBufferSize(x->intValue());}else throw ::jxx::lang::UnsupportedOperationException();return ::jxx::CAST<NetworkChannel>(thisPtr());}
::jxx::Ptr<::jxx::lang::Object> ServerSocketChannel::getOption(const ::jxx::Ptr<Option>&n)const{if(!n)throw ::jxx::lang::NullPointerException();auto t=n->name()->utf8();if(t=="SO_REUSEADDR")return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(socketView_->getReuseAddress()));if(t=="SO_RCVBUF")return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(socketView_->getReceiveBufferSize()));throw ::jxx::lang::UnsupportedOperationException();}
::jxx::Ptr<::jxx::util::Set<ServerSocketChannel::Option>> ServerSocketChannel::supportedOptions()const{auto r=::jxx::NEW<::jxx::util::HashSet<Option>>();r->add(::jxx::net::StandardSocketOptions::SO_RCVBUF_);r->add(::jxx::net::StandardSocketOptions::SO_REUSEADDR_);return ::jxx::CAST<::jxx::util::Set<Option>>(r);}
::jxx::lang::jbool ServerSocketChannel::isOpen()const{return state_&&!state_->closed;}
void ServerSocketChannel::close(){std::lock_guard<std::mutex>l(mutex_);if(socketView_)socketView_->close();}
} // namespace jxx::nio::channels
