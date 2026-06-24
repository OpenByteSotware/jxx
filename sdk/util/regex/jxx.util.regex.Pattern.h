#pragma once

#include <regex>
#include <string>

#include "lang/jxx.lang.buildin_array.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx {
namespace util {
namespace regex {

class Matcher;
class PatternSyntaxException;

class Pattern final : public jxx::lang::Object, public virtual jxx::io::Serializable {
public:
    static constexpr jxx::lang::jint UNIX_LINES = 0x01;
    static constexpr jxx::lang::jint CASE_INSENSITIVE = 0x02;
    static constexpr jxx::lang::jint COMMENTS = 0x04;
    static constexpr jxx::lang::jint MULTILINE = 0x08;
    static constexpr jxx::lang::jint LITERAL = 0x10;
    static constexpr jxx::lang::jint DOTALL = 0x20;
    static constexpr jxx::lang::jint UNICODE_CASE = 0x40;
    static constexpr jxx::lang::jint CANON_EQ = 0x80;
    static constexpr jxx::lang::jint UNICODE_CHARACTER_CLASS = 0x100;

private:
    jxx::Ptr<jxx::lang::String> regex_;
    jxx::lang::jint flags_;
    std::string compiledPatternUtf8_;
    std::regex compiled_;

    Pattern(
        jxx::Ptr<jxx::lang::String> regex,
        jxx::lang::jint flags,
        std::string compiledPatternUtf8,
        std::regex compiled);

public:
    virtual ~Pattern() = default;

    static jxx::Ptr<Pattern> compile(jxx::Ptr<jxx::lang::String> regex);
    static jxx::Ptr<Pattern> compile(jxx::Ptr<jxx::lang::String> regex, jxx::lang::jint flags);
    static jxx::lang::jbool matches(
        jxx::Ptr<jxx::lang::String> regex,
        jxx::Ptr<jxx::lang::CharSequence> input);

    jxx::Ptr<Matcher> matcher(jxx::Ptr<jxx::lang::CharSequence> input);
    jxx::Ptr<jxx::lang::String> pattern() const;
    jxx::lang::jint flags() const;

    jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> split(
        jxx::Ptr<jxx::lang::CharSequence> input);
    jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> split(
        jxx::Ptr<jxx::lang::CharSequence> input,
        jxx::lang::jint limit);

    static jxx::Ptr<jxx::lang::String> quote(jxx::Ptr<jxx::lang::String> s);
    virtual jxx::Ptr<jxx::lang::String> toString() const override;

    const std::regex& nativeRegex() const;
    const std::string& nativePatternUtf8() const;
};

} // namespace regex
} // namespace util
} // namespace jxx
