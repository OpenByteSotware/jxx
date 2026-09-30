#pragma once
#include <atomic>
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
namespace jxx::nio::channels::spi {
class AbstractSelectionKey : public ::jxx::lang::ClassBase<AbstractSelectionKey,::jxx::nio::channels::SelectionKey> {
public:
 using JxxSuper=::jxx::nio::channels::SelectionKey;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<AbstractSelectionKey,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 ~AbstractSelectionKey()override=default;
 ::jxx::lang::jbool isValid()const noexcept final;
 void cancel()final;
protected:
 AbstractSelectionKey()=default;
 void invalidate_()noexcept;
private:
 std::atomic<::jxx::lang::jbool> valid_{true};
}; }
