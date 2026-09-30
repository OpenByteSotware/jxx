#pragma once
#include <mutex>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::nio::channels { class Selector;
class SelectionKey final : public ::jxx::lang::ClassBase<SelectionKey,::jxx::lang::Object> {
public:
 static constexpr ::jxx::lang::jint OP_READ_=1;
 static constexpr ::jxx::lang::jint OP_WRITE_=4;
 static constexpr ::jxx::lang::jint OP_CONNECT_=8;
 static constexpr ::jxx::lang::jint OP_ACCEPT_=16;
 SelectionKey(const ::jxx::Ptr<::jxx::lang::Object>& channel,const ::jxx::Ptr<Selector>& selector,::jxx::lang::jint ops,const ::jxx::Ptr<::jxx::lang::Object>& attachment);
 ::jxx::Ptr<::jxx::lang::Object> channel() const;
 ::jxx::Ptr<Selector> selector() const;
 ::jxx::lang::jbool isValid() const noexcept;
 void cancel();
 ::jxx::lang::jint interestOps() const;
 ::jxx::Ptr<SelectionKey> interestOps(::jxx::lang::jint ops);
 ::jxx::lang::jint readyOps() const;
 ::jxx::lang::jbool isReadable() const; ::jxx::lang::jbool isWritable() const;
 ::jxx::lang::jbool isConnectable() const; ::jxx::lang::jbool isAcceptable() const;
 ::jxx::Ptr<::jxx::lang::Object> attach(const ::jxx::Ptr<::jxx::lang::Object>& value);
 ::jxx::Ptr<::jxx::lang::Object> attachment() const;
private:
 friend class Selector;
 void setReadyOps_(::jxx::lang::jint ops);
 mutable std::mutex mutex_; ::jxx::Ptr<::jxx::lang::Object> channel_;
 std::weak_ptr<Selector> selector_; ::jxx::lang::jint interestOps_=0,readyOps_=0;
 ::jxx::lang::jbool valid_=true; ::jxx::Ptr<::jxx::lang::Object> attachment_;
}; }
