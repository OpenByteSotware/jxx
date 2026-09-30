#include "nio/channels/jxx.nio.channels.Selector.h"
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
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "util/jxx.util.HashSet.h"
namespace jxx::nio::channels {
namespace {
void nonblocking(::jxx::net::internal::NativeSocket s){
#if defined(_WIN32)
 u_long m=1;if(::ioctlsocket(s,FIONBIO,&m)!=0)throw ::jxx::io::IOException("selector wakeup socket configuration failed");
#else
 int f=::fcntl(s,F_GETFL,0);if(f<0||::fcntl(s,F_SETFL,f|O_NONBLOCK)!=0)throw ::jxx::io::IOException("selector wakeup socket configuration failed");
#endif
}
}
Selector::Selector(){selected_=::jxx::NEW<::jxx::util::HashSet<SelectionKey>>();initializeWakeup_();}
Selector::~Selector(){try{close();}catch(...){}}
::jxx::Ptr<Selector> Selector::open(){return ::jxx::NEW<Selector>();}
void Selector::initializeWakeup_(){::jxx::net::internal::ensureNetworkInitialized();
#if defined(_WIN32)
 auto listener=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(listener==INVALID_SOCKET)throw ::jxx::io::IOException("selector wakeup listener failed");sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;if(::bind(listener,reinterpret_cast<sockaddr*>(&a),sizeof(a))!=0||::listen(listener,1)!=0){::closesocket(listener);throw ::jxx::io::IOException("selector wakeup bind failed");}int n=sizeof(a);::getsockname(listener,reinterpret_cast<sockaddr*>(&a),&n);wakeWrite_=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(wakeWrite_==INVALID_SOCKET||::connect(wakeWrite_,reinterpret_cast<sockaddr*>(&a),sizeof(a))!=0){::closesocket(listener);if(wakeWrite_!=INVALID_SOCKET)::closesocket(wakeWrite_);throw ::jxx::io::IOException("selector wakeup connect failed");}wakeRead_=::accept(listener,nullptr,nullptr);::closesocket(listener);if(wakeRead_==INVALID_SOCKET)throw ::jxx::io::IOException("selector wakeup accept failed");
#else
 int pair[2];if(::socketpair(AF_UNIX,SOCK_STREAM,0,pair)!=0)throw ::jxx::io::IOException("selector wakeup socketpair failed");wakeRead_=pair[0];wakeWrite_=pair[1];
#endif
 nonblocking(wakeRead_);nonblocking(wakeWrite_);
}
::jxx::lang::jbool Selector::isOpen()const noexcept{return open_.load();}
void Selector::signalWakeup_()noexcept{if(wakeWrite_==::jxx::net::internal::kInvalidSocket)return;const char b=1;
#if defined(_WIN32)
 ::send(wakeWrite_,&b,1,0);
#else
 ::send(wakeWrite_,&b,1,MSG_NOSIGNAL);
#endif
}
void Selector::drainWakeup_()noexcept{char b[64];while(wakeRead_!=::jxx::net::internal::kInvalidSocket){int n=::recv(wakeRead_,b,sizeof(b),0);if(n<=0)break;}wakeupPending_=false;}
void Selector::close(){bool expected=true;if(!open_.compare_exchange_strong(expected,false))return;signalWakeup_();std::vector<::jxx::Ptr<SelectionKey>> keys;{std::lock_guard<std::mutex>l(mutex_);keys=keys_;keys_.clear();cancelled_.clear();if(selected_)selected_->clear();}for(auto&k:keys)if(k)k->cancel();::jxx::net::internal::closeNativeSocket(wakeRead_);::jxx::net::internal::closeNativeSocket(wakeWrite_);wakeRead_=wakeWrite_=::jxx::net::internal::kInvalidSocket;}
void Selector::processCancelled_(){std::lock_guard<std::mutex>l(mutex_);for(auto&k:cancelled_){selected_->remove(::jxx::CAST<::jxx::lang::Object>(k));keys_.erase(std::remove(keys_.begin(),keys_.end(),k),keys_.end());}cancelled_.clear();}
void Selector::cancelKey(const ::jxx::Ptr<SelectionKey>&k){{std::lock_guard<std::mutex>l(mutex_);if(std::find(cancelled_.begin(),cancelled_.end(),k)==cancelled_.end())cancelled_.push_back(k);}wakeup();}
::jxx::Ptr<::jxx::util::Set<SelectionKey>> Selector::keys(){if(!isOpen())throw ClosedSelectorException();processCancelled_();auto r=::jxx::NEW<::jxx::util::HashSet<SelectionKey>>();std::lock_guard<std::mutex>l(mutex_);for(auto&k:keys_)if(k&&k->isValid())r->add(k);return ::jxx::CAST<::jxx::util::Set<SelectionKey>>(r);}
::jxx::Ptr<::jxx::util::Set<SelectionKey>> Selector::selectedKeys(){if(!isOpen())throw ClosedSelectorException();return selected_;}
::jxx::lang::jbool Selector::validOps_(const ::jxx::Ptr<::jxx::lang::Object>&c,::jxx::lang::jint o)const{auto sc=::jxx::CAST<SocketChannel>(c);if(sc)return (o&~sc->validOps())==0;auto ss=::jxx::CAST<ServerSocketChannel>(c);if(ss)return (o&~SelectionKey::OP_ACCEPT_)==0;return false;}
::jxx::Ptr<SelectionKey> Selector::keyFor(const ::jxx::Ptr<::jxx::lang::Object>&c)const{std::lock_guard<std::mutex>l(mutex_);for(auto&k:keys_)if(k&&k->isValid()&&k->channel().get()==c.get())return k;return nullptr;}
::jxx::Ptr<SelectionKey> Selector::registerChannel(const ::jxx::Ptr<::jxx::lang::Object>&c,::jxx::lang::jint o,const ::jxx::Ptr<::jxx::lang::Object>&a){if(!isOpen())throw ClosedSelectorException();if(!c)throw ::jxx::lang::NullPointerException();if(!validOps_(c,o))throw ::jxx::lang::IllegalArgumentException();auto old=keyFor(c);if(old){old->interestOps(o);old->attach(a);return old;}auto k=::jxx::NEW<SelectionKey>(c,::jxx::CAST<Selector>(thisPtr()),o,a);{std::lock_guard<std::mutex>l(mutex_);keys_.push_back(k);}wakeup();return k;}
::jxx::lang::jint Selector::select(){return select_(-1);}
::jxx::lang::jint Selector::select(::jxx::lang::jlong t){if(t<0)throw ::jxx::lang::IllegalArgumentException();return select_(t);}
::jxx::lang::jint Selector::selectNow(){return select_(0);}
::jxx::Ptr<Selector> Selector::wakeup(){bool expected=false;if(wakeupPending_.compare_exchange_strong(expected,true))signalWakeup_();return ::jxx::CAST<Selector>(thisPtr());}
::jxx::lang::jint Selector::select_(::jxx::lang::jlong timeout){if(!isOpen())throw ClosedSelectorException();processCancelled_();fd_set rd,wr,ex;FD_ZERO(&rd);FD_ZERO(&wr);FD_ZERO(&ex);FD_SET(wakeRead_,&rd);int maxfd=(int)wakeRead_;std::vector<::jxx::Ptr<SelectionKey>> live;{std::lock_guard<std::mutex>l(mutex_);live=keys_;}for(auto&k:live){if(!k||!k->isValid())continue;auto sc=::jxx::CAST<SocketChannel>(k->channel());auto ss=::jxx::CAST<ServerSocketChannel>(k->channel());auto h=sc?sc->socket()->nativeSocketHandle():(ss ? ss->socket()->nativeSocketHandle() : ::jxx::net::internal::kInvalidSocket);if(h==::jxx::net::internal::kInvalidSocket)continue;int ops=k->interestOps();if(ops&(SelectionKey::OP_READ_|SelectionKey::OP_ACCEPT_))FD_SET(h,&rd);if(ops&(SelectionKey::OP_WRITE_|SelectionKey::OP_CONNECT_))FD_SET(h,&wr);FD_SET(h,&ex);if((int)h>maxfd)maxfd=(int)h;}timeval tv{},*ptv=nullptr;if(timeout>=0){tv.tv_sec=(long)(timeout/1000);tv.tv_usec=(long)((timeout%1000)*1000);ptv=&tv;}int n=::select(
#if defined(_WIN32)
0,
#else
maxfd+1,
#endif
&rd,&wr,&ex,ptv);if(n<0){if(!isOpen())return 0;throw ::jxx::io::IOException("selector operation failed");}if(FD_ISSET(wakeRead_,&rd)){drainWakeup_();if(--n<=0){processCancelled_();return 0;}}if(n==0)return 0;int changed=0;for(auto&k:live){if(!k||!k->isValid())continue;auto sc=::jxx::CAST<SocketChannel>(k->channel());auto ss=::jxx::CAST<ServerSocketChannel>(k->channel());auto h=sc?sc->socket()->nativeSocketHandle():ss->socket()->nativeSocketHandle();int ready=0,interest=k->interestOps();if(FD_ISSET(h,&rd))ready|=interest&(SelectionKey::OP_READ_|SelectionKey::OP_ACCEPT_);if(FD_ISSET(h,&wr))ready|=interest&(SelectionKey::OP_WRITE_|SelectionKey::OP_CONNECT_);if(FD_ISSET(h,&ex))ready|=interest&SelectionKey::OP_CONNECT_;if(!ready)continue;bool already=selected_->contains(::jxx::CAST<::jxx::lang::Object>(k));int old=already?k->readyOps():0;int merged=already?(old|ready):ready;k->setReadyOps_(merged);if(!already){selected_->add(k);++changed;}else if(merged!=old)++changed;}processCancelled_();return changed;}
}
