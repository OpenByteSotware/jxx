#include "lang/management/jxx.lang.management.ThreadInfo.h"

#include <sstream>

#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace jxx::lang::management {
ThreadInfo::ThreadInfo()
    : threadName_(::jxx::NEW<::jxx::lang::String>("")),
      stackTrace_(::jxx::NEW<::jxx::lang::JxxArray<
          ::jxx::Ptr<::jxx::lang::StackTraceElement>, 1U>>(0)),
      lockedMonitors_(::jxx::NEW<::jxx::lang::JxxArray<
          ::jxx::Ptr<MonitorInfo>, 1U>>(0)),
      lockedSynchronizers_(::jxx::NEW<::jxx::lang::JxxArray<
          ::jxx::Ptr<LockInfo>, 1U>>(0)) {}
::jxx::lang::jlong ThreadInfo::getThreadId() const { return threadId_; }
::jxx::Ptr<::jxx::lang::String> ThreadInfo::getThreadName() const { return threadName_; }
::jxx::lang::Thread::State ThreadInfo::getThreadState() const { return threadState_; }
::jxx::lang::jlong ThreadInfo::getBlockedTime() const { return blockedTime_; }
::jxx::lang::jlong ThreadInfo::getBlockedCount() const { return blockedCount_; }
::jxx::lang::jlong ThreadInfo::getWaitedTime() const { return waitedTime_; }
::jxx::lang::jlong ThreadInfo::getWaitedCount() const { return waitedCount_; }
::jxx::Ptr<LockInfo> ThreadInfo::getLockInfo() const { return lockInfo_; }
::jxx::Ptr<::jxx::lang::String> ThreadInfo::getLockName() const { return lockName_; }
::jxx::lang::jlong ThreadInfo::getLockOwnerId() const { return lockOwnerId_; }
::jxx::Ptr<::jxx::lang::String> ThreadInfo::getLockOwnerName() const {
    return lockOwnerName_;
}
::jxx::Ptr<::jxx::lang::JxxArray<
    ::jxx::Ptr<::jxx::lang::StackTraceElement>, 1U>>
ThreadInfo::getStackTrace() const { return stackTrace_; }
::jxx::lang::jbool ThreadInfo::isSuspended() const { return suspended_; }
::jxx::lang::jbool ThreadInfo::isInNative() const { return inNative_; }
::jxx::Ptr<::jxx::lang::String> ThreadInfo::toString() const {
    std::ostringstream out;
    out << '"' << (threadName_ == nullptr ? "" : threadName_->utf8())
        << '"' << " Id=" << threadId_;
    return ::jxx::NEW<::jxx::lang::String>(out.str());
}
::jxx::Ptr<ThreadInfo> ThreadInfo::from(
    const ::jxx::Ptr<::jxx::ext::management::openmbean::CompositeData>& data) {
    if (data == nullptr) return nullptr;
    throw ::jxx::lang::UnsupportedOperationException(
        "CompositeData requires javax.management.openmbean support");
}
::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<MonitorInfo>, 1U>>
ThreadInfo::getLockedMonitors() const { return lockedMonitors_; }
::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<LockInfo>, 1U>>
ThreadInfo::getLockedSynchronizers() const { return lockedSynchronizers_; }
} // namespace jxx::lang::management
