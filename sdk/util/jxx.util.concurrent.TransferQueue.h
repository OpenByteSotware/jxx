#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.concurrent.BlockingQueue.h"
namespace jxx::util::concurrent {
template<typename E>
class TransferQueue : public ::jxx::lang::InterfaceBase<TransferQueue<E>, BlockingQueue<E>> {
public:
 using Super=::jxx::lang::InterfaceBase<TransferQueue<E>,BlockingQueue<E>>;
 using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 ~TransferQueue() override=default;
 virtual void transfer(const ::jxx::Ptr<E>& element)=0;
 virtual ::jxx::lang::jbool tryTransfer(const ::jxx::Ptr<E>& element)=0;
 virtual ::jxx::lang::jbool tryTransfer(const ::jxx::Ptr<E>& element,::jxx::lang::jlong timeout,const ::jxx::Ptr<TimeUnit>& unit)=0;
 virtual ::jxx::lang::jbool hasWaitingConsumer()=0;
 virtual ::jxx::lang::jint getWaitingConsumerCount()=0;
};
}
