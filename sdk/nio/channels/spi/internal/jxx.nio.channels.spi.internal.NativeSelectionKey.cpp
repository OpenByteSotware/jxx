#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectionKey.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "nio/channels/jxx.nio.channels.CancelledKeyException.h"
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
namespace jxx::nio::channels::spi::internal {
NativeSelectionKey::NativeSelectionKey(const ::jxx::Ptr<::jxx::nio::channels::SelectableChannel>&c,const ::jxx::Ptr<::jxx::nio::channels::Selector>&s,::jxx::lang::jint o,const ::jxx::Ptr<::jxx::lang::Object>&a):channel_(c),selector_(s),interestOps_(o){attach(a);}
::jxx::Ptr<::jxx::nio::channels::SelectableChannel> NativeSelectionKey::channel()const{std::lock_guard<std::mutex>l(mutex_);return channel_;}
::jxx::Ptr<::jxx::nio::channels::Selector> NativeSelectionKey::selector()const{return selector_.lock();}
::jxx::lang::jint NativeSelectionKey::interestOps()const{std::lock_guard<std::mutex>l(mutex_);if(!isValid())throw ::jxx::nio::channels::CancelledKeyException();return interestOps_;}
::jxx::Ptr<::jxx::nio::channels::SelectionKey> NativeSelectionKey::interestOps(::jxx::lang::jint o){const auto owner=selector_.lock();if(owner==nullptr||!owner->validOps_(::jxx::CAST<::jxx::lang::Object>(channel_),o))throw ::jxx::lang::IllegalArgumentException();{std::lock_guard<std::mutex>l(mutex_);if(!isValid())throw ::jxx::nio::channels::CancelledKeyException();interestOps_=o;}owner->wakeup();return ::jxx::CAST<::jxx::nio::channels::SelectionKey>(thisPtr());}
::jxx::lang::jint NativeSelectionKey::readyOps()const{std::lock_guard<std::mutex>l(mutex_);if(!isValid())throw ::jxx::nio::channels::CancelledKeyException();return readyOps_;}
void NativeSelectionKey::setReadyOps_(::jxx::lang::jint o){std::lock_guard<std::mutex>l(mutex_);if(isValid())readyOps_=o;}
}
