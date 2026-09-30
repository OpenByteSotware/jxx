#pragma once
#include <mutex>
#include <vector>
#include "nio/channels/jxx.nio.channels.Selector.h"
namespace jxx::nio::channels::spi {
class AbstractSelectionKey;
class AbstractSelector : public ::jxx::lang::ClassBase<AbstractSelector,::jxx::nio::channels::Selector> {
public:
 using JxxSuper=::jxx::nio::channels::Selector;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<AbstractSelector,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 ~AbstractSelector()override=default;
 ::jxx::Ptr<SelectorProvider> provider()const override;
protected:
 AbstractSelector();
 explicit AbstractSelector(const ::jxx::Ptr<SelectorProvider>& provider);
 void setProvider_(const ::jxx::Ptr<SelectorProvider>& provider);
 void begin();
 void end();
 void cancel_(
     const ::jxx::Ptr<AbstractSelectionKey>& key);
 std::vector<::jxx::Ptr<AbstractSelectionKey>> takeCancelled_();
 void deregister(
     const ::jxx::Ptr<AbstractSelectionKey>& key);
private:
 friend class AbstractSelectionKey;
 ::jxx::Ptr<SelectorProvider> provider_;
 mutable std::mutex cancelledMutex_;
 std::vector<::jxx::Ptr<AbstractSelectionKey>> cancelledKeys_;
}; }
