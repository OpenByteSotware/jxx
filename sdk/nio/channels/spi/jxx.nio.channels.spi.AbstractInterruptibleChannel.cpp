#include "nio/channels/spi/jxx.nio.channels.spi.AbstractInterruptibleChannel.h"
#include "lang/jxx.lang.Thread.h"
#include "nio/channels/jxx.nio.channels.ClosedByInterruptException.h"
namespace jxx::nio::channels::spi {
::jxx::Ptr<::jxx::lang::ClassAny> AbstractInterruptibleChannel::Class(){return JxxClassInfoMarker::Class();}
::jxx::lang::jbool AbstractInterruptibleChannel::isOpen()const{return open_.load(std::memory_order_acquire);}
void AbstractInterruptibleChannel::close(){::jxx::lang::jbool expected=true;if(!open_.compare_exchange_strong(expected,false,std::memory_order_acq_rel))return;implCloseChannel();}
void AbstractInterruptibleChannel::begin(){const auto thread=::jxx::lang::Thread::currentThread();if(thread!=nullptr&&thread->isInterrupted()){close();throw ::jxx::nio::channels::ClosedByInterruptException();}}
void AbstractInterruptibleChannel::end(::jxx::lang::jbool completed){const auto thread=::jxx::lang::Thread::currentThread();if(!completed&&thread!=nullptr&&thread->isInterrupted()){close();throw ::jxx::nio::channels::ClosedByInterruptException();}}
}
