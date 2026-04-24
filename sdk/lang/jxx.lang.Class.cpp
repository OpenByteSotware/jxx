
#include "jxx.lang.String.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.ClassNotFoundException.h"
#include "jxx.lang.IllegalAccessException.h"
#include "jxx.lang.IllegalStateException.h"
#include "jxx.lang.Class.h"

namespace jxx::lang {
    // Java parity: throws ClassNotFoundException if not found
    ClassAny Class::forName(const char* canonicalName) {
        const TypeInfo* ti = TypeRegistry::instance().findByName(canonicalName);
        if (!ti) {
            throw ClassNotFoundException(String("Class not found: ") + canonicalName);
        }
        return ClassAny(ti);
    }
}