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
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "net/internal/jxx.net.internal.NativeSocketState.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.StandardSocketOptions.h"
#include "net/jxx.net.Socket.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "net/jxx.net.SocketException.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "util/jxx.util.HashSet.h"
namespace jxx::nio::channels {
::jxx::Ptr<ServerSocketChannel> ServerSocketChannel::open(){auto c=::jxx::NEW<ServerSocketChannel>();c->state_->serverChannel=c;return c;}
ServerSocketChannel::ServerSocketChannel():state_(::jxx::NEW<::jxx::net::internal::NativeSocketState>()),socket_(::jxx::NEW<::jxx::net::ServerSocket>(state_)){}
ServerSocketChannel::~ServerSocketChannel(){try{close();}catch(...){}}
void ServerSocketChannel::setBlocking_(::jxx::lang::jbool b){if(state_->socket==::jxx::net::internal::kInvalidSocket)return;
#if defined(_WIN32)
u_long m=b?0UL:1UL;if(::ioctlsocket(state_->socket,FIONBIO,&m)!=0)throw ::jxx::io::IOException();
#else
int f=::fcntl(state_->socket,F_GETFL,0);if(f<0||::fcntl(state_->socket,F_SETFL,b?(f&~O_NONBLOCK):(f|O_NONBLOCK))!=0)throw ::jxx::io::IOException();
#endif
}
::jxx::Ptr<ServerSocketChannel> ServerSocketChannel::configureBlocking(::jxx::lang::jbool b){if(!isOpen())throw ClosedChannelException();setBlocking_(b);blocking_=b;return ::jxx::CAST<ServerSocketChannel>(thisPtr());}
::jxx::lang::jbool ServerSocketChannel::isBlocking()const noexcept{return blocking_;}
::jxx::Ptr<NetworkChannel> ServerSocketChannel::bind(const ::jxx::Ptr<::jxx::net::SocketAddress>&a){return bind(a,0);}
::jxx::Ptr<NetworkChannel> ServerSocketChannel::bind(const ::jxx::Ptr<::jxx::net::SocketAddress>&a,::jxx::lang::jint b){socket_->bind(a,b);setBlocking_(blocking_);return ::jxx::CAST<NetworkChannel>(thisPtr());}
::jxx::Ptr<::jxx::net::ServerSocket> ServerSocketChannel::socket(){return socket_;}
::jxx::Ptr<SocketChannel> ServerSocketChannel::accept(){if(!isOpen())throw ClosedChannelException();try{auto s=socket_->accept();if(!s)return nullptr;auto c=::jxx::NEW<SocketChannel>(s);s->sharedNativeSocketState()->channel=c;return c;}catch(const ::jxx::net::SocketException&){if(!isOpen())throw AsynchronousCloseException();throw;}}
::jxx::lang::jint ServerSocketChannel::validOps()const noexcept{return SelectionKey::OP_ACCEPT_;}
::jxx::Ptr<SelectionKey> ServerSocketChannel::registerChannel(const ::jxx::Ptr<Selector>&s,::jxx::lang::jint o,const ::jxx::Ptr<::jxx::lang::Object>&a){if(!s)throw ::jxx::lang::NullPointerException();if(blocking_)throw ::jxx::lang::IllegalStateException();return s->registerChannel(::jxx::CAST<::jxx::lang::Object>(thisPtr()),o,a);}
::jxx::Ptr<SelectionKey> ServerSocketChannel::keyFor(const ::jxx::Ptr<Selector>&s)const{return s?s->keyFor(::jxx::CAST<::jxx::lang::Object>(const_cast<ServerSocketChannel*>(this)->thisPtr())):nullptr;}
::jxx::Ptr<::jxx::net::SocketAddress> ServerSocketChannel::getLocalAddress()const{return socket_->getLocalSocketAddress();}
::jxx::Ptr<NetworkChannel> ServerSocketChannel::setOption(const ::jxx::Ptr<Option>&n,const ::jxx::Ptr<::jxx::lang::Object>&v){if(!n||!v)throw ::jxx::lang::NullPointerException();auto t=n->name()->utf8();if(t=="SO_REUSEADDR"){auto x=::jxx::CAST<::jxx::lang::Boolean>(v);if(!x)throw ::jxx::lang::IllegalArgumentException();socket_->setReuseAddress(x->booleanValue());}else if(t=="SO_RCVBUF"){auto x=::jxx::CAST<::jxx::lang::Integer>(v);if(!x)throw ::jxx::lang::IllegalArgumentException();socket_->setReceiveBufferSize(x->intValue());}else throw ::jxx::lang::UnsupportedOperationException();return ::jxx::CAST<NetworkChannel>(thisPtr());}
::jxx::Ptr<::jxx::lang::Object> ServerSocketChannel::getOption(const ::jxx::Ptr<Option>&n)const{if(!n)throw ::jxx::lang::NullPointerException();auto t=n->name()->utf8();if(t=="SO_REUSEADDR")return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(socket_->getReuseAddress()));if(t=="SO_RCVBUF")return ::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Integer::valueOf(socket_->getReceiveBufferSize()));throw ::jxx::lang::UnsupportedOperationException();}
::jxx::Ptr<::jxx::util::Set<ServerSocketChannel::Option>> ServerSocketChannel::supportedOptions()const{auto r=::jxx::NEW<::jxx::util::HashSet<Option>>();r->add(::jxx::net::StandardSocketOptions::SO_RCVBUF_);r->add(::jxx::net::StandardSocketOptions::SO_REUSEADDR_);return ::jxx::CAST<::jxx::util::Set<Option>>(r);}
::jxx::lang::jbool ServerSocketChannel::isOpen()const{return state_&&!state_->closed;}
void ServerSocketChannel::close(){if(socket_)socket_->close();}
}
