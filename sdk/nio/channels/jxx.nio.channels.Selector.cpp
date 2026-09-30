#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
namespace jxx::nio::channels {
::jxx::Ptr<::jxx::lang::ClassAny> Selector::Class(){return JxxClassInfoMarker::Class();}
::jxx::Ptr<Selector> Selector::open(){return spi::SelectorProvider::provider()->openSelector();}
}
