#include "lang/jxx.lang.ClassInfo.h"
#include "util/regex/jxx.util.regex.PatternSyntaxException.h"

namespace jxx::util::regex {

::jxx::Ptr<::jxx::lang::ClassAny>
PatternSyntaxException::Class() {
    return JxxClassInfoMarker::Class();
}

} // namespace jxx::util::regex
