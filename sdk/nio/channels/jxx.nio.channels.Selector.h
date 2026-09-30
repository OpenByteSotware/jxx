#pragma once
#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.AutoCloseable.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Set.h"
namespace jxx::nio::channels { class SelectionKey; namespace spi { class AbstractSelectionKey; class AbstractSelectableChannel; class SelectorProvider; namespace internal { class NativeSelectionKey; class NativeSelector; } }
class Selector : public ::jxx::lang::ClassBase<Selector,::jxx::lang::Object,::jxx::io::Closeable,::jxx::lang::AutoCloseable> {
public:
 using JxxSuper=::jxx::lang::Object;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<Selector,JxxSuper,::jxx::io::Closeable,::jxx::lang::AutoCloseable>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 static ::jxx::Ptr<Selector> open();
 ~Selector()override=default;
 virtual ::jxx::Ptr<spi::SelectorProvider> provider()const=0;
 virtual ::jxx::lang::jbool isOpen()const noexcept=0;
 virtual void close()override=0;
 virtual ::jxx::Ptr<::jxx::util::Set<SelectionKey>> keys()=0;
 virtual ::jxx::Ptr<::jxx::util::Set<SelectionKey>> selectedKeys()=0;
 virtual ::jxx::lang::jint select()=0;
 virtual ::jxx::lang::jint select(::jxx::lang::jlong timeout)=0;
 virtual ::jxx::lang::jint selectNow()=0;
 virtual ::jxx::Ptr<Selector> wakeup()=0;
protected:
 Selector()=default;
private:
 friend class ::jxx::nio::channels::spi::AbstractSelectionKey;
 friend class ::jxx::nio::channels::spi::AbstractSelectableChannel;
 friend class ::jxx::nio::channels::spi::internal::NativeSelectionKey;
 friend class ::jxx::nio::channels::spi::internal::NativeSelector;
 virtual ::jxx::Ptr<SelectionKey> registerChannel(const ::jxx::Ptr<::jxx::lang::Object>& channel,::jxx::lang::jint operations,const ::jxx::Ptr<::jxx::lang::Object>& attachment)=0;
 virtual ::jxx::Ptr<SelectionKey> keyFor(const ::jxx::Ptr<::jxx::lang::Object>& channel)const=0;
 virtual ::jxx::lang::jbool validOps_(const ::jxx::Ptr<::jxx::lang::Object>& channel,::jxx::lang::jint operations)const=0;
 virtual void cancelKey(const ::jxx::Ptr<SelectionKey>& key)=0;
}; }
