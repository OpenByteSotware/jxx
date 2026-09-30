#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "nio/channels/jxx.nio.channels.CancelledKeyException.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
namespace jxx::nio::channels {
SelectionKey::SelectionKey(const ::jxx::Ptr<SelectableChannel>&c,const ::jxx::Ptr<Selector>&s,::jxx::lang::jint o,const ::jxx::Ptr<::jxx::lang::Object>&a):channel_(c),selector_(s),interestOps_(o),attachment_(a){}
::jxx::Ptr<SelectableChannel> SelectionKey::channel()const{std::lock_guard<std::mutex>l(mutex_);return channel_;}
::jxx::Ptr<Selector> SelectionKey::selector()const{return selector_.lock();}
::jxx::lang::jbool SelectionKey::isValid()const noexcept{try{std::lock_guard<std::mutex>l(mutex_);return valid_;}catch(...){return false;}}
void SelectionKey::cancel(){::jxx::Ptr<Selector>s;{std::lock_guard<std::mutex>l(mutex_);if(!valid_)return;valid_=false;readyOps_=0;s=selector_.lock();}if(s)s->cancelKey(::jxx::CAST<SelectionKey>(thisPtr()));}
::jxx::lang::jint SelectionKey::interestOps()const{std::lock_guard<std::mutex>l(mutex_);if(!valid_)throw CancelledKeyException();return interestOps_;}
::jxx::Ptr<SelectionKey> SelectionKey::interestOps(::jxx::lang::jint o){auto s=selector_.lock();if(s==nullptr||!s->validOps_(::jxx::CAST<::jxx::lang::Object>(channel_),o))throw ::jxx::lang::IllegalArgumentException();{std::lock_guard<std::mutex>l(mutex_);if(!valid_)throw CancelledKeyException();interestOps_=o;}s->wakeup();return ::jxx::CAST<SelectionKey>(thisPtr());}
::jxx::lang::jint SelectionKey::readyOps()const{std::lock_guard<std::mutex>l(mutex_);if(!valid_)throw CancelledKeyException();return readyOps_;}
::jxx::lang::jbool SelectionKey::isReadable()const{return (readyOps()&OP_READ_)!=0;}::jxx::lang::jbool SelectionKey::isWritable()const{return (readyOps()&OP_WRITE_)!=0;}::jxx::lang::jbool SelectionKey::isConnectable()const{return (readyOps()&OP_CONNECT_)!=0;}::jxx::lang::jbool SelectionKey::isAcceptable()const{return (readyOps()&OP_ACCEPT_)!=0;}
::jxx::Ptr<::jxx::lang::Object> SelectionKey::attach(const ::jxx::Ptr<::jxx::lang::Object>&v){std::lock_guard<std::mutex>l(mutex_);auto p=attachment_;attachment_=v;return p;}::jxx::Ptr<::jxx::lang::Object> SelectionKey::attachment()const{std::lock_guard<std::mutex>l(mutex_);return attachment_;}
void SelectionKey::setReadyOps_(::jxx::lang::jint o){std::lock_guard<std::mutex>l(mutex_);if(valid_)readyOps_=o;}
}
