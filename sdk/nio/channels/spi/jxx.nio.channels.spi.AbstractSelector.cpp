#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelector.h"
#include "lang/jxx.lang.Thread.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
namespace jxx::nio::channels::spi {
::jxx::Ptr<::jxx::lang::ClassAny> AbstractSelector::Class(){return JxxClassInfoMarker::Class();}
AbstractSelector::AbstractSelector(const ::jxx::Ptr<SelectorProvider>&p):provider_(p){}
::jxx::Ptr<SelectorProvider> AbstractSelector::provider()const{return provider_;}
void AbstractSelector::begin(){const auto t=::jxx::lang::Thread::currentThread();if(t!=nullptr)t->setParkWakeup_([self=::jxx::CAST<::jxx::nio::channels::Selector>(thisPtr())]{if(self!=nullptr)self->wakeup();});}
void AbstractSelector::end(){const auto t=::jxx::lang::Thread::currentThread();if(t!=nullptr)t->clearParkWakeup_();}
}
