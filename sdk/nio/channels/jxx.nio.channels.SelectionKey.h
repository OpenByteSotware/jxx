#pragma once
#include <mutex>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::nio::channels { class Selector; class SelectableChannel;
class SelectionKey : public ::jxx::lang::ClassBase<SelectionKey,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<SelectionKey,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 static constexpr ::jxx::lang::jint OP_READ_=1;
 static constexpr ::jxx::lang::jint OP_WRITE_=4;
 static constexpr ::jxx::lang::jint OP_CONNECT_=8;
 static constexpr ::jxx::lang::jint OP_ACCEPT_=16;
 ~SelectionKey()override=default;
 virtual ::jxx::Ptr<SelectableChannel> channel()const=0;
 virtual ::jxx::Ptr<Selector> selector()const=0;
 virtual ::jxx::lang::jbool isValid()const noexcept=0;
 virtual void cancel()=0;
 virtual ::jxx::lang::jint interestOps()const=0;
 virtual ::jxx::Ptr<SelectionKey> interestOps(::jxx::lang::jint ops)=0;
 virtual ::jxx::lang::jint readyOps()const=0;
 ::jxx::lang::jbool isReadable()const;
 ::jxx::lang::jbool isWritable()const;
 ::jxx::lang::jbool isConnectable()const;
 ::jxx::lang::jbool isAcceptable()const;
 ::jxx::Ptr<::jxx::lang::Object> attach(const ::jxx::Ptr<::jxx::lang::Object>& value);
 ::jxx::Ptr<::jxx::lang::Object> attachment()const;
protected:
 SelectionKey()=default;
private:
 friend class Selector;
 virtual void setReadyOps_(::jxx::lang::jint ops)=0;
 mutable std::mutex attachmentMutex_;
 ::jxx::Ptr<::jxx::lang::Object> attachment_;
}; }
