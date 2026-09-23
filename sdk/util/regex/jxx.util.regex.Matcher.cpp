#include <regex>
#include <string>

#include "util/regex/jxx.util.regex.Matcher.h"
#include "util/regex/jxx.util.regex.Pattern.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {
namespace regex {
namespace {

static jxx::Ptr<jxx::lang::String> toStringPtr(const jxx::Ptr<jxx::lang::CharSequence> seq) {
    if (seq == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    auto asString = jxx::CAST<jxx::lang::String>(seq);
    return asString;
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

Matcher::Matcher(const jxx::Ptr<Pattern> pattern, jxx::Ptr<jxx::lang::CharSequence> input)
    : pattern_(pattern)
    , input_(toStringPtr(input))
    , inputUtf8_(input_->utf8())
    , lastMatch_()
    , searchPos_(0)
    , regionStart_(0)
    , regionEnd_(inputUtf8_.size())
    , appendPos_(0)
    , hasMatch_(static_cast<jxx::lang::jbool>(false))
    , anchoringBounds_(true)
    , transparentBounds_(false)
    , hitEnd_(false)
    , requireEnd_(false)
    , matchBase_(0) {
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
    searchPos_ = regionStart_;
    appendPos_ = regionStart_;
    hitEnd_ = false;
    requireEnd_ = false;
    matchBase_ = regionStart_;
    hasMatch_ = static_cast<jxx::lang::jbool>(false);
    lastMatch_ = std::match_results<std::string::const_iterator>();
    return jxx::CAST<Matcher>(jxx::CAST<jxx::lang::Object>(shared_from_this()));
}

jxx::Ptr<Matcher> Matcher::reset(const jxx::Ptr<jxx::lang::CharSequence> input) {
    input_ = toStringPtr(input);
    inputUtf8_ = input_->utf8();
    regionStart_ = 0;
    regionEnd_ = inputUtf8_.size();
    return reset();
}

jxx::lang::jbool Matcher::matches() {
    const auto first = inputUtf8_.cbegin() + static_cast<std::ptrdiff_t>(regionStart_);
    const auto last = inputUtf8_.cbegin() + static_cast<std::ptrdiff_t>(regionEnd_);
    hasMatch_ = static_cast<jxx::lang::jbool>(
        std::regex_match(first, last, lastMatch_, pattern_->nativeRegex()));
    matchBase_ = regionStart_;
    if (hasMatch_) {
        searchPos_ = regionEnd_;
    }
    hitEnd_ = true;
    requireEnd_ = false;
    return hasMatch_;
}

jxx::lang::jbool Matcher::lookingAt() {
    const auto first = inputUtf8_.cbegin() + static_cast<std::ptrdiff_t>(regionStart_);
    const auto last = inputUtf8_.cbegin() + static_cast<std::ptrdiff_t>(regionEnd_);
    hasMatch_ = static_cast<jxx::lang::jbool>(std::regex_search(
        first,
        last,
        lastMatch_,
        pattern_->nativeRegex(),
        std::regex_constants::match_continuous));
    matchBase_ = regionStart_;
    if (hasMatch_) {
        searchPos_ = regionStart_ + static_cast<std::size_t>(lastMatch_.length());
    }
    hitEnd_ = hasMatch_ && searchPos_ == regionEnd_;
    requireEnd_ = false;
    return hasMatch_;
}

jxx::lang::jbool Matcher::find() {
    if (searchPos_ > regionEnd_) { hasMatch_ = false; hitEnd_ = true; return false; }
    const auto first = inputUtf8_.cbegin() + static_cast<std::ptrdiff_t>(searchPos_);
    const auto last = inputUtf8_.cbegin() + static_cast<std::ptrdiff_t>(regionEnd_);
    hasMatch_ = static_cast<jxx::lang::jbool>(std::regex_search(first, last, lastMatch_, pattern_->nativeRegex()));
    matchBase_ = searchPos_;
    if (!hasMatch_) { searchPos_ = regionEnd_; hitEnd_ = true; requireEnd_ = false; return false; }
    const auto matchStart = matchBase_ + static_cast<std::size_t>(lastMatch_.position(0));
    const auto matchEnd = matchStart + static_cast<std::size_t>(lastMatch_.length(0));
    searchPos_ = matchEnd == matchStart ? std::min(regionEnd_ + 1, matchEnd + 1) : matchEnd;
    hitEnd_ = matchEnd == regionEnd_;
    requireEnd_ = false;
    return true;
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
    return jxx::NEW<jxx::lang::String>(lastMatch_[static_cast<std::size_t>(group)].str());
}

jxx::lang::jint Matcher::groupCount() {
    return static_cast<jxx::lang::jint>(pattern_->nativeRegex().mark_count());
}

jxx::Ptr<Matcher> Matcher::region(jxx::lang::jint startValue, jxx::lang::jint endValue) {
    if (startValue < 0 || endValue < startValue || static_cast<std::size_t>(endValue) > inputUtf8_.size()) throw jxx::lang::IndexOutOfBoundsException();
    regionStart_ = static_cast<std::size_t>(startValue); regionEnd_ = static_cast<std::size_t>(endValue); return reset();
}
jxx::lang::jint Matcher::regionStart() const { return static_cast<jxx::lang::jint>(regionStart_); }
jxx::lang::jint Matcher::regionEnd() const { return static_cast<jxx::lang::jint>(regionEnd_); }
jxx::Ptr<Matcher> Matcher::useAnchoringBounds(jxx::lang::jbool value){ anchoringBounds_=value; return jxx::CAST<Matcher>(shared_from_this()); }
jxx::lang::jbool Matcher::hasAnchoringBounds() const{return anchoringBounds_;}
jxx::Ptr<Matcher> Matcher::useTransparentBounds(jxx::lang::jbool value){ transparentBounds_=value; return jxx::CAST<Matcher>(shared_from_this()); }
jxx::lang::jbool Matcher::hasTransparentBounds() const{return transparentBounds_;}
jxx::lang::jbool Matcher::hitEnd() const{return hitEnd_;}
jxx::lang::jbool Matcher::requireEnd() const{return requireEnd_;}

jxx::Ptr<Matcher> Matcher::appendReplacement(const jxx::Ptr<jxx::lang::StringBuffer>& buffer,const jxx::Ptr<jxx::lang::String>& replacement){
    if(!buffer||!replacement)throw jxx::lang::NullPointerException(); ensureMatchState();
    const auto matchStart=static_cast<std::size_t>(start()); const auto matchEnd=static_cast<std::size_t>(end());
    buffer->append(jxx::NEW<jxx::lang::String>(inputUtf8_.substr(appendPos_,matchStart-appendPos_)));
    buffer->append(jxx::NEW<jxx::lang::String>(lastMatch_.format(replacement->utf8())));
    appendPos_=matchEnd; return jxx::CAST<Matcher>(shared_from_this());
}
jxx::Ptr<jxx::lang::StringBuffer> Matcher::appendTail(const jxx::Ptr<jxx::lang::StringBuffer>& buffer){
    if(!buffer)throw jxx::lang::NullPointerException();
    buffer->append(jxx::NEW<jxx::lang::String>(inputUtf8_.substr(appendPos_,regionEnd_-appendPos_)));
    appendPos_=regionEnd_; return buffer;
}

jxx::Ptr<jxx::lang::String> Matcher::replaceAll(const jxx::Ptr<jxx::lang::String> replacement) {
    if (replacement == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return jxx::NEW<jxx::lang::String>(
        std::regex_replace(inputUtf8_, pattern_->nativeRegex(), replacement->utf8()));
}

jxx::Ptr<jxx::lang::String> Matcher::replaceFirst(const jxx::Ptr<jxx::lang::String> replacement) {
    if (replacement == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return jxx::NEW<jxx::lang::String>(
        std::regex_replace(
            inputUtf8_,
            pattern_->nativeRegex(),
            replacement->utf8(),
            std::regex_constants::format_first_only));
}

jxx::Ptr<jxx::lang::String> Matcher::quoteReplacement(const jxx::Ptr<jxx::lang::String> s) {
    if (s == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return jxx::NEW<jxx::lang::String>(escapeReplacement(s->utf8()));
}

} // namespace regex
} // namespace util
} // namespace jxx
