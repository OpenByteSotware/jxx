#pragma once

#include <functional>
#include <memory>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::util::concurrent::locks {
class LockSupport;
}

namespace jxx::lang {

class ThreadGroup;
class Throwable;

class Thread
    : public jxx::lang::ClassBase<
          Thread,
          Object,
          Runnable> {
public:
    using JxxSuper = Object;
    using Super =
        jxx::lang::ClassBase<
            Thread,
            JxxSuper,
            Runnable>;

    static constexpr jint MIN_PRIORITY = 1;
    static constexpr jint NORM_PRIORITY = 5;
    static constexpr jint MAX_PRIORITY = 10;

    enum class State : jint {
        NEW,
        RUNNABLE,
        BLOCKED,
        WAITING,
        TIMED_WAITING,
        TERMINATED
    };

    class UncaughtExceptionHandler
        : public jxx::lang::InterfaceBase<UncaughtExceptionHandler> {
    public:
        ~UncaughtExceptionHandler() override = default;

        virtual void uncaughtException(
            const jxx::Ptr<Thread>& thread,
            const jxx::Ptr<Throwable>& throwable) = 0;
    };

    Thread();

    explicit Thread(
        const jxx::Ptr<Runnable>& target);

    explicit Thread(
        const jxx::Ptr<String>& name);

    Thread(
        const jxx::Ptr<Runnable>& target,
        const jxx::Ptr<String>& name);

    Thread(const jxx::Ptr<ThreadGroup>& group,
           const jxx::Ptr<Runnable>& target);
    Thread(const jxx::Ptr<ThreadGroup>& group,
           const jxx::Ptr<String>& name);
    Thread(const jxx::Ptr<ThreadGroup>& group,
           const jxx::Ptr<Runnable>& target,
           const jxx::Ptr<String>& name);
    Thread(const jxx::Ptr<ThreadGroup>& group,
           const jxx::Ptr<Runnable>& target,
           const jxx::Ptr<String>& name,
           jlong stackSize);

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread(Thread&&) = delete;
    Thread& operator=(Thread&&) = delete;

    ~Thread() override;

    void start();
    void run() override;

    void interrupt();
    jbool isInterrupted() const;
    static jbool interrupted();

    jbool isAlive() const;

    void join();
    void join(jlong millis);
    void join(jlong millis, jint nanos);

    static void sleep(jlong millis);
    static void sleep(jlong millis, jint nanos);
    static void yield();

    static jxx::Ptr<Thread> currentThread();

    jlong getId() const;
    jxx::Ptr<ThreadGroup> getThreadGroup() const;

    jxx::Ptr<String> getName() const;
    void setName(const jxx::Ptr<String>& name);

    jint getPriority() const;
    void setPriority(jint priority);

    jbool isDaemon() const;
    void setDaemon(jbool daemon);

    State getState() const;

    static void setDefaultUncaughtExceptionHandler(
        const jxx::Ptr<UncaughtExceptionHandler>& handler);
    static jxx::Ptr<UncaughtExceptionHandler>
        getDefaultUncaughtExceptionHandler();

    void setUncaughtExceptionHandler(
        const jxx::Ptr<UncaughtExceptionHandler>& handler);
    jxx::Ptr<UncaughtExceptionHandler>
        getUncaughtExceptionHandler() const;

    jxx::Ptr<String> toString() const override;

public:
    /* Internal portable parking integration. */
    void setParkWakeup_(const std::function<void()>& wakeup);
    void clearParkWakeup_();

private:
    struct NativeState;

    std::unique_ptr<NativeState> state_;

    static void entry_(
        const jxx::Ptr<Thread>& self);

    void dispatchUncaughtException_(
        const jxx::Ptr<Throwable>& throwable);

    static thread_local std::weak_ptr<Thread>
        currentThread_;
};

} // namespace jxx::lang
