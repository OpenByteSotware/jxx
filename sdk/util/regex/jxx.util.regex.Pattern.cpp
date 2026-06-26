#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "util/regex/jxx.util.regex.Matcher.h"
#include "util/regex/jxx.util.regex.Pattern.h"
#include "util/regex/jxx.util.regex.PatternSyntaxException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {
namespace regex {
namespace {

static std::string escapeRegexLiteral(const std::string& s) {
    std::string out;
    out.reserve(s.size() * 2U);
    for (char c : s) {
        switch (c) {
            case '\\': case '.': case '^': case '$': case '|': case '(': case ')':
            case '[': case ']': case '{': case '}': case '*': case '+': case '?':
                out.push_back('\\');
                out.push_back(c);
                break;
            default:
                out.push_back(c);
                break;
        }
    }
    return out;
}

static std::regex_constants::syntax_option_type toSyntaxFlags(jxx::lang::jint flags) {
    auto syntax = std::regex_constants::ECMAScript;
    if ((flags & Pattern::CASE_INSENSITIVE) != 0) {
        syntax |= std::regex_constants::icase;
    }
    return syntax;
}

static std::string compilePatternUtf8(jxx::Ptr<jxx::lang::String> regex, jxx::lang::jint flags) {
    if (regex == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    std::string patternUtf8 = regex->utf8();
    if ((flags & Pattern::LITERAL) != 0) {
        patternUtf8 = escapeRegexLiteral(patternUtf8);
    }
    return patternUtf8;
}

static jxx::Ptr<jxx::lang::String> toStringPtr(jxx::Ptr<jxx::lang::CharSequence> seq) {
    if (seq == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    auto asString = jxx::CAST<jxx::lang::String>(seq);
    return asString;
}

static jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> vectorToStringArray(const std::vector<jxx::Ptr<jxx::lang::String>>& items) {
    auto arr = jxx::NEW<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>>(static_cast<jxx::lang::jint>(items.size()));
    for (jxx::lang::jint i = 0; i < static_cast<jxx::lang::jint>(items.size()); ++i) {
        (*arr)(i) = items[static_cast<std::size_t>(i)];
    }
    return arr;
}

} // anonymous namespace

Pattern::Pattern(
    jxx::Ptr<jxx::lang::String> regex,
    jxx::lang::jint flags,
    std::string compiledPatternUtf8,
    std::regex compiled)
    : regex_(regex)
    , flags_(flags)
    , compiledPatternUtf8_(std::move(compiledPatternUtf8))
    , compiled_(std::move(compiled)) {
}

jxx::Ptr<Pattern> Pattern::compile(jxx::Ptr<jxx::lang::String> regex) {
    return compile(regex, static_cast<jxx::lang::jint>(0));
}

jxx::Ptr<Pattern> Pattern::compile(jxx::Ptr<jxx::lang::String> regex, jxx::lang::jint flags) {
    if (regex == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    try {
        std::string compiledPattern = compilePatternUtf8(regex, flags);
        std::regex native(compiledPattern, toSyntaxFlags(flags));
        return jxx::NEW<Pattern>(regex, flags, std::move(compiledPattern), std::move(native));
    } catch (const std::regex_error& ex) {
        throw jxx::util::regex::PatternSyntaxException(ex.what());            
    }
}

jxx::lang::jbool Pattern::matches(
    jxx::Ptr<jxx::lang::String> regex,
    jxx::Ptr<jxx::lang::CharSequence> input) {
    auto p = compile(regex);
    return p->matcher(input)->matches();
}

jxx::Ptr<Matcher> Pattern::matcher(jxx::Ptr<jxx::lang::CharSequence> input) {
    return jxx::NEW<Matcher>(jxx::CAST<Pattern>(jxx::CAST<jxx::lang::Object>(shared_from_this())), input);
}

jxx::Ptr<jxx::lang::String> Pattern::pattern() const {
    return regex_;
}

jxx::lang::jint Pattern::flags() const {
    return flags_;
}

jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> Pattern::split(
    jxx::Ptr<jxx::lang::CharSequence> input) {
    return split(input, static_cast<jxx::lang::jint>(0));
}

jxx::Ptr<jxx::JxxArray<jxx::Ptr<jxx::lang::String>, 1U>> Pattern::split(
    jxx::Ptr<jxx::lang::CharSequence> input,
    jxx::lang::jint limit) {
    auto src = toStringPtr(input);
    std::string in = src->utf8();
    std::sregex_token_iterator it(in.begin(), in.end(), compiled_, -1);
    std::sregex_token_iterator end;
    std::vector<jxx::Ptr<jxx::lang::String>> parts;
    for (; it != end; ++it) {
        if (limit > 0 && static_cast<jxx::lang::jint>(parts.size()) == limit - 1) {
            std::string rest = in.substr(static_cast<std::size_t>(it->first - in.begin()));
            parts.push_back(jxx::NEW<jxx::lang::String>(rest));
            return vectorToStringArray(parts);
        }
        parts.push_back(jxx::NEW<jxx::lang::String>(it->str()));
    }
    if (limit == 0) {
        while (!parts.empty() && parts.back() != nullptr && parts.back()->utf8().empty()) {
            parts.pop_back();
        }
    }
    return vectorToStringArray(parts);
}

jxx::Ptr<jxx::lang::String> Pattern::quote(jxx::Ptr<jxx::lang::String> s) {
    if (s == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return jxx::NEW<jxx::lang::String>(escapeRegexLiteral(s->utf8()));
}

jxx::Ptr<jxx::lang::String> Pattern::toString() const {
    return regex_;
}

const std::regex& Pattern::nativeRegex() const {
    return compiled_;
}

const std::string& Pattern::nativePatternUtf8() const {
    return compiledPatternUtf8_;
}

void Pattern::writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) {
    if (out == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    out->writeObject(regex_);
	out->writeInt(flags_);
}

void Pattern::readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) {
    if (in == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    regex_ = jxx::CAST<jxx::lang::String, jxx::lang::Object>(in->readObject());
    flags_ = in->readInt();
    compiledPatternUtf8_ = compilePatternUtf8(regex_, flags_);
    compiled_ = std::regex(compiledPatternUtf8_, toSyntaxFlags(flags_));
}
void Pattern::readObjectNoData() {
    throw jxx::lang::RuntimeException("No data available for deserialization");
}

} // namespace regex
} // namespace util
} // namespace jxx
