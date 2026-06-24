
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.Scanner.h"


namespace jxx {
namespace util {
namespace {

class SnapshotMatchResult final : public virtual regex::MatchResult {
private:
    std::vector<jxx::lang::jint> starts_;
    std::vector<jxx::lang::jint> ends_;
    std::vector<jxx::Ptr<jxx::lang::String>> groups_;
public:
    SnapshotMatchResult(
        std::vector<jxx::lang::jint> starts,
        std::vector<jxx::lang::jint> ends,
        std::vector<jxx::Ptr<jxx::lang::String>> groups)
        : starts_(std::move(starts))
        , ends_(std::move(ends))
        , groups_(std::move(groups)) {
    }
    virtual ~SnapshotMatchResult() = default;

    virtual jxx::lang::jint start() override { return start(static_cast<jxx::lang::jint>(0)); }
    virtual jxx::lang::jint start(jxx::lang::jint group) override {
        if (group < 0 || static_cast<std::size_t>(group) >= starts_.size()) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
        return starts_[static_cast<std::size_t>(group)];
    }
    virtual jxx::lang::jint end() override { return end(static_cast<jxx::lang::jint>(0)); }
    virtual jxx::lang::jint end(jxx::lang::jint group) override {
        if (group < 0 || static_cast<std::size_t>(group) >= ends_.size()) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
        return ends_[static_cast<std::size_t>(group)];
    }
    virtual jxx::Ptr<jxx::lang::String> group() override { return group(static_cast<jxx::lang::jint>(0)); }
    virtual jxx::Ptr<jxx::lang::String> group(jxx::lang::jint groupIndex) override {
        if (groupIndex < 0 || static_cast<std::size_t>(groupIndex) >= groups_.size()) {
            throw jxx::lang::IndexOutOfBoundsException();
        }
        return groups_[static_cast<std::size_t>(groupIndex)];
    }
    virtual jxx::lang::jint groupCount() override {
        return static_cast<jxx::lang::jint>(groups_.empty() ? 0 : groups_.size() - 1U);
    }
};

static jxx::Ptr<jxx::lang::String> defaultDelimiterPattern() {
    return std::make_shared<jxx::lang::String>("\\s+");
}

static std::u16string readAllFromReader(jxx::Ptr<jxx::io::Reader> reader) {
    std::u16string out;
    while (true) {
        const auto ch = reader->read();
        if (ch < 0) break;
        out.push_back(static_cast<char16_t>(ch));
    }
    return out;
}

static std::vector<unsigned char> readAllBytes(jxx::Ptr<jxx::io::InputStream> in) {
    std::vector<unsigned char> bytes;
    while (true) {
        const auto b = in->read();
        if (b < 0) break;
        bytes.push_back(static_cast<unsigned char>(b & 0xFF));
    }
    return bytes;
}

static std::string bytesToString(const std::vector<unsigned char>& bytes) {
    return std::string(bytes.begin(), bytes.end());
}

static jxx::Ptr<jxx::lang::String> decodeBytes(
    const std::vector<unsigned char>& bytes,
    jxx::Ptr<jxx::lang::String> charsetName) {

    if (charsetName == nullptr) {
        return std::make_shared<jxx::lang::String>(bytesToString(bytes));
    }

    const std::string charset = charsetName->utf8();
    if (charset == "UTF-8" || charset == "UTF8" || charset == "utf-8" || charset == "utf8") {
        return std::make_shared<jxx::lang::String>(bytesToString(bytes));
    }
    if (charset == "ISO-8859-1" || charset == "iso-8859-1" || charset == "LATIN1" || charset == "latin1") {
        std::u16string text;
        text.reserve(bytes.size());
        for (unsigned char b : bytes) text.push_back(static_cast<char16_t>(b));
        return std::make_shared<jxx::lang::String>(text);
    }
    throw jxx::lang::IllegalArgumentException();
}

static std::string asciiLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return s;
}

static jxx::lang::jbool tryParseLong(
    jxx::Ptr<jxx::lang::String> token,
    jxx::lang::jint radix,
    jxx::lang::jlong& out) {
    if (token == nullptr) return static_cast<jxx::lang::jbool>(false);
    try {
        std::size_t idx = 0;
        const std::string s = token->utf8();
        long long value = std::stoll(s, &idx, radix);
        if (idx != s.size()) return static_cast<jxx::lang::jbool>(false);
        out = static_cast<jxx::lang::jlong>(value);
        return static_cast<jxx::lang::jbool>(true);
    } catch (...) {
        return static_cast<jxx::lang::jbool>(false);
    }
}

static jxx::lang::jbool tryParseInt(
    jxx::Ptr<jxx::lang::String> token,
    jxx::lang::jint radix,
    jxx::lang::jint& out) {
    jxx::lang::jlong value = 0;
    if (!tryParseLong(token, radix, value)) {
        return static_cast<jxx::lang::jbool>(false);
    }
    if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
        return static_cast<jxx::lang::jbool>(false);
    }
    out = static_cast<jxx::lang::jint>(value);
    return static_cast<jxx::lang::jbool>(true);
}

static jxx::lang::jbool tryParseDouble(jxx::Ptr<jxx::lang::String> token, jxx::lang::jdouble& out) {
    if (token == nullptr) return static_cast<jxx::lang::jbool>(false);
    try {
        std::size_t idx = 0;
        const std::string s = token->utf8();
        double value = std::stod(s, &idx);
        if (idx != s.size()) return static_cast<jxx::lang::jbool>(false);
        out = static_cast<jxx::lang::jdouble>(value);
        return static_cast<jxx::lang::jbool>(true);
    } catch (...) {
        return static_cast<jxx::lang::jbool>(false);
    }
}

static jxx::lang::jbool tryParseBoolean(jxx::Ptr<jxx::lang::String> token, jxx::lang::jbool& out) {
    if (token == nullptr) return static_cast<jxx::lang::jbool>(false);
    const std::string s = asciiLower(token->utf8());
    if (s == "true") {
        out = static_cast<jxx::lang::jbool>(true);
        return static_cast<jxx::lang::jbool>(true);
    }
    if (s == "false") {
        out = static_cast<jxx::lang::jbool>(false);
        return static_cast<jxx::lang::jbool>(true);
    }
    return static_cast<jxx::lang::jbool>(false);
}

static jxx::Ptr<jxx::lang::String> requirePatternString(jxx::Ptr<jxx::lang::String> pattern) {
    if (pattern == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return pattern;
}

static jxx::Ptr<regex::MatchResult> snapshotForTokenMatch(
    jxx::Ptr<regex::Pattern> pattern,
    const std::string& tokenUtf8,
    std::size_t absoluteTokenStart,
    jxx::Ptr<jxx::lang::String>& tokenTextOut) {

    std::smatch m;
    if (!std::regex_match(tokenUtf8, m, pattern->nativeRegex())) {
        return nullptr;
    }

    tokenTextOut = std::make_shared<jxx::lang::String>(m.str());
    std::vector<jxx::lang::jint> starts;
    std::vector<jxx::lang::jint> ends;
    std::vector<jxx::Ptr<jxx::lang::String>> groups;
    starts.reserve(m.size());
    ends.reserve(m.size());
    groups.reserve(m.size());

    for (std::size_t i = 0; i < m.size(); ++i) {
        if (!m[i].matched) {
            starts.push_back(static_cast<jxx::lang::jint>(-1));
            ends.push_back(static_cast<jxx::lang::jint>(-1));
            groups.push_back(nullptr);
            continue;
        }
        const auto subgroupStart = static_cast<jxx::lang::jint>(absoluteTokenStart + static_cast<std::size_t>(m.position(i)));
        const auto subgroupEnd = static_cast<jxx::lang::jint>(absoluteTokenStart + static_cast<std::size_t>(m.position(i) + m.length(i)));
        starts.push_back(subgroupStart);
        ends.push_back(subgroupEnd);
        groups.push_back(std::make_shared<jxx::lang::String>(m.str(i)));
    }

    return std::make_shared<SnapshotMatchResult>(std::move(starts), std::move(ends), std::move(groups));
}

} // anonymous namespace

Scanner::Scanner(jxx::Ptr<jxx::lang::String> source)
    : source_(source)
    , sourceUtf8_(source == nullptr ? std::string() : source->utf8())
    , position_(0)
    , delimiterPattern_(regex::Pattern::compile(defaultDelimiterPattern()))
    , locale_(nullptr)
    , radix_(10)
    , closed_(static_cast<jxx::lang::jbool>(false))
    , lastException_(nullptr)
    , sourceReader_(nullptr)
    , sourceInputStream_(nullptr)
    , lastMatchResult_(nullptr) {
    if (source == nullptr) {
        throw jxx::lang::NullPointerException();
    }
}

Scanner::Scanner(jxx::Ptr<jxx::io::InputStream> source)
    : Scanner(source, nullptr) {
}

Scanner::Scanner(jxx::Ptr<jxx::io::InputStream> source, jxx::Ptr<jxx::lang::String> charsetName)
    : Scanner(decodeBytes(readAllBytes(source), charsetName)) {
    if (source == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    sourceInputStream_ = source;
}

Scanner::Scanner(jxx::Ptr<jxx::io::Reader> source)
    : Scanner(std::make_shared<jxx::lang::String>(readAllFromReader(source))) {
    if (source == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    sourceReader_ = source;
}

jxx::Ptr<Scanner> Scanner::self() {
    return jxx::Ptr<Scanner>(this, [](Scanner*) {});
}

void Scanner::ensureOpen() const {
    if (closed_) {
        throw jxx::lang::IllegalStateException();
    }
}

void Scanner::clearMatchResult() {
    lastMatchResult_ = nullptr;
}

std::size_t Scanner::skipDelimitersFrom(std::size_t pos) const {
    std::size_t current = pos;
    while (current < sourceUtf8_.size()) {
        const std::string remaining = sourceUtf8_.substr(current);
        std::smatch m;
        if (!std::regex_search(remaining, m, delimiterPattern_->nativeRegex(), std::regex_constants::match_continuous)) {
            break;
        }
        if (m.length() == 0) {
            break;
        }
        current += static_cast<std::size_t>(m.length());
    }
    return current;
}

jxx::lang::jbool Scanner::locateNextToken(std::size_t from, std::size_t& tokenStart, std::size_t& tokenEnd) const {
    tokenStart = skipDelimitersFrom(from);
    if (tokenStart >= sourceUtf8_.size()) {
        return static_cast<jxx::lang::jbool>(false);
    }

    const std::string remaining = sourceUtf8_.substr(tokenStart);
    std::smatch m;
    if (std::regex_search(remaining, m, delimiterPattern_->nativeRegex())) {
        if (m.position() == 0 && m.length() > 0) {
            tokenEnd = tokenStart;
            return static_cast<jxx::lang::jbool>(false);
        }
        tokenEnd = tokenStart + static_cast<std::size_t>(m.position());
    } else {
        tokenEnd = sourceUtf8_.size();
    }
    if (tokenEnd < tokenStart) tokenEnd = tokenStart;
    return static_cast<jxx::lang::jbool>(tokenEnd > tokenStart);
}

jxx::lang::jbool Scanner::locateLine(std::size_t from, std::size_t& lineStart, std::size_t& lineEnd, std::size_t& newlineWidth) const {
    lineStart = from;
    if (lineStart > sourceUtf8_.size()) {
        return static_cast<jxx::lang::jbool>(false);
    }
    if (lineStart == sourceUtf8_.size()) {
        return static_cast<jxx::lang::jbool>(false);
    }
    const std::size_t rn = sourceUtf8_.find("\r\n", lineStart);
    const std::size_t r = sourceUtf8_.find('\r', lineStart);
    const std::size_t n = sourceUtf8_.find('\n', lineStart);
    std::size_t best = std::string::npos;
    newlineWidth = 0;
    if (rn != std::string::npos) {
        best = rn;
        newlineWidth = 2;
    }
    if (r != std::string::npos && (best == std::string::npos || r < best)) {
        best = r;
        newlineWidth = 1;
    }
    if (n != std::string::npos && (best == std::string::npos || n < best)) {
        best = n;
        newlineWidth = 1;
    }
    if (best == std::string::npos) {
        lineEnd = sourceUtf8_.size();
        newlineWidth = 0;
    } else {
        lineEnd = best;
    }
    return static_cast<jxx::lang::jbool>(true);
}

jxx::Ptr<jxx::lang::String> Scanner::tokenString(std::size_t start, std::size_t end) const {
    return std::make_shared<jxx::lang::String>(sourceUtf8_.substr(start, end - start));
}

jxx::Ptr<regex::MatchResult> Scanner::performRegexSearch(
    jxx::Ptr<regex::Pattern> pattern,
    std::size_t searchStart,
    std::size_t searchLimit,
    jxx::lang::jbool anchoredAtCurrentPosition,
    std::size_t& matchStart,
    std::size_t& matchEnd,
    jxx::Ptr<jxx::lang::String>& matchText) const {
    if (pattern == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    if (searchStart > sourceUtf8_.size()) {
        return nullptr;
    }
    const std::size_t limit = std::min(searchLimit, sourceUtf8_.size());
    if (searchStart > limit) {
        return nullptr;
    }

    const std::string target = sourceUtf8_.substr(searchStart, limit - searchStart);
    std::smatch m;
    const bool ok = anchoredAtCurrentPosition
        ? std::regex_search(target, m, pattern->nativeRegex(), std::regex_constants::match_continuous)
        : std::regex_search(target, m, pattern->nativeRegex());
    if (!ok) {
        return nullptr;
    }

    matchStart = searchStart + static_cast<std::size_t>(m.position());
    matchEnd = matchStart + static_cast<std::size_t>(m.length());
    matchText = std::make_shared<jxx::lang::String>(m.str());

    std::vector<jxx::lang::jint> starts;
    std::vector<jxx::lang::jint> ends;
    std::vector<jxx::Ptr<jxx::lang::String>> groups;
    starts.reserve(m.size());
    ends.reserve(m.size());
    groups.reserve(m.size());

    for (std::size_t i = 0; i < m.size(); ++i) {
        if (!m[i].matched) {
            starts.push_back(static_cast<jxx::lang::jint>(-1));
            ends.push_back(static_cast<jxx::lang::jint>(-1));
            groups.push_back(nullptr);
            continue;
        }
        const jxx::lang::jint subgroupStart = static_cast<jxx::lang::jint>(searchStart + m.position(i));
        const jxx::lang::jint subgroupEnd = static_cast<jxx::lang::jint>(searchStart + m.position(i) + m.length(i));
        starts.push_back(subgroupStart);
        ends.push_back(subgroupEnd);
        groups.push_back(std::make_shared<jxx::lang::String>(m.str(i)));
    }

    return std::make_shared<SnapshotMatchResult>(std::move(starts), std::move(ends), std::move(groups));
}

jxx::lang::jbool Scanner::matchTokenAtCurrentPosition(
    jxx::Ptr<regex::Pattern> pattern,
    std::size_t& tokenStart,
    std::size_t& tokenEnd,
    jxx::Ptr<jxx::lang::String>& tokenText,
    jxx::Ptr<regex::MatchResult>& snapshot) const {
    if (pattern == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    if (!locateNextToken(position_, tokenStart, tokenEnd)) {
        tokenText = nullptr;
        snapshot = nullptr;
        return static_cast<jxx::lang::jbool>(false);
    }
    const std::string token = sourceUtf8_.substr(tokenStart, tokenEnd - tokenStart);
    snapshot = snapshotForTokenMatch(pattern, token, tokenStart, tokenText);
    return static_cast<jxx::lang::jbool>(snapshot != nullptr);
}

void Scanner::close() {
    if (closed_) return;
    try {
        if (sourceReader_ != nullptr) sourceReader_->close();
        if (sourceInputStream_ != nullptr) sourceInputStream_->close();
    } catch (...) {
        // Stage 3 still keeps best-effort close semantics.
    }
    closed_ = static_cast<jxx::lang::jbool>(true);
}

jxx::Ptr<jxx::io::IOException> Scanner::ioException() {
    return lastException_;
}

jxx::Ptr<regex::Pattern> Scanner::delimiter() {
    ensureOpen();
    return delimiterPattern_;
}

jxx::Ptr<Scanner> Scanner::useDelimiter(jxx::Ptr<regex::Pattern> pattern) {
    ensureOpen();
    if (pattern == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    delimiterPattern_ = pattern;
    clearMatchResult();
    return self();
}

jxx::Ptr<Scanner> Scanner::useDelimiter(jxx::Ptr<jxx::lang::String> pattern) {
    ensureOpen();
    delimiterPattern_ = regex::Pattern::compile(requirePatternString(pattern));
    clearMatchResult();
    return self();
}

jxx::Ptr<Locale> Scanner::locale() {
    ensureOpen();
    return locale_;
}

jxx::Ptr<Scanner> Scanner::useLocale(jxx::Ptr<Locale> locale) {
    ensureOpen();
    if (locale == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    locale_ = locale;
    return self();
}

jxx::lang::jint Scanner::radix() {
    ensureOpen();
    return radix_;
}

jxx::Ptr<Scanner> Scanner::useRadix(jxx::lang::jint radix) {
    ensureOpen();
    if (radix < 2 || radix > 36) {
        throw jxx::lang::IllegalArgumentException();
    }
    radix_ = radix;
    return self();
}

jxx::lang::jbool Scanner::hasNext() {
    ensureOpen();
    std::size_t start = 0;
    std::size_t end = 0;
    return locateNextToken(position_, start, end);
}

jxx::lang::jbool Scanner::hasNext(jxx::Ptr<regex::Pattern> pattern) {
    ensureOpen();
    std::size_t tokenStart = 0;
    std::size_t tokenEnd = 0;
    jxx::Ptr<jxx::lang::String> tokenText = nullptr;
    jxx::Ptr<regex::MatchResult> snapshot = nullptr;
    return matchTokenAtCurrentPosition(pattern, tokenStart, tokenEnd, tokenText, snapshot);
}

jxx::lang::jbool Scanner::hasNext(jxx::Ptr<jxx::lang::String> pattern) {
    return hasNext(regex::Pattern::compile(requirePatternString(pattern)));
}

jxx::Ptr<jxx::lang::String> Scanner::next() {
    ensureOpen();
    std::size_t start = 0;
    std::size_t end = 0;
    if (!locateNextToken(position_, start, end)) {
        throw jxx::util::NoSuchElementException();
    }
    position_ = end;
    clearMatchResult();
    return tokenString(start, end);
}

jxx::Ptr<jxx::lang::String> Scanner::next(jxx::Ptr<regex::Pattern> pattern) {
    ensureOpen();
    std::size_t tokenStart = 0;
    std::size_t tokenEnd = 0;
    jxx::Ptr<jxx::lang::String> tokenText = nullptr;
    jxx::Ptr<regex::MatchResult> snapshot = nullptr;
    if (!matchTokenAtCurrentPosition(pattern, tokenStart, tokenEnd, tokenText, snapshot)) {
        std::size_t anyStart = 0;
        std::size_t anyEnd = 0;
        if (!locateNextToken(position_, anyStart, anyEnd)) {
            throw jxx::util::NoSuchElementException();
        }
        throw InputMismatchException();
    }
    position_ = tokenEnd;
    lastMatchResult_ = snapshot;
    return tokenText;
}

jxx::Ptr<jxx::lang::String> Scanner::next(jxx::Ptr<jxx::lang::String> pattern) {
    return next(regex::Pattern::compile(requirePatternString(pattern)));
}

void Scanner::remove() {
    throw jxx::lang::UnsupportedOperationException();
}

jxx::lang::jbool Scanner::hasNextLine() {
    ensureOpen();
    std::size_t lineStart = 0;
    std::size_t lineEnd = 0;
    std::size_t newlineWidth = 0;
    return locateLine(position_, lineStart, lineEnd, newlineWidth);
}

jxx::Ptr<jxx::lang::String> Scanner::nextLine() {
    ensureOpen();
    std::size_t lineStart = 0;
    std::size_t lineEnd = 0;
    std::size_t newlineWidth = 0;
    if (!locateLine(position_, lineStart, lineEnd, newlineWidth)) {
        throw jxx::util::NoSuchElementException();
    }
    position_ = lineEnd + newlineWidth;
    clearMatchResult();
    return std::make_shared<jxx::lang::String>(sourceUtf8_.substr(lineStart, lineEnd - lineStart));
}

jxx::lang::jbool Scanner::hasNextBoolean() {
    ensureOpen();
    std::size_t start = 0;
    std::size_t end = 0;
    if (!locateNextToken(position_, start, end)) return static_cast<jxx::lang::jbool>(false);
    jxx::lang::jbool value = static_cast<jxx::lang::jbool>(false);
    return tryParseBoolean(tokenString(start, end), value);
}

jxx::lang::jbool Scanner::nextBoolean() {
    ensureOpen();
    auto token = next();
    jxx::lang::jbool value = static_cast<jxx::lang::jbool>(false);
    if (!tryParseBoolean(token, value)) {
        throw InputMismatchException();
    }
    return value;
}

jxx::lang::jbool Scanner::hasNextInt() {
    return hasNextInt(radix_);
}

jxx::lang::jbool Scanner::hasNextInt(jxx::lang::jint radix) {
    ensureOpen();
    if (radix < 2 || radix > 36) {
        throw jxx::lang::IllegalArgumentException();
    }
    std::size_t start = 0;
    std::size_t end = 0;
    if (!locateNextToken(position_, start, end)) return static_cast<jxx::lang::jbool>(false);
    jxx::lang::jint value = 0;
    return tryParseInt(tokenString(start, end), radix, value);
}

jxx::lang::jint Scanner::nextInt() {
    return nextInt(radix_);
}

jxx::lang::jint Scanner::nextInt(jxx::lang::jint radix) {
    ensureOpen();
    if (radix < 2 || radix > 36) {
        throw jxx::lang::IllegalArgumentException();
    }
    auto token = next();
    jxx::lang::jint value = 0;
    if (!tryParseInt(token, radix, value)) {
        throw InputMismatchException();
    }
    return value;
}

jxx::lang::jbool Scanner::hasNextLong() {
    return hasNextLong(radix_);
}

jxx::lang::jbool Scanner::hasNextLong(jxx::lang::jint radix) {
    ensureOpen();
    if (radix < 2 || radix > 36) {
        throw jxx::lang::IllegalArgumentException();
    }
    std::size_t start = 0;
    std::size_t end = 0;
    if (!locateNextToken(position_, start, end)) return static_cast<jxx::lang::jbool>(false);
    jxx::lang::jlong value = 0;
    return tryParseLong(tokenString(start, end), radix, value);
}

jxx::lang::jlong Scanner::nextLong() {
    return nextLong(radix_);
}

jxx::lang::jlong Scanner::nextLong(jxx::lang::jint radix) {
    ensureOpen();
    if (radix < 2 || radix > 36) {
        throw jxx::lang::IllegalArgumentException();
    }
    auto token = next();
    jxx::lang::jlong value = 0;
    if (!tryParseLong(token, radix, value)) {
        throw InputMismatchException();
    }
    return value;
}

jxx::lang::jbool Scanner::hasNextDouble() {
    ensureOpen();
    std::size_t start = 0;
    std::size_t end = 0;
    if (!locateNextToken(position_, start, end)) return static_cast<jxx::lang::jbool>(false);
    jxx::lang::jdouble value = 0.0;
    return tryParseDouble(tokenString(start, end), value);
}

jxx::lang::jdouble Scanner::nextDouble() {
    ensureOpen();
    auto token = next();
    jxx::lang::jdouble value = 0.0;
    if (!tryParseDouble(token, value)) {
        throw InputMismatchException();
    }
    return value;
}

jxx::Ptr<jxx::lang::String> Scanner::findInLine(jxx::Ptr<regex::Pattern> pattern) {
    ensureOpen();
    std::size_t lineStart = 0;
    std::size_t lineEnd = 0;
    std::size_t newlineWidth = 0;
    if (!locateLine(position_, lineStart, lineEnd, newlineWidth)) {
        clearMatchResult();
        return nullptr;
    }
    std::size_t matchStart = 0;
    std::size_t matchEnd = 0;
    jxx::Ptr<jxx::lang::String> matchText = nullptr;
    auto snapshot = performRegexSearch(pattern, position_, lineEnd, static_cast<jxx::lang::jbool>(false), matchStart, matchEnd, matchText);
    if (snapshot == nullptr) {
        clearMatchResult();
        return nullptr;
    }
    position_ = matchEnd;
    lastMatchResult_ = snapshot;
    return matchText;
}

jxx::Ptr<jxx::lang::String> Scanner::findInLine(jxx::Ptr<jxx::lang::String> pattern) {
    return findInLine(regex::Pattern::compile(requirePatternString(pattern)));
}

jxx::Ptr<jxx::lang::String> Scanner::findWithinHorizon(jxx::Ptr<regex::Pattern> pattern, jxx::lang::jint horizon) {
    ensureOpen();
    if (horizon < 0) {
        throw jxx::lang::IllegalArgumentException();
    }
    const std::size_t limit = (horizon == 0)
        ? sourceUtf8_.size()
        : std::min(sourceUtf8_.size(), position_ + static_cast<std::size_t>(horizon));
    std::size_t matchStart = 0;
    std::size_t matchEnd = 0;
    jxx::Ptr<jxx::lang::String> matchText = nullptr;
    auto snapshot = performRegexSearch(pattern, position_, limit, static_cast<jxx::lang::jbool>(false), matchStart, matchEnd, matchText);
    if (snapshot == nullptr) {
        clearMatchResult();
        return nullptr;
    }
    position_ = matchEnd;
    lastMatchResult_ = snapshot;
    return matchText;
}

jxx::Ptr<jxx::lang::String> Scanner::findWithinHorizon(jxx::Ptr<jxx::lang::String> pattern, jxx::lang::jint horizon) {
    return findWithinHorizon(regex::Pattern::compile(requirePatternString(pattern)), horizon);
}

jxx::Ptr<Scanner> Scanner::skip(jxx::Ptr<regex::Pattern> pattern) {
    ensureOpen();
    std::size_t matchStart = 0;
    std::size_t matchEnd = 0;
    jxx::Ptr<jxx::lang::String> matchText = nullptr;
    auto snapshot = performRegexSearch(pattern, position_, sourceUtf8_.size(), static_cast<jxx::lang::jbool>(true), matchStart, matchEnd, matchText);
    if (snapshot == nullptr || matchStart != position_) {
        throw jxx::util::NoSuchElementException();
    }
    position_ = matchEnd;
    lastMatchResult_ = snapshot;
    return self();
}

jxx::Ptr<Scanner> Scanner::skip(jxx::Ptr<jxx::lang::String> pattern) {
    return skip(regex::Pattern::compile(requirePatternString(pattern)));
}

jxx::Ptr<regex::MatchResult> Scanner::match() {
    ensureOpen();
    if (lastMatchResult_ == nullptr) {
        throw jxx::lang::IllegalStateException();
    }
    return lastMatchResult_;
}

} // namespace util
} // namespace jxx
