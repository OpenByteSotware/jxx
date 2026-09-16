#include "util/jxx.util.MissingResourceException.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util {

jxx::Ptr<jxx::lang::ClassAny> MissingResourceException::Class() {
    return JxxClassInfoMarker::Class();
}

MissingResourceException::MissingResourceException(
    const jxx::Ptr<jxx::lang::String>& message,
    const jxx::Ptr<jxx::lang::String>& className,
    const jxx::Ptr<jxx::lang::String>& key)
    : JxxSuper(message)
    , className_(className)
    , key_(key) {
}

jxx::Ptr<jxx::lang::String>
MissingResourceException::getClassName() const {
    return className_;
}

jxx::Ptr<jxx::lang::String> MissingResourceException::getKey() const {
    return key_;
}

const char* MissingResourceException::typeName() const noexcept {
    return "MissingResourceException";
}

} // namespace jxx::util
