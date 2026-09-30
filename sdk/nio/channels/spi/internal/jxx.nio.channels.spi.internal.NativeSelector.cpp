#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelector.h"
#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <cerrno>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include <algorithm>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "nio/channels/jxx.nio.channels.ClosedSelectorException.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectionKey.h"
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
#include "util/jxx.util.HashSet.h"
namespace jxx::nio::channels::spi::internal {
namespace {
void nonblocking(::jxx::net::internal::NativeSocket s){
#if defined(_WIN32)
 u_long m=1;if(::ioctlsocket(s,FIONBIO,&m)!=0)throw ::jxx::io::IOException("selector wakeup socket configuration failed");
#else
 int f=::fcntl(s,F_GETFL,0);if(f<0||::fcntl(s,F_SETFL,f|O_NONBLOCK)!=0)throw ::jxx::io::IOException("selector wakeup socket configuration failed");
#endif
}
}
NativeSelector::NativeSelector(const ::jxx::Ptr<::jxx::nio::channels::spi::SelectorProvider>& provider)
    : ::jxx::nio::channels::spi::AbstractSelector(provider){selected_=::jxx::NEW<::jxx::util::HashSet<::jxx::nio::channels::SelectionKey>>();initializeWakeup_();}
NativeSelector::~NativeSelector(){try{close();}catch(...){}}
void NativeSelector::initializeWakeup_(){::jxx::net::internal::ensureNetworkInitialized();
#if defined(_WIN32)
 auto listener=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(listener==INVALID_SOCKET)throw ::jxx::io::IOException("selector wakeup listener failed");sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;if(::bind(listener,reinterpret_cast<sockaddr*>(&a),sizeof(a))!=0||::listen(listener,1)!=0){::closesocket(listener);throw ::jxx::io::IOException("selector wakeup bind failed");}int n=sizeof(a);::getsockname(listener,reinterpret_cast<sockaddr*>(&a),&n);wakeWrite_=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(wakeWrite_==INVALID_SOCKET||::connect(wakeWrite_,reinterpret_cast<sockaddr*>(&a),sizeof(a))!=0){::closesocket(listener);if(wakeWrite_!=INVALID_SOCKET)::closesocket(wakeWrite_);throw ::jxx::io::IOException("selector wakeup connect failed");}wakeRead_=::accept(listener,nullptr,nullptr);::closesocket(listener);if(wakeRead_==INVALID_SOCKET)throw ::jxx::io::IOException("selector wakeup accept failed");
#else
 int pair[2];if(::socketpair(AF_UNIX,SOCK_STREAM,0,pair)!=0)throw ::jxx::io::IOException("selector wakeup socketpair failed");wakeRead_=pair[0];wakeWrite_=pair[1];
#endif
 nonblocking(wakeRead_);nonblocking(wakeWrite_);
}
::jxx::lang::jbool NativeSelector::isOpen()const noexcept{return open_.load();}
void NativeSelector::signalWakeup_()noexcept{if(wakeWrite_==::jxx::net::internal::kInvalidSocket)return;const char b=1;
#if defined(_WIN32)
 ::send(wakeWrite_,&b,1,0);
#else
 ::send(wakeWrite_,&b,1,MSG_NOSIGNAL);
#endif
}
void NativeSelector::drainWakeup_()noexcept{char b[64];while(wakeRead_!=::jxx::net::internal::kInvalidSocket){int n=::recv(wakeRead_,b,sizeof(b),0);if(n<=0)break;}wakeupPending_=false;}
void NativeSelector::close(){bool expected=true;if(!open_.compare_exchange_strong(expected,false))return;signalWakeup_();std::vector<::jxx::Ptr<::jxx::nio::channels::SelectionKey>> keys;{std::unique_lock<std::mutex>l(mutex_);selectionCondition_.wait(l,[this]{return activeSelections_==0;});keys=keys_;keys_.clear();cancelled_.clear();if(selected_)selected_->clear();}for(auto&k:keys)if(k)k->cancel();::jxx::net::internal::closeNativeSocket(wakeRead_);::jxx::net::internal::closeNativeSocket(wakeWrite_);wakeRead_=wakeWrite_=::jxx::net::internal::kInvalidSocket;}
void NativeSelector::processCancelled_(){std::lock_guard<std::mutex>l(mutex_);for(auto&k:cancelled_){selected_->remove(::jxx::CAST<::jxx::lang::Object>(k));keys_.erase(std::remove(keys_.begin(),keys_.end(),k),keys_.end());}cancelled_.clear();}
void NativeSelector::cancelKey(const ::jxx::Ptr<::jxx::nio::channels::SelectionKey>&k){{std::lock_guard<std::mutex>l(mutex_);if(std::find(cancelled_.begin(),cancelled_.end(),k)==cancelled_.end())cancelled_.push_back(k);}wakeup();}
::jxx::Ptr<::jxx::util::Set<::jxx::nio::channels::SelectionKey>> NativeSelector::keys(){if(!isOpen())throw ::jxx::nio::channels::ClosedSelectorException();processCancelled_();auto r=::jxx::NEW<::jxx::util::HashSet<::jxx::nio::channels::SelectionKey>>();std::lock_guard<std::mutex>l(mutex_);for(auto&k:keys_)if(k&&k->isValid())r->add(k);return ::jxx::CAST<::jxx::util::Set<::jxx::nio::channels::SelectionKey>>(r);}
::jxx::Ptr<::jxx::util::Set<::jxx::nio::channels::SelectionKey>> NativeSelector::selectedKeys(){if(!isOpen())throw ::jxx::nio::channels::ClosedSelectorException();return selected_;}
::jxx::lang::jbool NativeSelector::validOps_(const ::jxx::Ptr<::jxx::lang::Object>&c,::jxx::lang::jint o)const{auto sc=::jxx::CAST<::jxx::nio::channels::SocketChannel>(c);if(sc)return (o&~sc->validOps())==0;auto ss=::jxx::CAST<::jxx::nio::channels::ServerSocketChannel>(c);if(ss)return (o&~::jxx::nio::channels::SelectionKey::OP_ACCEPT_)==0;return false;}
::jxx::Ptr<::jxx::nio::channels::SelectionKey> NativeSelector::keyFor(const ::jxx::Ptr<::jxx::lang::Object>&c)const{std::lock_guard<std::mutex>l(mutex_);for(auto&k:keys_)if(k&&k->isValid()&&k->channel().get()==c.get())return k;return nullptr;}
::jxx::Ptr<::jxx::nio::channels::SelectionKey> NativeSelector::registerChannel(const ::jxx::Ptr<::jxx::lang::Object>&c,::jxx::lang::jint o,const ::jxx::Ptr<::jxx::lang::Object>&a){if(!isOpen())throw ::jxx::nio::channels::ClosedSelectorException();if(!c)throw ::jxx::lang::NullPointerException();if(!validOps_(c,o))throw ::jxx::lang::IllegalArgumentException();auto old=keyFor(c);if(old){old->interestOps(o);old->attach(a);return old;}auto selectable = ::jxx::CAST<::jxx::nio::channels::SelectableChannel>(c);if(selectable==nullptr)throw ::jxx::lang::IllegalArgumentException();auto k=::jxx::CAST<::jxx::nio::channels::SelectionKey>(::jxx::NEW<::jxx::nio::channels::spi::internal::NativeSelectionKey>(selectable,::jxx::CAST<::jxx::nio::channels::Selector>(thisPtr()),o,a));{std::lock_guard<std::mutex>l(mutex_);keys_.push_back(k);}wakeup();return k;}
::jxx::lang::jint NativeSelector::select(){return select_(-1);}
::jxx::lang::jint NativeSelector::select(::jxx::lang::jlong t){if(t<0)throw ::jxx::lang::IllegalArgumentException();return select_(t);}
::jxx::lang::jint NativeSelector::selectNow(){return select_(0);}
::jxx::Ptr<::jxx::nio::channels::Selector> NativeSelector::wakeup(){bool expected=false;if(wakeupPending_.compare_exchange_strong(expected,true))signalWakeup_();return ::jxx::CAST<::jxx::nio::channels::Selector>(thisPtr());}
::jxx::lang::jint NativeSelector::select_(::jxx::lang::jlong timeout){if(!isOpen())throw ::jxx::nio::channels::ClosedSelectorException();struct SelectionGuard{NativeSelector*owner;explicit SelectionGuard(NativeSelector*value):owner(value){std::lock_guard<std::mutex>l(owner->mutex_);++owner->activeSelections_;}~SelectionGuard(){std::lock_guard<std::mutex>l(owner->mutex_);--owner->activeSelections_;owner->selectionCondition_.notify_all();}}selectionGuard(this);processCancelled_();fd_set rd,wr,ex;FD_ZERO(&rd);FD_ZERO(&wr);FD_ZERO(&ex);FD_SET(wakeRead_,&rd);int maxfd=(int)wakeRead_;std::vector<::jxx::Ptr<::jxx::nio::channels::SelectionKey>> live;{std::lock_guard<std::mutex>l(mutex_);live=keys_;}for(auto&k:live){if(!k||!k->isValid())continue;auto sc=::jxx::CAST<::jxx::nio::channels::SocketChannel>(k->channel());auto ss=::jxx::CAST<::jxx::nio::channels::ServerSocketChannel>(k->channel());auto h=sc?sc->socket()->nativeSocketHandle():(ss ? ss->socket()->nativeSocketHandle() : ::jxx::net::internal::kInvalidSocket);if(h==::jxx::net::internal::kInvalidSocket)continue;int ops=k->interestOps();if(ops&(::jxx::nio::channels::SelectionKey::OP_READ_|::jxx::nio::channels::SelectionKey::OP_ACCEPT_))FD_SET(h,&rd);if(ops&(::jxx::nio::channels::SelectionKey::OP_WRITE_|::jxx::nio::channels::SelectionKey::OP_CONNECT_))FD_SET(h,&wr);FD_SET(h,&ex);if((int)h>maxfd)maxfd=(int)h;}timeval tv{},*ptv=nullptr;if(timeout>=0){tv.tv_sec=(long)(timeout/1000);tv.tv_usec=(long)((timeout%1000)*1000);ptv=&tv;}int n=::select(
#if defined(_WIN32)
0,
#else
maxfd+1,
#endif
&rd,&wr,&ex,ptv);if(n<0){if(!isOpen())return 0;throw ::jxx::io::IOException("selector operation failed");}if(FD_ISSET(wakeRead_,&rd)){drainWakeup_();if(--n<=0){processCancelled_();return 0;}}if(n==0)return 0;int changed=0;for(auto&k:live){if(!k||!k->isValid())continue;auto sc=::jxx::CAST<::jxx::nio::channels::SocketChannel>(k->channel());auto ss=::jxx::CAST<::jxx::nio::channels::ServerSocketChannel>(k->channel());auto h=sc?sc->socket()->nativeSocketHandle():ss->socket()->nativeSocketHandle();int ready=0,interest=k->interestOps();if(FD_ISSET(h,&rd))ready|=interest&(::jxx::nio::channels::SelectionKey::OP_READ_|::jxx::nio::channels::SelectionKey::OP_ACCEPT_);if(FD_ISSET(h,&wr))ready|=interest&(::jxx::nio::channels::SelectionKey::OP_WRITE_|::jxx::nio::channels::SelectionKey::OP_CONNECT_);if(FD_ISSET(h,&ex))ready|=interest&::jxx::nio::channels::SelectionKey::OP_CONNECT_;if(!ready)continue;bool already=selected_->contains(::jxx::CAST<::jxx::lang::Object>(k));int old=already?k->readyOps():0;int merged=already?(old|ready):ready;k->setReadyOps_(merged);if(!already){selected_->add(k);++changed;}else if(merged!=old)++changed;}processCancelled_();return changed;}
}
