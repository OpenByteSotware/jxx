#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.Map.h"
namespace jxx::util::concurrent {
template<typename K,typename V>
class ConcurrentMap : public ::jxx::lang::InterfaceBase<ConcurrentMap<K,V>,::jxx::util::Map<K,V>> {
public:
 using JxxSuper=::jxx::util::Map<K,V>;using Super=::jxx::lang::InterfaceBase<ConcurrentMap<K,V>,JxxSuper>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;
 ~ConcurrentMap() override=default;
 virtual ::jxx::Ptr<V> putIfAbsent(const ::jxx::Ptr<K>& key,const ::jxx::Ptr<V>& value) override=0;
 virtual ::jxx::lang::jbool remove(const ::jxx::Ptr<::jxx::lang::Object>& key,const ::jxx::Ptr<::jxx::lang::Object>& value) override=0;
 virtual ::jxx::lang::jbool replace(const ::jxx::Ptr<K>& key,const ::jxx::Ptr<V>& oldValue,const ::jxx::Ptr<V>& newValue) override=0;
 virtual ::jxx::Ptr<V> replace(const ::jxx::Ptr<K>& key,const ::jxx::Ptr<V>& value) override=0;
};}
