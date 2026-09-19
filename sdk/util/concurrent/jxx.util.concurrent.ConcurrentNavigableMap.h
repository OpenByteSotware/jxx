#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentMap.h"
#include "util/jxx.util.NavigableMap.h"
namespace jxx::util::concurrent {
template<typename K,typename V>
class ConcurrentNavigableMap : public ::jxx::lang::InterfaceBase<ConcurrentNavigableMap<K,V>,ConcurrentMap<K,V>,::jxx::util::NavigableMap<K,V>> {
public:
 using Super=::jxx::lang::InterfaceBase<ConcurrentNavigableMap<K,V>,ConcurrentMap<K,V>,::jxx::util::NavigableMap<K,V>>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 ~ConcurrentNavigableMap() override=default;
 virtual ::jxx::Ptr<ConcurrentNavigableMap<K,V>> subMap(const ::jxx::Ptr<K>& fromKey,::jxx::lang::jbool fromInclusive,const ::jxx::Ptr<K>& toKey,::jxx::lang::jbool toInclusive)=0;
 virtual ::jxx::Ptr<ConcurrentNavigableMap<K,V>> headMap(const ::jxx::Ptr<K>& toKey,::jxx::lang::jbool inclusive)=0;
 virtual ::jxx::Ptr<ConcurrentNavigableMap<K,V>> tailMap(const ::jxx::Ptr<K>& fromKey,::jxx::lang::jbool inclusive)=0;
 virtual ::jxx::Ptr<ConcurrentNavigableMap<K,V>> subMap(const ::jxx::Ptr<K>& fromKey,const ::jxx::Ptr<K>& toKey)=0;
 virtual ::jxx::Ptr<ConcurrentNavigableMap<K,V>> headMap(const ::jxx::Ptr<K>& toKey)=0;
 virtual ::jxx::Ptr<ConcurrentNavigableMap<K,V>> tailMap(const ::jxx::Ptr<K>& fromKey)=0;
 virtual ::jxx::Ptr<ConcurrentNavigableMap<K,V>> descendingMap()=0;
};}
