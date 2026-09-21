#include "lang/jxx.lang.Compiler.h"

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {

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
}

void Compiler::disable() noexcept {
}

} // namespace jxx::lang
