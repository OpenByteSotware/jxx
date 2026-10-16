#pragma once
#include <mutex>
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Thread.h"
namespace jxx::util::concurrent::locks {
class AbstractOwnableSynchronizer : public ::jxx::lang::ClassBase<AbstractOwnableSynchronizer,::jxx::lang::Object,::jxx::io::SerializableI> {
public: using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<AbstractOwnableSynchronizer,JxxSuper,::jxx::io::SerializableI>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<AbstractOwnableSynchronizer,JxxSuper,::jxx::io::SerializableI>;static ::jxx::Ptr<::jxx::lang::ClassAny>Class();~AbstractOwnableSynchronizer()override=default;
protected: AbstractOwnableSynchronizer();void setExclusiveOwnerThread(const ::jxx::Ptr<::jxx::lang::Thread>&thread);::jxx::Ptr<::jxx::lang::Thread>getExclusiveOwnerThread()const;
public:
 void writeObject(
     const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override {
     if (output == nullptr) throw ::jxx::lang::NullPointerException();
     // The exclusive owner is transient synchronization state.
 }
 void readObject(
     const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override {
     if (input == nullptr) throw ::jxx::lang::NullPointerException();
     clearExclusiveOwner_();
 }
 void readObjectNoData() override {
     clearExclusiveOwner_();
 }
private:
 void clearExclusiveOwner_() {
     std::lock_guard<std::mutex> lock(ownerMutex_);
     owner_.reset();
 }
mutable std::mutex ownerMutex_;::jxx::Ptr<::jxx::lang::Thread> owner_;
};}
