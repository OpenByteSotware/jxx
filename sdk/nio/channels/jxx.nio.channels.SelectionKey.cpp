#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "nio/channels/jxx.nio.channels.CancelledKeyException.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
namespace jxx::nio::channels {
SelectionKey::SelectionKey(const ::jxx::Ptr<::jxx::lang::Object>&c,const ::jxx::Ptr<Selector>&s,::jxx::lang::jint o,const ::jxx::Ptr<::jxx::lang::Object>&a):channel_(c),selector_(s),interestOps_(o),attachment_(a){}
::jxx::Ptr<::jxx::lang::Object> SelectionKey::channel()const{return channel_;}
::jxx::Ptr<Selector> SelectionKey::selector()const{return selector_.lock();}
::jxx::lang::jbool SelectionKey::isValid()const noexcept{return valid_;}
void SelectionKey::cancel(){std::lock_guard<std::mutex>l(mutex_);valid_=false;readyOps_=0;}
::jxx::lang::jint SelectionKey::interestOps()const{if(!valid_)throw CancelledKeyException();return interestOps_;}
::jxx::Ptr<SelectionKey> SelectionKey::interestOps(::jxx::lang::jint o){if(!valid_)throw CancelledKeyException();auto s=selector_.lock();if(s==nullptr||!s->validOps_(channel_,o))throw ::jxx::lang::IllegalArgumentException();interestOps_=o;return ::jxx::CAST<SelectionKey>(thisPtr());}
::jxx::lang::jint SelectionKey::readyOps()const{if(!valid_)throw CancelledKeyException();return readyOps_;}
::jxx::lang::jbool SelectionKey::isReadable()const{return (readyOps()&OP_READ_)!=0;}
::jxx::lang::jbool SelectionKey::isWritable()const{return (readyOps()&OP_WRITE_)!=0;}
::jxx::lang::jbool SelectionKey::isConnectable()const{return (readyOps()&OP_CONNECT_)!=0;}
::jxx::lang::jbool SelectionKey::isAcceptable()const{return (readyOps()&OP_ACCEPT_)!=0;}
::jxx::Ptr<::jxx::lang::Object> SelectionKey::attach(const ::jxx::Ptr<::jxx::lang::Object>&v){auto p=attachment_;attachment_=v;return p;}
::jxx::Ptr<::jxx::lang::Object> SelectionKey::attachment()const{return attachment_;}
void SelectionKey::setReadyOps_(::jxx::lang::jint o){readyOps_=o;}
}
