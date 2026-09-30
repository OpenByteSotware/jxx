#include "nio/channels/jxx.nio.channels.Selector.h"
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <sys/select.h>
#endif
#include <algorithm>
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
::jxx::Ptr<Selector> Selector::open(){auto s=::jxx::NEW<Selector>();s->selected_=::jxx::NEW<::jxx::util::HashSet<SelectionKey>>();return s;}
::jxx::lang::jbool Selector::isOpen()const noexcept{return open_;}
void Selector::close(){open_=false;std::lock_guard<std::mutex>l(mutex_);for(auto&k:keys_)if(k)k->cancel();keys_.clear();if(selected_)selected_->clear();}
::jxx::Ptr<::jxx::util::Set<SelectionKey>> Selector::keys(){if(!open_)throw ClosedSelectorException();auto r=::jxx::NEW<::jxx::util::HashSet<SelectionKey>>();std::lock_guard<std::mutex>l(mutex_);for(auto&k:keys_)if(k&&k->isValid())r->add(k);return ::jxx::CAST<::jxx::util::Set<SelectionKey>>(r);}
::jxx::Ptr<::jxx::util::Set<SelectionKey>> Selector::selectedKeys(){if(!open_)throw ClosedSelectorException();return selected_;}
::jxx::lang::jbool Selector::validOps_(const ::jxx::Ptr<::jxx::lang::Object>&c,::jxx::lang::jint o)const{auto sc=::jxx::CAST<SocketChannel>(c);if(sc)return (o&~sc->validOps())==0;auto ss=::jxx::CAST<ServerSocketChannel>(c);if(ss)return (o&~SelectionKey::OP_ACCEPT_)==0;return false;}
::jxx::Ptr<SelectionKey> Selector::keyFor(const ::jxx::Ptr<::jxx::lang::Object>&c)const{std::lock_guard<std::mutex>l(mutex_);for(auto&k:keys_)if(k&&k->isValid()&&k->channel().get()==c.get())return k;return nullptr;}
::jxx::Ptr<SelectionKey> Selector::registerChannel(const ::jxx::Ptr<::jxx::lang::Object>&c,::jxx::lang::jint o,const ::jxx::Ptr<::jxx::lang::Object>&a){if(!open_)throw ClosedSelectorException();if(!c)throw ::jxx::lang::NullPointerException();if(!validOps_(c,o))throw ::jxx::lang::IllegalArgumentException();auto old=keyFor(c);if(old){old->interestOps(o);old->attach(a);return old;}auto k=::jxx::NEW<SelectionKey>(c,::jxx::CAST<Selector>(thisPtr()),o,a);std::lock_guard<std::mutex>l(mutex_);keys_.push_back(k);return k;}
::jxx::lang::jint Selector::select(){return select_(-1);}
::jxx::lang::jint Selector::select(::jxx::lang::jlong t){if(t<0)throw ::jxx::lang::IllegalArgumentException();return select_(t);}
::jxx::lang::jint Selector::selectNow(){return select_(0);}
::jxx::Ptr<Selector> Selector::wakeup(){wakeup_=true;return ::jxx::CAST<Selector>(thisPtr());}
::jxx::lang::jint Selector::select_(::jxx::lang::jlong timeout){if(!open_)throw ClosedSelectorException();if(wakeup_.exchange(false))return 0;fd_set rd,wr,ex;FD_ZERO(&rd);FD_ZERO(&wr);FD_ZERO(&ex);auto snapshot=keys();auto it=snapshot->iterator();int maxfd=0;std::vector<::jxx::Ptr<SelectionKey>> live;while(it->hasNext()){auto k=it->next();auto sc=::jxx::CAST<SocketChannel>(k->channel());auto ss=::jxx::CAST<ServerSocketChannel>(k->channel());auto h=sc?sc->socket()->nativeSocketHandle():(ss ? ss->socket()->nativeSocketHandle() : ::jxx::net::internal::kInvalidSocket);if(h==::jxx::net::internal::kInvalidSocket)continue;int ops=k->interestOps();if(ops&(SelectionKey::OP_READ_|SelectionKey::OP_ACCEPT_))FD_SET(h,&rd);if(ops&(SelectionKey::OP_WRITE_|SelectionKey::OP_CONNECT_))FD_SET(h,&wr);FD_SET(h,&ex);if((int)h>maxfd)maxfd=(int)h;live.push_back(k);}timeval tv{},*ptv=nullptr;if(timeout>=0){tv.tv_sec=(long)(timeout/1000);tv.tv_usec=(long)((timeout%1000)*1000);ptv=&tv;}int n=::select(
#if defined(_WIN32)
0,
#else
maxfd+1,
#endif
&rd,&wr,&ex,ptv);if(n<=0)return 0;int changed=0;for(auto&k:live){auto sc=::jxx::CAST<SocketChannel>(k->channel());auto ss=::jxx::CAST<ServerSocketChannel>(k->channel());auto h=sc?sc->socket()->nativeSocketHandle():ss->socket()->nativeSocketHandle();int ready=0,interest=k->interestOps();if(FD_ISSET(h,&rd))ready|=interest&(SelectionKey::OP_READ_|SelectionKey::OP_ACCEPT_);if(FD_ISSET(h,&wr))ready|=interest&(SelectionKey::OP_WRITE_|SelectionKey::OP_CONNECT_);if(FD_ISSET(h,&ex))ready|=interest&SelectionKey::OP_CONNECT_;k->setReadyOps_(ready);if(ready){selected_->add(k);++changed;}}return changed;}
}
