#include "jxx.lang.String.h"
#include "jxx.lang.Boolean.h"

namespace jxx::lang {

    // ---- Static initialization ----
    jxx::Ptr<Boolean> Boolean::TRUE = jxx::Ptr<Boolean>(jxx::NEW<Boolean>(true));
    jxx::Ptr<Boolean> Boolean::FALSE = jxx::Ptr<Boolean>(jxx::NEW<Boolean>(false));

    // ---- Constructor ----
    Boolean::Boolean(jbool v) : value(v) {}

    // ---- valueOf ----
    jxx::Ptr<Boolean> Boolean::valueOf(jbool b) {
        return b ? TRUE : FALSE;
    }

    jxx::Ptr<Boolean> Boolean::valueOf(const jxx::Ptr<String> s) {
        return parseBoolean(s) ? TRUE : FALSE;
    }

    // ---- parseBoolean ----
    jbool Boolean::parseBoolean(const jxx::Ptr<String> s) {
        if (!s) return false;

        // assuming String has equalsIgnoreCase
        return s->equalsIgnoreCase(String::valueOf("true"));
    }

    // ---- value access ----
    jbool Boolean::booleanValue() const {
        return value;
    }

    // ---- toString ----
    jxx::Ptr<String> Boolean::toString() const {
        return value ? String::valueOf("true")
            : String::valueOf("false");
    }

    jxx::Ptr<String> Boolean::toString(bool b) {
        return b ? String::valueOf("true")
            : String::valueOf("false");
    }

    // ---- hashCode ----
    // Matches Java: 1231 for true, 1237 for false
    int Boolean::hashCode() const {
        return hashCode(value);
    }

    int Boolean::hashCode(jbool value) {
        return value ? 1231 : 1237;
    }

    // ---- equals ----
    jxx::lang::jbool Boolean::equals(const jxx::Ptr<jxx::lang::Object> obj) const {
        if (this == obj.get()) return true;
        if (!obj) return false;

        auto other = jxx::CAST<Boolean, jxx::lang::Object>(obj);
        if (!other) return false;

        return value == other->value;
    }

    // ---- compareTo ----
    jint Boolean::compareTo(const jxx::Ptr<Boolean> b) const {
        if (!b) return 1;
        return compare(this->value, b->value);
    }

    // ---- compare ----
    jint Boolean::compare(jbool x, jbool y) {
        if (x == y) return 0;
        return x ? 1 : -1;
    }

}