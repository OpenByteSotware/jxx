#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class ClassAny;
class String;

class Compiler final
    : public ClassBase<Compiler, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<Compiler, JxxSuper>;
    using JxxClassInfoMarker = ClassInfo<Compiler, JxxSuper>;

    static jxx::Ptr<ClassAny> Class();

    static jbool compileClass(
        const jxx::Ptr<ClassAny>& type);
    static jbool compileClasses(
        const jxx::Ptr<String>& namePattern);
    static jxx::Ptr<Object> command(
        const jxx::Ptr<Object>& argument);
    static void enable() noexcept;
    static void disable() noexcept;

private:
    Compiler() = delete;
};

} // namespace jxx::lang
