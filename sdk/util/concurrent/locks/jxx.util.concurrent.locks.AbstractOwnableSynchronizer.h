#pragma once
#include <mutex>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Thread.h"
namespace jxx::util::concurrent::locks {
class AbstractOwnableSynchronizer : public ::jxx::lang::ClassBase<AbstractOwnableSynchronizer,::jxx::lang::Object,::jxx::io::SerializableI> {
public: using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<AbstractOwnableSynchronizer,JxxSuper,::jxx::io::SerializableI>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<AbstractOwnableSynchronizer,JxxSuper,::jxx::io::SerializableI>;static ::jxx::Ptr<::jxx::lang::ClassAny>Class();~AbstractOwnableSynchronizer()override=default;
protected: AbstractOwnableSynchronizer();void setExclusiveOwnerThread(const ::jxx::Ptr<::jxx::lang::Thread>&thread);::jxx::Ptr<::jxx::lang::Thread>getExclusiveOwnerThread()const;
public:void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&o)override{(void)o;}void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&i)override{(void)i;}void readObjectNoData()override{}
private:mutable std::mutex ownerMutex_;::jxx::Ptr<::jxx::lang::Thread> owner_;
};}
