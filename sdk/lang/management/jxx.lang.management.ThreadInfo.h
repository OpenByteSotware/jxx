#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.StackTraceElement.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/management/jxx.lang.management.LockInfo.h"
#include "lang/management/jxx.lang.management.MonitorInfo.h"

namespace jxx::lang::management {

class ThreadInfo final
    : public ::jxx::lang::ClassBase<ThreadInfo, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ThreadInfo, JxxSuper>;

    ::jxx::lang::jlong getThreadId() const;
    ::jxx::Ptr<::jxx::lang::String> getThreadName() const;
    ::jxx::lang::Thread::State getThreadState() const;
    ::jxx::lang::jlong getBlockedTime() const;
    ::jxx::lang::jlong getBlockedCount() const;
    ::jxx::lang::jlong getWaitedTime() const;
    ::jxx::lang::jlong getWaitedCount() const;
    ::jxx::Ptr<LockInfo> getLockInfo() const;
    ::jxx::Ptr<::jxx::lang::String> getLockName() const;
    ::jxx::lang::jlong getLockOwnerId() const;
    ::jxx::Ptr<::jxx::lang::String> getLockOwnerName() const;
    ::jxx::Ptr<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::StackTraceElement>, 1U>> getStackTrace() const;
    ::jxx::lang::jbool isSuspended() const;
    ::jxx::lang::jbool isInNative() const;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
    static ::jxx::Ptr<ThreadInfo> from(
        const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data);
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<MonitorInfo>, 1U>>
        getLockedMonitors() const;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<LockInfo>, 1U>>
        getLockedSynchronizers() const;

private:
    ThreadInfo();
    ::jxx::lang::jlong threadId_ = 0;
    ::jxx::Ptr<::jxx::lang::String> threadName_;
    ::jxx::lang::Thread::State threadState_ =
        ::jxx::lang::Thread::State::TERMINATED;
    ::jxx::lang::jlong blockedTime_ = -1;
    ::jxx::lang::jlong blockedCount_ = 0;
    ::jxx::lang::jlong waitedTime_ = -1;
    ::jxx::lang::jlong waitedCount_ = 0;
    ::jxx::Ptr<LockInfo> lockInfo_;
    ::jxx::Ptr<::jxx::lang::String> lockName_;
    ::jxx::lang::jlong lockOwnerId_ = -1;
    ::jxx::Ptr<::jxx::lang::String> lockOwnerName_;
    ::jxx::Ptr<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::StackTraceElement>, 1U>> stackTrace_;
    ::jxx::lang::jbool suspended_ = false;
    ::jxx::lang::jbool inNative_ = false;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<MonitorInfo>, 1U>> lockedMonitors_;
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<LockInfo>, 1U>> lockedSynchronizers_;
};

} // namespace jxx::lang::management
