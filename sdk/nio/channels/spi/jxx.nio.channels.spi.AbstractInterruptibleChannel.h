#pragma once
#include <atomic>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "nio/channels/jxx.nio.channels.InterruptibleChannel.h"
namespace jxx::nio::channels::spi {
class AbstractInterruptibleChannel
 : public ::jxx::lang::ClassBase<AbstractInterruptibleChannel,::jxx::lang::Object,::jxx::nio::channels::InterruptibleChannel> {
public:
 using JxxSuper=::jxx::lang::Object;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<AbstractInterruptibleChannel,JxxSuper,::jxx::nio::channels::InterruptibleChannel>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 ~AbstractInterruptibleChannel() override=default;
 ::jxx::lang::jbool isOpen() const override;
 void close() override;
protected:
 AbstractInterruptibleChannel()=default;
 virtual void implCloseChannel()=0;
 void begin();
 void end(::jxx::lang::jbool completed);
private:
 std::atomic<::jxx::lang::jbool> open_{true};
};
}
