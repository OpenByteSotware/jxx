#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"

namespace jxx::io { class File; class InputStream; class OutputStream; }
namespace jxx::lang {
class Process;
class String;
class Thread;

class Runtime final : public ClassBase<Runtime, Object> {
private:
    struct ConstructionToken {};

public:
    using JxxSuper = Object;
    using Super = ClassBase<Runtime, JxxSuper>;

    static jxx::Ptr<Runtime> getRuntime();

    void addShutdownHook(const jxx::Ptr<Thread>& hook);
    jbool removeShutdownHook(const jxx::Ptr<Thread>& hook);
    void exit(jint status);
    void halt(jint status);

    jint availableProcessors() const;
    jlong freeMemory() const;
    jlong totalMemory() const;
    jlong maxMemory() const;
    void gc();
    void runFinalization();
    void traceInstructions(jbool enable);
    void traceMethodCalls(jbool enable);

    void load(const jxx::Ptr<String>& filename);
    void loadLibrary(const jxx::Ptr<String>& libraryName);

    jxx::Ptr<jxx::io::InputStream> getLocalizedInputStream(
        const jxx::Ptr<jxx::io::InputStream>& input);
    jxx::Ptr<jxx::io::OutputStream> getLocalizedOutputStream(
        const jxx::Ptr<jxx::io::OutputStream>& output);

    jxx::Ptr<Process> exec(const jxx::Ptr<String>& command);
    jxx::Ptr<Process> exec(
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& command);
    jxx::Ptr<Process> exec(
        const jxx::Ptr<String>& command,
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& environment);
    jxx::Ptr<Process> exec(
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& command,
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& environment);
    jxx::Ptr<Process> exec(
        const jxx::Ptr<String>& command,
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& environment,
        const jxx::Ptr<jxx::io::File>& directory);
    jxx::Ptr<Process> exec(
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& command,
        const jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>>& environment,
        const jxx::Ptr<jxx::io::File>& directory);

    explicit Runtime(ConstructionToken);

private:
    void runShutdownHooks_();

    mutable std::mutex mutex_;
    std::condition_variable shutdownCondition_;
    std::vector<jxx::Ptr<Thread>> shutdownHooks_;
    jbool shuttingDown_ = false;
    jbool shutdownComplete_ = false;
};
} // namespace jxx::lang
