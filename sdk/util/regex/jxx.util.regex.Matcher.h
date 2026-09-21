#pragma once

#include <regex>
#include <string>
#include <vector>

#include "util/regex/jxx.util.regex.MatchResult.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.StringBuffer.h"


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
    std::size_t regionStart_;
    std::size_t regionEnd_;
    std::size_t appendPos_;
    jxx::lang::jbool hasMatch_;
    jxx::lang::jbool anchoringBounds_;
    jxx::lang::jbool transparentBounds_;
    jxx::lang::jbool hitEnd_;
    jxx::lang::jbool requireEnd_;
    std::size_t matchBase_;

    void ensureMatchState() const;

public:
    Matcher(const jxx::Ptr<Pattern> pattern, jxx::Ptr<jxx::lang::CharSequence> input);
    virtual ~Matcher() = default;

    jxx::Ptr<Pattern> pattern();
    jxx::Ptr<Matcher> reset();
    jxx::Ptr<Matcher> reset(const jxx::Ptr<jxx::lang::CharSequence> input);

    jxx::lang::jbool matches();
    jxx::lang::jbool lookingAt();
    jxx::lang::jbool find();
    jxx::lang::jbool find(jxx::lang::jint start);
    jxx::Ptr<Matcher> region(jxx::lang::jint start, jxx::lang::jint end);
    jxx::lang::jint regionStart() const;
    jxx::lang::jint regionEnd() const;
    jxx::Ptr<Matcher> useAnchoringBounds(jxx::lang::jbool value);
    jxx::lang::jbool hasAnchoringBounds() const;
    jxx::Ptr<Matcher> useTransparentBounds(jxx::lang::jbool value);
    jxx::lang::jbool hasTransparentBounds() const;
    jxx::lang::jbool hitEnd() const;
    jxx::lang::jbool requireEnd() const;

    virtual jxx::lang::jint start();
    virtual jxx::lang::jint start(jxx::lang::jint group);
    virtual jxx::lang::jint end();
    virtual jxx::lang::jint end(jxx::lang::jint group);
    virtual jxx::Ptr<jxx::lang::String> group();
    virtual jxx::Ptr<jxx::lang::String> group(jxx::lang::jint group);
    virtual jxx::lang::jint groupCount();

    jxx::Ptr<Matcher> appendReplacement(
        const jxx::Ptr<jxx::lang::StringBuffer>& buffer,
        const jxx::Ptr<jxx::lang::String>& replacement);
    jxx::Ptr<jxx::lang::StringBuffer> appendTail(
        const jxx::Ptr<jxx::lang::StringBuffer>& buffer);

    jxx::Ptr<jxx::lang::String> replaceAll(const jxx::Ptr<jxx::lang::String> replacement);
    jxx::Ptr<jxx::lang::String> replaceFirst(const jxx::Ptr<jxx::lang::String> replacement);
    static jxx::Ptr<jxx::lang::String> quoteReplacement(const jxx::Ptr<jxx::lang::String> s);
};

} // namespace regex
} // namespace util
} // namespace jxx
