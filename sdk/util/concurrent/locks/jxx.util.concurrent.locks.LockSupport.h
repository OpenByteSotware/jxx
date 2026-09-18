#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx_types.h"
namespace jxx::util::concurrent::locks {
class LockSupport final : public ::jxx::lang::ClassBase<LockSupport,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<LockSupport,JxxSuper>; using JxxClassInfoMarker=::jxx::lang::ClassInfo<LockSupport,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 static void unpark(const ::jxx::Ptr<::jxx::lang::Thread>& thread);
 static void park(); static void park(const ::jxx::Ptr<::jxx::lang::Object>& blocker);
 static void parkNanos(::jxx::lang::jlong nanos); static void parkNanos(const ::jxx::Ptr<::jxx::lang::Object>& blocker,::jxx::lang::jlong nanos);
 static void parkUntil(::jxx::lang::jlong deadline); static void parkUntil(const ::jxx::Ptr<::jxx::lang::Object>& blocker,::jxx::lang::jlong deadline);
 static ::jxx::Ptr<::jxx::lang::Object> getBlocker(const ::jxx::Ptr<::jxx::lang::Thread>& thread);
private: LockSupport()=delete;
}; }
