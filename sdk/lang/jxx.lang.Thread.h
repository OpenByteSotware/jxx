#pragma once

#include <memory>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class Thread final
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

    Thread();

    explicit Thread(
        const jxx::Ptr<Runnable>& target);

    explicit Thread(
        const jxx::Ptr<String>& name);

    Thread(
        const jxx::Ptr<Runnable>& target,
        const jxx::Ptr<String>& name);

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

    jxx::Ptr<String> getName() const;
    void setName(const jxx::Ptr<String>& name);

    jint getPriority() const;
    void setPriority(jint priority);

    jbool isDaemon() const;
    void setDaemon(jbool daemon);

    State getState() const;

    jxx::Ptr<String> toString() const override;

private:
    struct NativeState;

    std::unique_ptr<NativeState> state_;

    static void entry_(
        const jxx::Ptr<Thread>& self);

    static thread_local std::weak_ptr<Thread>
        currentThread_;
};

} // namespace jxx::lang
