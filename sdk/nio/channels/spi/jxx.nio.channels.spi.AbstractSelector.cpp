#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelector.h"
#include <algorithm>
#include "lang/jxx.lang.Thread.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectionKey.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectableChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
namespace jxx::nio::channels::spi {
::jxx::Ptr<::jxx::lang::ClassAny> AbstractSelector::Class(){return JxxClassInfoMarker::Class();}
AbstractSelector::AbstractSelector() = default;
AbstractSelector::AbstractSelector(const ::jxx::Ptr<SelectorProvider>&p):provider_(p){}
void AbstractSelector::setProvider_(const ::jxx::Ptr<SelectorProvider>&p){provider_=p;}
::jxx::Ptr<SelectorProvider> AbstractSelector::provider()const{return provider_;}
void AbstractSelector::begin(){const auto t=::jxx::lang::Thread::currentThread();if(t!=nullptr)t->setParkWakeup_([self=::jxx::CAST<::jxx::nio::channels::Selector>(thisPtr())]{if(self!=nullptr)self->wakeup();});}
void AbstractSelector::end(){const auto t=::jxx::lang::Thread::currentThread();if(t!=nullptr)t->clearParkWakeup_();}
void AbstractSelector::cancel_(
    const ::jxx::Ptr<AbstractSelectionKey>& key) {
    if (key == nullptr) return;
    std::lock_guard<std::mutex> lock(cancelledMutex_);
    if (std::find(cancelledKeys_.begin(), cancelledKeys_.end(), key)
            == cancelledKeys_.end()) {
        cancelledKeys_.push_back(key);
    }
}
std::vector<::jxx::Ptr<AbstractSelectionKey>>
AbstractSelector::takeCancelled_() {
    std::lock_guard<std::mutex> lock(cancelledMutex_);
    std::vector<::jxx::Ptr<AbstractSelectionKey>> result;
    result.swap(cancelledKeys_);
    return result;
}
void AbstractSelector::deregister(
    const ::jxx::Ptr<AbstractSelectionKey>& key) {
    if (key == nullptr) return;
    const auto channel = ::jxx::CAST<AbstractSelectableChannel>(
        key->channel());
    if (channel != nullptr) {
        channel->removeKey_(
            ::jxx::CAST<::jxx::nio::channels::SelectionKey>(key));
    }
}
}
