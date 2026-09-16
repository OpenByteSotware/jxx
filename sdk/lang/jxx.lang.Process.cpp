#include "lang/jxx.lang.Process.h"
#include <chrono>
#include <thread>
#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::lang {
jbool Process::waitFor(jlong timeout, const jxx::Ptr<jxx::util::concurrent::TimeUnit>& unit) {
    if (!unit) throw NullPointerException();
    const auto deadline=std::chrono::steady_clock::now()+unit->toChrono(timeout);
    while (isAlive() && std::chrono::steady_clock::now()<deadline) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return !isAlive();
}
jxx::Ptr<Process> Process::destroyForcibly(){destroy();return jxx::CAST<Process>(thisPtr());}
jbool Process::isAlive(){try{(void)exitValue();return false;}catch(const IllegalThreadStateException&){return true;}}
} // namespace jxx::lang
