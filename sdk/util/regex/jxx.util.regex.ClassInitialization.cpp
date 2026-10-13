#include "util/regex/jxx.util.regex.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "util/regex/jxx.util.regex.MatchResult.h"
#include "util/regex/jxx.util.regex.Matcher.h"
#include "util/regex/jxx.util.regex.Pattern.h"
#include "util/regex/jxx.util.regex.PatternSyntaxException.h"

namespace jxx::util::regex {
namespace {

template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<MatchResult>();
        registerType<Matcher>();
        registerType<Pattern>();
        registerType<PatternSyntaxException>();
    });
}

} // namespace jxx::util::regex
