#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/concurrent/jxx.util.concurrent.CopyOnWriteArrayList.h"
namespace jxx::util::concurrent {
template<typename E> class CopyOnWriteArraySet final
    : public ::jxx::lang::ClassBase<
          CopyOnWriteArraySet<E>,
          ::jxx::util::AbstractSet<E>,
          ::jxx::io::SerializableI> {
public:using JxxSuper=::jxx::util::AbstractSet<E>;using Super=::jxx::lang::ClassBase<CopyOnWriteArraySet<E>,JxxSuper,::jxx::io::SerializableI>;using JxxClassInfoMarker=typename Super::JxxClassInfoMarker;static ::jxx::Ptr<::jxx::lang::ClassAny>Class(){return JxxClassInfoMarker::Class();}CopyOnWriteArraySet():Super(),list_(::jxx::NEW<CopyOnWriteArrayList<E>>()){}::jxx::lang::jint size()override{return list_->size();}::jxx::lang::jbool isEmpty()override{return list_->isEmpty();}::jxx::lang::jbool contains(const ::jxx::Ptr<::jxx::lang::Object>&o)override{return list_->contains(o);}::jxx::Ptr<::jxx::util::Iterator<E>>iterator()override{return list_->iterator();}::jxx::lang::ObjectArray toArray()override{return list_->toArray();}::jxx::lang::jbool add(const ::jxx::Ptr<E>&e)override{return list_->addIfAbsent(e);}::jxx::lang::jbool remove(const ::jxx::Ptr<::jxx::lang::Object>&o)override{return list_->remove(o);}void clear()override{list_->clear();}
void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&out)override{if(!out)throw ::jxx::lang::NullPointerException();out->writeInt(size());auto it=iterator();while(it->hasNext())out->writeObject(::jxx::CAST<::jxx::lang::Object>(it->next()));}
void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&in)override{if(!in)throw ::jxx::lang::NullPointerException();auto restored=::jxx::NEW<CopyOnWriteArrayList<E>>();const auto n=in->readInt();if(n<0)throw ::jxx::io::InvalidObjectException(::jxx::NEW<::jxx::lang::String>("Negative set size"));for(::jxx::lang::jint i=0;i<n;++i){auto value=::jxx::CAST<E>(in->readObject());restored->addIfAbsent(value);}list_=restored;}
void readObjectNoData()override{list_=::jxx::NEW<CopyOnWriteArrayList<E>>();}
private: ::jxx::Ptr<CopyOnWriteArrayList<E>>list_;};}
