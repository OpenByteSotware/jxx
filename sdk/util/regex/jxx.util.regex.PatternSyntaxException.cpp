#include <sstream>
#include "lang/jxx.lang.ClassInfo.h"

#include "util/regex/jxx.util.regex.PatternSyntaxException.h"

namespace jxx {
namespace util {
namespace regex {

} // namespace regex
} // namespace util
} // namespace jxx

jxx::Ptr<jxx::lang::ClassAny> jxx::util::regex::PatternSyntaxException::Class()
{
    return JxxClassInfoMarker::Class();
}
