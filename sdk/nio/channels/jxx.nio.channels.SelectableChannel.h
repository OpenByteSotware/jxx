#pragma once
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractInterruptibleChannel.h"
namespace jxx::nio::channels { class SelectionKey; class Selector; namespace spi { class SelectorProvider; }
class SelectableChannel
 : public ::jxx::lang::ClassBase<SelectableChannel,::jxx::nio::channels::spi::AbstractInterruptibleChannel> {
public:
 using JxxSuper=::jxx::nio::channels::spi::AbstractInterruptibleChannel;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<SelectableChannel,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 ~SelectableChannel()override=default;
 virtual ::jxx::Ptr<spi::SelectorProvider> provider()const=0;
 virtual ::jxx::lang::jint validOps()const noexcept=0;
 virtual ::jxx::lang::jbool isRegistered()const=0;
 virtual ::jxx::Ptr<SelectionKey> keyFor(const ::jxx::Ptr<Selector>&)const=0;
 virtual ::jxx::Ptr<SelectionKey> register_(const ::jxx::Ptr<Selector>&,::jxx::lang::jint)=0;
 virtual ::jxx::Ptr<SelectionKey> register_(const ::jxx::Ptr<Selector>&,::jxx::lang::jint,const ::jxx::Ptr<::jxx::lang::Object>&)=0;
 virtual ::jxx::Ptr<SelectableChannel> configureBlocking(::jxx::lang::jbool)=0;
 virtual ::jxx::lang::jbool isBlocking()const noexcept=0;
 virtual ::jxx::Ptr<::jxx::lang::Object> blockingLock()=0;
protected:
 SelectableChannel()=default;
}; }
