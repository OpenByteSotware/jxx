#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectableChannel.h"
#include <algorithm>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.IllegalBlockingModeException.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectionKey.h"
namespace jxx::nio::channels::spi {
::jxx::Ptr<::jxx::lang::ClassAny> AbstractSelectableChannel::Class(){return JxxClassInfoMarker::Class();}
AbstractSelectableChannel::AbstractSelectableChannel(const ::jxx::Ptr<SelectorProvider>&p):provider_(p),blockingLock_(::jxx::NEW<::jxx::lang::Object>()){}
::jxx::Ptr<SelectorProvider> AbstractSelectableChannel::provider()const{return provider_==nullptr?SelectorProvider::provider():provider_;}
void AbstractSelectableChannel::purgeCancelled_()const{keys_.erase(std::remove_if(keys_.begin(),keys_.end(),[](const auto&k){return k==nullptr||!k->isValid();}),keys_.end());}
::jxx::lang::jbool AbstractSelectableChannel::isRegistered()const{std::lock_guard<std::mutex>l(mutex_);purgeCancelled_();return !keys_.empty();}
::jxx::Ptr<::jxx::nio::channels::SelectionKey> AbstractSelectableChannel::keyFor(const ::jxx::Ptr<::jxx::nio::channels::Selector>&s)const{if(s==nullptr)return nullptr;std::lock_guard<std::mutex>l(mutex_);purgeCancelled_();for(const auto&k:keys_)if(k->selector().get()==s.get())return k;return nullptr;}
::jxx::Ptr<::jxx::nio::channels::SelectionKey> AbstractSelectableChannel::register_(const ::jxx::Ptr<::jxx::nio::channels::Selector>&s,::jxx::lang::jint o){return register_(s,o,nullptr);}
::jxx::Ptr<::jxx::nio::channels::SelectionKey> AbstractSelectableChannel::register_(const ::jxx::Ptr<::jxx::nio::channels::Selector>&s,::jxx::lang::jint o,const ::jxx::Ptr<::jxx::lang::Object>&a){if(s==nullptr)throw ::jxx::lang::NullPointerException();if(!isOpen())throw ::jxx::nio::channels::ClosedChannelException();if((o&~validOps())!=0)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex>l(mutex_);purgeCancelled_();if(blocking_)throw ::jxx::nio::channels::IllegalBlockingModeException();for(const auto&k:keys_)if(k->selector().get()==s.get()){k->interestOps(o);k->attach(a);return k;}auto self=::jxx::CAST<::jxx::nio::channels::SelectableChannel>(thisPtr());auto k=s->registerChannel(::jxx::CAST<::jxx::lang::Object>(self),o,a);keys_.push_back(k);return k;}
::jxx::Ptr<::jxx::nio::channels::SelectableChannel> AbstractSelectableChannel::configureBlocking(::jxx::lang::jbool b){std::lock_guard<std::mutex>l(mutex_);purgeCancelled_();if(!isOpen())throw ::jxx::nio::channels::ClosedChannelException();if(b&&!keys_.empty())throw ::jxx::nio::channels::IllegalBlockingModeException();implConfigureBlocking(b);blocking_=b;return ::jxx::CAST<::jxx::nio::channels::SelectableChannel>(thisPtr());}
::jxx::lang::jbool AbstractSelectableChannel::isBlocking()const noexcept{try{std::lock_guard<std::mutex>l(mutex_);return blocking_;}catch(...){return true;}}
::jxx::Ptr<::jxx::lang::Object> AbstractSelectableChannel::blockingLock(){return blockingLock_;}
void AbstractSelectableChannel::implCloseChannel() {
    std::vector<::jxx::Ptr<::jxx::nio::channels::SelectionKey>> keys;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        keys = keys_;
        keys_.clear();
    }

    for (const auto& key : keys) {
        if (key == nullptr) {
            continue;
        }

        const auto abstractKey =
            ::jxx::CAST<AbstractSelectionKey>(key);
        if (abstractKey != nullptr) {
            abstractKey->cancelWithReference_(key);
        } else {
            key->cancel();
        }
    }

    implCloseSelectableChannel();
}
void AbstractSelectableChannel::removeKey_(
    const ::jxx::Ptr<::jxx::nio::channels::SelectionKey>& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    keys_.erase(
        std::remove(keys_.begin(), keys_.end(), key),
        keys_.end());
}
}
