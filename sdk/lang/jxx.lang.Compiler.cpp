#include "lang/jxx.lang.Compiler.h"

#include <atomic>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
namespace {

std::atomic<jbool>& compilerEnabled() noexcept {
    static std::atomic<jbool> enabled{true};
    return enabled;
}

} // namespace

jxx::Ptr<ClassAny> Compiler::Class() {
    return JxxClassInfoMarker::Class();
}

jbool Compiler::compileClass(
    const jxx::Ptr<ClassAny>& type) {
    if (type == nullptr) {
        throw NullPointerException();
    }
    return false;
}

jbool Compiler::compileClasses(
    const jxx::Ptr<String>& namePattern) {
    if (namePattern == nullptr) {
        throw NullPointerException();
    }
    return false;
}

jxx::Ptr<Object> Compiler::command(
    const jxx::Ptr<Object>& argument) {
    if (argument == nullptr) {
        throw NullPointerException();
    }
    return nullptr;
}

void Compiler::enable() noexcept {
    compilerEnabled().store(true, std::memory_order_release);
}

void Compiler::disable() noexcept {
    compilerEnabled().store(false, std::memory_order_release);
}

} // namespace jxx::lang
