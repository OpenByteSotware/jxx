#pragma once
#include <mutex>
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectionKey.h"
namespace jxx::nio::channels::spi::internal {
class NativeSelectionKey final : public ::jxx::lang::ClassBase<NativeSelectionKey,::jxx::nio::channels::spi::AbstractSelectionKey> {
public:
 using JxxSuper=::jxx::nio::channels::spi::AbstractSelectionKey;
 NativeSelectionKey(const ::jxx::Ptr<::jxx::nio::channels::SelectableChannel>& channel,const ::jxx::Ptr<::jxx::nio::channels::Selector>& selector,::jxx::lang::jint operations,const ::jxx::Ptr<::jxx::lang::Object>& attachment);
 ~NativeSelectionKey()override=default;
 ::jxx::Ptr<::jxx::nio::channels::SelectableChannel> channel()const override;
 ::jxx::Ptr<::jxx::nio::channels::Selector> selector()const override;
 ::jxx::lang::jint interestOps()const override;
 ::jxx::Ptr<::jxx::nio::channels::SelectionKey> interestOps(::jxx::lang::jint operations)override;
 ::jxx::lang::jint readyOps()const override;
private:
 void setReadyOps_(::jxx::lang::jint operations)override;
 mutable std::mutex mutex_;
 ::jxx::Ptr<::jxx::nio::channels::SelectableChannel> channel_;
 std::weak_ptr<::jxx::nio::channels::Selector> selector_;
 ::jxx::lang::jint interestOps_=0;
 ::jxx::lang::jint readyOps_=0;
}; }
