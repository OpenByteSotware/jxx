#pragma once

#include <regex>
#include <string>
#include <vector>

#include "util/regex/jxx.util.regex.MatchResult.h"
#include "lang/jxx.lang.CharSequence.h"


namespace jxx {
namespace util {
namespace regex {

class Pattern;

class Matcher final : public jxx::lang::Object {
private:
    jxx::Ptr<Pattern> pattern_;
    jxx::Ptr<jxx::lang::String> input_;
    std::string inputUtf8_;
    std::match_results<std::string::const_iterator> lastMatch_;
    std::size_t searchPos_;
    jxx::lang::jbool hasMatch_;

    void ensureMatchState() const;

public:
    Matcher(jxx::Ptr<Pattern> pattern, jxx::Ptr<jxx::lang::CharSequence> input);
    virtual ~Matcher() = default;

    jxx::Ptr<Pattern> pattern();
    jxx::Ptr<Matcher> reset();
    jxx::Ptr<Matcher> reset(jxx::Ptr<jxx::lang::CharSequence> input);

    jxx::lang::jbool matches();
    jxx::lang::jbool lookingAt();
    jxx::lang::jbool find();
    jxx::lang::jbool find(jxx::lang::jint start);

    virtual jxx::lang::jint start();
    virtual jxx::lang::jint start(jxx::lang::jint group);
    virtual jxx::lang::jint end();
    virtual jxx::lang::jint end(jxx::lang::jint group);
    virtual jxx::Ptr<jxx::lang::String> group();
    virtual jxx::Ptr<jxx::lang::String> group(jxx::lang::jint group);
    virtual jxx::lang::jint groupCount();

    jxx::Ptr<jxx::lang::String> replaceAll(jxx::Ptr<jxx::lang::String> replacement);
    jxx::Ptr<jxx::lang::String> replaceFirst(jxx::Ptr<jxx::lang::String> replacement);
    static jxx::Ptr<jxx::lang::String> quoteReplacement(jxx::Ptr<jxx::lang::String> s);
};

} // namespace regex
} // namespace util
} // namespace jxx
