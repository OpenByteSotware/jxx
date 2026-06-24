#include <regex>
#include <string>

#include "io/jxx.util.regex.Matcher.h"
#include "io/jxx.util.regex.Pattern.h"
#include "io/jxx.lang.IllegalArgumentException.h"
#include "io/jxx.lang.IllegalStateException.h"
#include "io/jxx.lang.IndexOutOfBoundsException.h"
#include "io/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {
namespace regex {
namespace {

static jxx::Ptr<jxx::lang::String> toStringPtr(jxx::Ptr<jxx::lang::CharSequence> seq) {
    if (seq == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    auto asString = jxx::CAST<jxx::lang::String>(seq);
    return asString != nullptr ? asString : seq->toString();
}

static std::string escapeReplacement(const std::string& s) {
    std::string out;
    out.reserve(s.size() * 2U);
    for (char c : s) {
        if (c == '$' || c == '\\') out.push_back('\\');
        out.push_back(c);
    }
    return out;
}

} // anonymous namespace

Matcher::Matcher(jxx::Ptr<Pattern> pattern, jxx::Ptr<jxx::lang::CharSequence> input)
    : pattern_(pattern)
    , input_(toStringPtr(input))
    , inputUtf8_(input_->utf8())
    , lastMatch_()
    , searchPos_(0)
    , hasMatch_(static_cast<jxx::lang::jbool>(false)) {
    if (pattern_ == nullptr) {
        throw jxx::lang::NullPointerException();
    }
}

void Matcher::ensureMatchState() const {
    if (!hasMatch_) {
        throw jxx::lang::IllegalStateException();
    }
}

jxx::Ptr<Pattern> Matcher::pattern() {
    return pattern_;
}

jxx::Ptr<Matcher> Matcher::reset() {
    searchPos_ = 0;
    hasMatch_ = static_cast<jxx::lang::jbool>(false);
    lastMatch_ = std::match_results<std::string::const_iterator>();
    return jxx::CAST<Matcher>(jxx::CAST<jxx::lang::Object>(shared_from_this()));
}

jxx::Ptr<Matcher> Matcher::reset(jxx::Ptr<jxx::lang::CharSequence> input) {
    input_ = toStringPtr(input);
    inputUtf8_ = input_->utf8();
    return reset();
}

jxx::lang::jbool Matcher::matches() {
    hasMatch_ = static_cast<jxx::lang::jbool>(std::regex_match(inputUtf8_, lastMatch_, pattern_->nativeRegex()));
    if (hasMatch_) {
        searchPos_ = static_cast<std::size_t>(lastMatch_.position() + lastMatch_.length());
    }
    return hasMatch_;
}

jxx::lang::jbool Matcher::lookingAt() {
    hasMatch_ = static_cast<jxx::lang::jbool>(std::regex_search(
        inputUtf8_.cbegin(),
        inputUtf8_.cend(),
        lastMatch_,
        pattern_->nativeRegex(),
        std::regex_constants::match_continuous));
    if (hasMatch_) {
        searchPos_ = static_cast<std::size_t>(lastMatch_.position() + lastMatch_.length());
    }
    return hasMatch_;
}

jxx::lang::jbool Matcher::find() {
    if (searchPos_ > inputUtf8_.size()) {
        hasMatch_ = static_cast<jxx::lang::jbool>(false);
        return static_cast<jxx::lang::jbool>(false);
    }
    const auto begin = inputUtf8_.cbegin() + static_cast<std::ptrdiff_t>(searchPos_);
    hasMatch_ = static_cast<jxx::lang::jbool>(std::regex_search(begin, inputUtf8_.cend(), lastMatch_, pattern_->nativeRegex()));
    if (hasMatch_) {
        searchPos_ += static_cast<std::size_t>(lastMatch_.position() + lastMatch_.length());
    }
    return hasMatch_;
}

jxx::lang::jbool Matcher::find(jxx::lang::jint start) {
    if (start < 0 || static_cast<std::size_t>(start) > inputUtf8_.size()) {
        throw jxx::lang::IndexOutOfBoundsException();
    }
    searchPos_ = static_cast<std::size_t>(start);
    return find();
}

jxx::lang::jint Matcher::start() {
    return start(static_cast<jxx::lang::jint>(0));
}

jxx::lang::jint Matcher::start(jxx::lang::jint group) {
    ensureMatchState();
    if (group < 0 || static_cast<std::size_t>(group) >= lastMatch_.size()) {
        throw jxx::lang::IndexOutOfBoundsException();
    }
    if (!lastMatch_[static_cast<std::size_t>(group)].matched) {
        return static_cast<jxx::lang::jint>(-1);
    }
    const auto pos = static_cast<jxx::lang::jint>(lastMatch_.position(static_cast<std::size_t>(group)));
    const auto absoluteBase = static_cast<jxx::lang::jint>(searchPos_ - static_cast<std::size_t>(lastMatch_.position() + lastMatch_.length()));
    return static_cast<jxx::lang::jint>(absoluteBase + pos);
}

jxx::lang::jint Matcher::end() {
    return end(static_cast<jxx::lang::jint>(0));
}

jxx::lang::jint Matcher::end(jxx::lang::jint group) {
    ensureMatchState();
    if (group < 0 || static_cast<std::size_t>(group) >= lastMatch_.size()) {
        throw jxx::lang::IndexOutOfBoundsException();
    }
    if (!lastMatch_[static_cast<std::size_t>(group)].matched) {
        return static_cast<jxx::lang::jint>(-1);
    }
    return static_cast<jxx::lang::jint>(start(group) + lastMatch_[static_cast<std::size_t>(group)].length());
}

jxx::Ptr<jxx::lang::String> Matcher::group() {
    return group(static_cast<jxx::lang::jint>(0));
}

jxx::Ptr<jxx::lang::String> Matcher::group(jxx::lang::jint group) {
    ensureMatchState();
    if (group < 0 || static_cast<std::size_t>(group) >= lastMatch_.size()) {
        throw jxx::lang::IndexOutOfBoundsException();
    }
    if (!lastMatch_[static_cast<std::size_t>(group)].matched) {
        return nullptr;
    }
    return std::make_shared<jxx::lang::String>(lastMatch_[static_cast<std::size_t>(group)].str());
}

jxx::lang::jint Matcher::groupCount() {
    return static_cast<jxx::lang::jint>(pattern_->nativeRegex().mark_count());
}

jxx::Ptr<jxx::lang::String> Matcher::replaceAll(jxx::Ptr<jxx::lang::String> replacement) {
    if (replacement == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return std::make_shared<jxx::lang::String>(
        std::regex_replace(inputUtf8_, pattern_->nativeRegex(), replacement->utf8()));
}

jxx::Ptr<jxx::lang::String> Matcher::replaceFirst(jxx::Ptr<jxx::lang::String> replacement) {
    if (replacement == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return std::make_shared<jxx::lang::String>(
        std::regex_replace(
            inputUtf8_,
            pattern_->nativeRegex(),
            replacement->utf8(),
            std::regex_constants::format_first_only));
}

jxx::Ptr<jxx::lang::String> Matcher::quoteReplacement(jxx::Ptr<jxx::lang::String> s) {
    if (s == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return std::make_shared<jxx::lang::String>(escapeReplacement(s->utf8()));
}

} // namespace regex
} // namespace util
} // namespace jxx
