#pragma once

#include <cstddef>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <regex>
#include <utility>
#include <vector>
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.InputMismatchException.h"
#include "util/jxx.util.Iterator.h"
#include "util/regex/jxx.util.regex.MatchResult.h"
#include "util/regex/jxx.util.regex.Pattern.h"

namespace jxx {
namespace util {

class Locale;

class Scanner final
    : public virtual jxx::lang::Object
    , public virtual Iterator<jxx::lang::String>
    , public virtual jxx::io::Closeable {
private:
    jxx::Ptr<jxx::lang::String> source_;
    std::string sourceUtf8_;
    std::size_t position_;
    jxx::Ptr<regex::Pattern> delimiterPattern_;
    jxx::Ptr<Locale> locale_;
    jxx::lang::jint radix_;
    jxx::lang::jbool closed_;
    jxx::Ptr<jxx::io::IOException> lastException_;
    jxx::Ptr<jxx::io::Reader> sourceReader_;
    jxx::Ptr<jxx::io::InputStream> sourceInputStream_;
    jxx::Ptr<regex::MatchResult> lastMatchResult_;

    jxx::Ptr<Scanner> self();
    void ensureOpen() const;
    void clearMatchResult();
    std::size_t skipDelimitersFrom(std::size_t pos) const;
    jxx::lang::jbool locateNextToken(std::size_t from, std::size_t& tokenStart, std::size_t& tokenEnd) const;
    jxx::lang::jbool locateLine(std::size_t from, std::size_t& lineStart, std::size_t& lineEnd, std::size_t& newlineWidth) const;
    jxx::Ptr<jxx::lang::String> tokenString(std::size_t start, std::size_t end) const;

    jxx::Ptr<regex::MatchResult> performRegexSearch(
        jxx::Ptr<regex::Pattern> pattern,
        std::size_t searchStart,
        std::size_t searchLimit,
        jxx::lang::jbool anchoredAtCurrentPosition,
        std::size_t& matchStart,
        std::size_t& matchEnd,
        jxx::Ptr<jxx::lang::String>& matchText) const;

    jxx::lang::jbool matchTokenAtCurrentPosition(
        jxx::Ptr<regex::Pattern> pattern,
        std::size_t& tokenStart,
        std::size_t& tokenEnd,
        jxx::Ptr<jxx::lang::String>& tokenText,
        jxx::Ptr<regex::MatchResult>& snapshot) const;

public:
    explicit Scanner(jxx::Ptr<jxx::lang::String> source);
    explicit Scanner(jxx::Ptr<jxx::io::InputStream> source);
    Scanner(jxx::Ptr<jxx::io::InputStream> source, jxx::Ptr<jxx::lang::String> charsetName);
    explicit Scanner(jxx::Ptr<jxx::io::Reader> source);
    virtual ~Scanner() = default;

    virtual void close() override;
    virtual jxx::Ptr<jxx::io::IOException> ioException();

    virtual jxx::Ptr<regex::Pattern> delimiter();
    virtual jxx::Ptr<Scanner> useDelimiter(jxx::Ptr<regex::Pattern> pattern);
    virtual jxx::Ptr<Scanner> useDelimiter(jxx::Ptr<jxx::lang::String> pattern);

    virtual jxx::Ptr<Locale> locale();
    virtual jxx::Ptr<Scanner> useLocale(jxx::Ptr<Locale> locale);

    virtual jxx::lang::jint radix();
    virtual jxx::Ptr<Scanner> useRadix(jxx::lang::jint radix);

    virtual jxx::lang::jbool hasNext() override;
    virtual jxx::lang::jbool hasNext(jxx::Ptr<regex::Pattern> pattern);
    virtual jxx::lang::jbool hasNext(jxx::Ptr<jxx::lang::String> pattern);

    virtual jxx::Ptr<jxx::lang::String> next() override;
    virtual jxx::Ptr<jxx::lang::String> next(jxx::Ptr<regex::Pattern> pattern);
    virtual jxx::Ptr<jxx::lang::String> next(jxx::Ptr<jxx::lang::String> pattern);

    virtual void remove() override;

    virtual jxx::lang::jbool hasNextLine();
    virtual jxx::Ptr<jxx::lang::String> nextLine();

    virtual jxx::lang::jbool hasNextBoolean();
    virtual jxx::lang::jbool nextBoolean();

    virtual jxx::lang::jbool hasNextInt();
    virtual jxx::lang::jbool hasNextInt(jxx::lang::jint radix);
    virtual jxx::lang::jint nextInt();
    virtual jxx::lang::jint nextInt(jxx::lang::jint radix);

    virtual jxx::lang::jbool hasNextLong();
    virtual jxx::lang::jbool hasNextLong(jxx::lang::jint radix);
    virtual jxx::lang::jlong nextLong();
    virtual jxx::lang::jlong nextLong(jxx::lang::jint radix);

    virtual jxx::lang::jbool hasNextDouble();
    virtual jxx::lang::jdouble nextDouble();

    virtual jxx::Ptr<jxx::lang::String> findInLine(jxx::Ptr<regex::Pattern> pattern);
    virtual jxx::Ptr<jxx::lang::String> findInLine(jxx::Ptr<jxx::lang::String> pattern);

    virtual jxx::Ptr<jxx::lang::String> findWithinHorizon(jxx::Ptr<regex::Pattern> pattern, jxx::lang::jint horizon);
    virtual jxx::Ptr<jxx::lang::String> findWithinHorizon(jxx::Ptr<jxx::lang::String> pattern, jxx::lang::jint horizon);

    virtual jxx::Ptr<Scanner> skip(jxx::Ptr<regex::Pattern> pattern);
    virtual jxx::Ptr<Scanner> skip(jxx::Ptr<jxx::lang::String> pattern);

    virtual jxx::Ptr<regex::MatchResult> match();

    jxx::lang::jbool hasNextFloat();

    jxx::lang::jfloat nextFloat();


    jxx::lang::jbool hasNextByte();

    jxx::lang::jbool hasNextByte(jxx::lang::jint radix);
    jxx::lang::jbyte nextByte();

    jxx::lang::jbyte nextByte(jxx::lang::jint radix);

    jxx::lang::jbool hasNextShort();

    jxx::lang::jbool hasNextShort(jxx::lang::jint radix);

    jxx::lang::jshort nextShort();

    jxx::lang::jshort nextShort(jxx::lang::jint radix);
};

} // namespace util
} // namespace jxx
