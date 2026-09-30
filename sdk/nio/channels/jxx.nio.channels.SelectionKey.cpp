#include "nio/channels/jxx.nio.channels.SelectionKey.h"
namespace jxx::nio::channels {
::jxx::Ptr<::jxx::lang::ClassAny> SelectionKey::Class(){return JxxClassInfoMarker::Class();}
::jxx::lang::jbool SelectionKey::isReadable()const{return (readyOps()&OP_READ_)!=0;}
::jxx::lang::jbool SelectionKey::isWritable()const{return (readyOps()&OP_WRITE_)!=0;}
::jxx::lang::jbool SelectionKey::isConnectable()const{return (readyOps()&OP_CONNECT_)!=0;}
::jxx::lang::jbool SelectionKey::isAcceptable()const{return (readyOps()&OP_ACCEPT_)!=0;}
::jxx::Ptr<::jxx::lang::Object> SelectionKey::attach(const ::jxx::Ptr<::jxx::lang::Object>&v){std::lock_guard<std::mutex>l(attachmentMutex_);auto old=attachment_;attachment_=v;return old;}
::jxx::Ptr<::jxx::lang::Object> SelectionKey::attachment()const{std::lock_guard<std::mutex>l(attachmentMutex_);return attachment_;}
}
