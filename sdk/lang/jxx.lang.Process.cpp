#include <algorithm>
#include <chrono>
#include <thread>
#include "lang/jxx.lang.Process.h"
#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::lang {
jbool Process::waitFor(
    jlong timeout,
    const jxx::Ptr<jxx::util::concurrent::TimeUnit>& unit) {
    if (unit == nullptr) throw NullPointerException();
    if (!isAlive()) return true;
    if (timeout <= 0) return false;

    const auto duration = unit->toChrono(timeout);
    const auto start = std::chrono::steady_clock::now();
    for (;;) {
        if (!isAlive()) return true;
        const auto elapsed = std::chrono::steady_clock::now() - start;
        if (elapsed >= duration) return !isAlive();
        const auto remaining = duration - elapsed;
        std::this_thread::sleep_for(
            std::min(remaining, std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(1))));
    }
}
jxx::Ptr<Process> Process::destroyForcibly(){destroy();return jxx::CAST<Process>(thisPtr());}
jbool Process::isAlive(){try{(void)exitValue();return false;}catch(const IllegalThreadStateException&){return true;}}
} // namespace jxx::lang
