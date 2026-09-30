#pragma once
#include "nio/channels/jxx.nio.channels.Selector.h"
namespace jxx::nio::channels::spi {
class AbstractSelector : public ::jxx::lang::ClassBase<AbstractSelector,::jxx::nio::channels::Selector> {
public:
 using JxxSuper=::jxx::nio::channels::Selector;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<AbstractSelector,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 ~AbstractSelector()override=default;
 ::jxx::Ptr<SelectorProvider> provider()const override;
protected:
 explicit AbstractSelector(const ::jxx::Ptr<SelectorProvider>& provider);
 void begin();
 void end();
private:
 ::jxx::Ptr<SelectorProvider> provider_;
}; }
