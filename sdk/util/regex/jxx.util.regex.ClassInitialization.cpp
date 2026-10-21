#include "util/regex/jxx.util.regex.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.String.h"
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
        ::jxx::io::serialization::registerSerializableClass<Pattern>(
            1,
            [] {
                return Pattern::compile(
                    ::jxx::NEW<::jxx::lang::String>(""));
            });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            Pattern, 1U, 2U>();
        registerType<PatternSyntaxException>();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::util::regex
