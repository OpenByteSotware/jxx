#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/concurrent/jxx.util.concurrent.CopyOnWriteArrayList.h"
namespace jxx::util::concurrent {
template<typename E> class CopyOnWriteArraySet final:public ::jxx::lang::ClassBase<CopyOnWriteArraySet<E>,::jxx::util::AbstractSet<E>>{
public:using JxxSuper=::jxx::util::AbstractSet<E>;using Super=::jxx::lang::ClassBase<CopyOnWriteArraySet<E>,JxxSuper>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}CopyOnWriteArraySet():Super(),list_(::jxx::NEW<CopyOnWriteArrayList<E>>()){}::jxx::lang::jint size()override{return list_->size();}::jxx::lang::jbool isEmpty()override{return list_->isEmpty();}::jxx::lang::jbool contains(const ::jxx::Ptr<::jxx::lang::Object>&o)override{return list_->contains(o);}::jxx::Ptr<::jxx::util::Iterator<E>>iterator()override{return list_->iterator();}::jxx::lang::ObjectArray toArray()override{return list_->toArray();}::jxx::lang::jbool add(const ::jxx::Ptr<E>&e)override{return list_->addIfAbsent(e);}::jxx::lang::jbool remove(const ::jxx::Ptr<::jxx::lang::Object>&o)override{return list_->remove(o);}void clear()override{list_->clear();}
private: ::jxx::Ptr<CopyOnWriteArrayList<E>>list_;};}
