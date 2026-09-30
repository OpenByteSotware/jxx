#pragma once
#include <mutex>
#include <vector>
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
namespace jxx::nio::channels::spi {
class AbstractSelector;
class AbstractSelectableChannel
 : public ::jxx::lang::ClassBase<AbstractSelectableChannel,::jxx::nio::channels::SelectableChannel> {
public:
 using JxxSuper=::jxx::nio::channels::SelectableChannel;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<AbstractSelectableChannel,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 ~AbstractSelectableChannel()override=default;
 ::jxx::Ptr<SelectorProvider> provider()const override;
 ::jxx::lang::jbool isRegistered()const override;
 ::jxx::Ptr<::jxx::nio::channels::SelectionKey> keyFor(const ::jxx::Ptr<::jxx::nio::channels::Selector>&)const override;
 ::jxx::Ptr<::jxx::nio::channels::SelectionKey> register_(const ::jxx::Ptr<::jxx::nio::channels::Selector>&,::jxx::lang::jint)override;
 ::jxx::Ptr<::jxx::nio::channels::SelectionKey> register_(const ::jxx::Ptr<::jxx::nio::channels::Selector>&,::jxx::lang::jint,const ::jxx::Ptr<::jxx::lang::Object>&)override;
 ::jxx::Ptr<::jxx::nio::channels::SelectableChannel> configureBlocking(::jxx::lang::jbool)override;
 ::jxx::lang::jbool isBlocking()const noexcept override;
 ::jxx::Ptr<::jxx::lang::Object> blockingLock()override;
protected:
 explicit AbstractSelectableChannel(const ::jxx::Ptr<SelectorProvider>& provider=nullptr);
 void implCloseChannel()final;
 virtual void implCloseSelectableChannel()=0;
 virtual void implConfigureBlocking(::jxx::lang::jbool)=0;
 void removeKey_(
     const ::jxx::Ptr<::jxx::nio::channels::SelectionKey>& key);
private:
 friend class AbstractSelector;
 void purgeCancelled_()const;
 ::jxx::Ptr<SelectorProvider> provider_;
 ::jxx::Ptr<::jxx::lang::Object> blockingLock_;
 mutable std::mutex mutex_;
 mutable std::vector<::jxx::Ptr<::jxx::nio::channels::SelectionKey>> keys_;
 ::jxx::lang::jbool blocking_=true;
}; }
