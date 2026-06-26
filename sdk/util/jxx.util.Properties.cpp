
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "util/jxx.util.NoSuchElementException.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "io/jxx.io.PrintStream.h"
#include "io/jxx.io.PrintWriter.h"
#include "io/jxx.io.Reader.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Properties.h"

namespace jxx {
namespace util {
namespace {

class ObjectEnumerationImpl : public virtual Enumeration<jxx::lang::Object> {
private:
    std::vector<jxx::Ptr<jxx::lang::Object>> snapshot_;
    std::size_t index_;
public:
    explicit ObjectEnumerationImpl(std::vector<jxx::Ptr<jxx::lang::Object>> snapshot)
        : snapshot_(std::move(snapshot)), index_(0) {
    }
    virtual ~ObjectEnumerationImpl() = default;
    virtual jxx::lang::jbool hasMoreElements() override {
        return static_cast<jxx::lang::jbool>(index_ < snapshot_.size());
    }
    virtual jxx::Ptr<jxx::lang::Object> nextElement() override {
        if (index_ >= snapshot_.size()) {
            throw jxx::util::NoSuchElementException();
        }
        return snapshot_[index_++];
    }
};

class StringIteratorImpl : public virtual Iterator<jxx::lang::String> {
private:
    std::vector<jxx::Ptr<jxx::lang::String>> snapshot_;
    std::size_t index_;
public:
    explicit StringIteratorImpl(std::vector<jxx::Ptr<jxx::lang::String>> snapshot)
        : snapshot_(std::move(snapshot)), index_(0) {
    }
    virtual ~StringIteratorImpl() = default;
    virtual jxx::lang::jbool hasNext() override {
        return static_cast<jxx::lang::jbool>(index_ < snapshot_.size());
    }
    virtual jxx::Ptr<jxx::lang::String> next() override {
        if (index_ >= snapshot_.size()) {
            throw jxx::util::NoSuchElementException();
        }
        return snapshot_[index_++];
    }
    virtual void remove() override {
        throw jxx::lang::UnsupportedOperationException();
    }
};

class StringSetImpl : public virtual Set<jxx::lang::String> {
private:
    std::vector<jxx::Ptr<jxx::lang::String>> values_;
public:
    explicit StringSetImpl(std::vector<jxx::Ptr<jxx::lang::String>> values)
        : values_(std::move(values)) {
    }
    virtual ~StringSetImpl() = default;
    virtual jxx::lang::jint size() override {
        return static_cast<jxx::lang::jint>(values_.size());
    }
    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        for (const auto& value : values_) {
            if (o == nullptr) {
                if (value == nullptr) {
                    return static_cast<jxx::lang::jbool>(true);
                }
            } else if (value != nullptr && o->equals(jxx::CAST<jxx::lang::Object>(value))) {
                return static_cast<jxx::lang::jbool>(true);
            }
        }
        return static_cast<jxx::lang::jbool>(false);
    }
    virtual jxx::Ptr<Iterator<jxx::lang::String>> iterator() override {
        return std::make_shared<StringIteratorImpl>(values_);
    }
    virtual jxx::lang::jbool add(jxx::Ptr<jxx::lang::String> /*e*/) override {
        throw jxx::lang::UnsupportedOperationException();
    }
    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> /*o*/) override {
        throw jxx::lang::UnsupportedOperationException();
    }
    virtual void clear() override {
        throw jxx::lang::UnsupportedOperationException();
    }   
          
    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> toArray() override {
        auto array = jxx::NEW<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>>(values_.size());
        for (std::size_t i = 0; i < values_.size(); ++i) {
            (*array)(i) = jxx::CAST<jxx::lang::Object, jxx::lang::Object>(values_[i]);
        }
        return array;
	}
    
    virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override { throw jxx::lang::UnsupportedOperationException(); }
	virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends < jxx::lang::String>> c) override { throw jxx::lang::UnsupportedOperationException(); }
	virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override { throw jxx::lang::UnsupportedOperationException(); }
	virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override { throw jxx::lang::UnsupportedOperationException(); }
    virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override { throw jxx::lang::UnsupportedOperationException(); }
    virtual jxx::lang::jint hashCode() override { throw jxx::lang::UnsupportedOperationException(); }
	virtual jxx::Ptr<Spliterator<jxx::lang::String>> spliterator() override { throw jxx::lang::UnsupportedOperationException(); }
};

static inline jxx::lang::jbool containsObjectVector(const std::vector<jxx::Ptr<jxx::lang::Object>>& values, jxx::Ptr<jxx::lang::Object> value) {
    for (const auto& existing : values) {
        if (value == nullptr) {
            if (existing == nullptr) {
                return static_cast<jxx::lang::jbool>(true);
            }
        } else if (existing != nullptr && value->equals(existing)) {
            return static_cast<jxx::lang::jbool>(true);
        }
    }
    return static_cast<jxx::lang::jbool>(false);
}

static inline jxx::lang::jbool containsStringVector(const std::vector<jxx::Ptr<jxx::lang::String>>& values, jxx::Ptr<jxx::lang::String> value) {
    for (const auto& existing : values) {
        if (value == nullptr) {
            if (existing == nullptr) {
                return static_cast<jxx::lang::jbool>(true);
            }
        } else if (existing != nullptr && value->equals(jxx::CAST<jxx::lang::Object>(existing))) {
            return static_cast<jxx::lang::jbool>(true);
        }
    }
    return static_cast<jxx::lang::jbool>(false);
}

static inline jxx::lang::jbool isWs(char16_t ch) {
    return static_cast<jxx::lang::jbool>(ch == u' ' || ch == u'\t' || ch == u'\f');
}

static jxx::lang::jint hexValue(char16_t ch) {
    if (ch >= u'0' && ch <= u'9') return static_cast<jxx::lang::jint>(ch - u'0');
    if (ch >= u'a' && ch <= u'f') return static_cast<jxx::lang::jint>(10 + (ch - u'a'));
    if (ch >= u'A' && ch <= u'F') return static_cast<jxx::lang::jint>(10 + (ch - u'A'));
    throw jxx::lang::IllegalArgumentException();
}

static std::u16string loadConvert(const std::u16string& in) {
    std::u16string out;
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        char16_t c = in[i];
        if (c != u'\\') {
            out.push_back(c);
            continue;
        }
        if (i + 1 >= in.size()) {
            out.push_back(u'\\');
            break;
        }
        char16_t next = in[++i];
        switch (next) {
            case u't': out.push_back(u'\t'); break;
            case u'n': out.push_back(u'\n'); break;
            case u'r': out.push_back(u'\r'); break;
            case u'f': out.push_back(u'\f'); break;
            case u'\\': out.push_back(u'\\'); break;
            case u'u': {
                if (i + 4 >= in.size()) {
                    throw jxx::lang::IllegalArgumentException();
                }
                jxx::lang::jint code = 0;
                for (jxx::lang::jint k = 0; k < 4; ++k) {
                    code = static_cast<jxx::lang::jint>((code << 4) + hexValue(in[++i]));
                }
                out.push_back(static_cast<char16_t>(code));
                break;
            }
            default: out.push_back(next); break;
        }
    }
    return out;
}

static std::u16string saveConvert(const std::u16string& in, jxx::lang::jbool escapeSpace, jxx::lang::jbool escapeUnicode) {
    static const char16_t hex[] = u"0123456789ABCDEF";
    std::u16string out;
    out.reserve(in.size() * 2U);
    for (std::size_t i = 0; i < in.size(); ++i) {
        char16_t c = in[i];
        switch (c) {
            case u' ': if (i == 0 || escapeSpace) out.push_back(u'\\'); out.push_back(u' '); break;
            case u'\\': out.append(u"\\\\"); break;
            case u'\t': out.append(u"\\t"); break;
            case u'\n': out.append(u"\\n"); break;
            case u'\r': out.append(u"\\r"); break;
            case u'\f': out.append(u"\\f"); break;
            case u'=': case u':': case u'#': case u'!': out.push_back(u'\\'); out.push_back(c); break;
            default:
                if (escapeUnicode && (c < 0x20 || c > 0x7e)) {
                    out.append(u"\\u");
                    out.push_back(hex[(c >> 12) & 0xF]);
                    out.push_back(hex[(c >> 8) & 0xF]);
                    out.push_back(hex[(c >> 4) & 0xF]);
                    out.push_back(hex[c & 0xF]);
                } else {
                    out.push_back(c);
                }
                break;
        }
    }
    return out;
}

static std::u16string formatDateLine() {
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm tmv{};
#if defined(_WIN32)
    localtime_s(&tmv, &tt);
#else
    localtime_r(&tt, &tmv);
#endif
    std::ostringstream oss;
    oss << '#' << std::put_time(&tmv, "%a %b %d %H:%M:%S %Y");
    return jxx::NEW<jxx::lang::String>(oss.str())->utf16();
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

static std::u16string readAllFromInputStreamLatin1(jxx::Ptr<jxx::io::InputStream> in) {
    std::u16string out;
    while (true) {
        const auto b = in->read();
        if (b < 0) break;
        out.push_back(static_cast<char16_t>(static_cast<unsigned char>(b & 0xFF)));
    }
    return out;
}

static std::u16string readAllFromInputStreamUtf8(jxx::Ptr<jxx::io::InputStream> in) {
    std::string bytes;
    while (true) {
        const auto b = in->read();
        if (b < 0) break;
        bytes.push_back(static_cast<char>(static_cast<unsigned char>(b & 0xFF)));
    }
    return jxx::NEW<jxx::lang::String>(bytes)->utf16();
}

static std::vector<std::u16string> toLogicalLines(const std::u16string& text) {
    std::vector<std::u16string> lines;
    std::u16string line;
    std::size_t i = 0;
    while (i < text.size()) {
        line.clear();
        while (i < text.size()) {
            char16_t c = text[i++];
            if (c == u'\r') {
                if (i < text.size() && text[i] == u'\n') ++i;
                break;
            }
            if (c == u'\n') break;
            line.push_back(c);
        }
        while (true) {
            std::size_t backslashes = 0;
            for (std::size_t p = line.size(); p > 0 && line[p - 1] == u'\\'; --p) ++backslashes;
            if ((backslashes % 2U) == 0U) break;
            if (!line.empty()) line.pop_back();
            while (i < text.size()) {
                char16_t c = text[i++];
                if (c == u'\r') {
                    if (i < text.size() && text[i] == u'\n') ++i;
                    break;
                }
                if (c == u'\n') break;
                line.push_back(c);
            }
            std::size_t skip = 0;
            while (skip < line.size() && isWs(line[skip])) ++skip;
            if (skip > 0) line.erase(0, skip);
        }
        lines.push_back(line);
    }
    if (text.empty()) lines.push_back(std::u16string());
    return lines;
}

static void loadIntoProperties(Properties* props, const std::u16string& text) {
    const auto lines = toLogicalLines(text);
    for (const auto& line : lines) {
        std::size_t start = 0;
        while (start < line.size() && isWs(line[start])) ++start;
        if (start >= line.size()) continue;
        if (line[start] == u'#' || line[start] == u'!') continue;

        std::size_t keyEnd = start;
        std::size_t valueStart = line.size();
        jxx::lang::jbool escaped = static_cast<jxx::lang::jbool>(false);
        while (keyEnd < line.size()) {
            const char16_t c = line[keyEnd];
            if ((c == u'=' || c == u':') && !escaped) {
                valueStart = keyEnd + 1;
                break;
            }
            if (isWs(c) && !escaped) {
                valueStart = keyEnd + 1;
                while (valueStart < line.size() && isWs(line[valueStart])) ++valueStart;
                if (valueStart < line.size() && (line[valueStart] == u'=' || line[valueStart] == u':')) ++valueStart;
                break;
            }
            if (c == u'\\') escaped = static_cast<jxx::lang::jbool>(!escaped);
            else escaped = static_cast<jxx::lang::jbool>(false);
            ++keyEnd;
        }
        while (valueStart < line.size() && isWs(line[valueStart])) ++valueStart;
        if (valueStart > line.size()) valueStart = line.size();

        const std::u16string keyRaw = line.substr(start, keyEnd - start);
        const std::u16string valueRaw = (valueStart <= line.size()) ? line.substr(valueStart) : std::u16string();
        props->setProperty(
            jxx::NEW<jxx::lang::String>(loadConvert(keyRaw)),
            jxx::NEW<jxx::lang::String>(loadConvert(valueRaw)));
    }
}

static std::u16string abbreviateForList(const std::u16string& in) {
    if (in.size() <= 40U) return in;
    return in.substr(0, 37U) + u"...";
}

static void writeCommentsToWriter(jxx::Ptr<jxx::io::Writer> writer, jxx::Ptr<jxx::lang::String> comments) {
    if (comments == nullptr) return;
    const std::u16string text = comments->utf16();
    static const char16_t hex[] = u"0123456789ABCDEF";
    std::u16string line(1, u'#');
    for (std::size_t i = 0; i < text.size(); ++i) {
        char16_t c = text[i];
        if (c == u'\r' || c == u'\n') {
            writer->write(jxx::NEW<jxx::lang::String>(line));
            writer->write(jxx::NEW<jxx::lang::String>(u"\n"));
            line.assign(1, u'#');
            if (c == u'\r' && i + 1 < text.size() && text[i + 1] == u'\n') ++i;
            continue;
        }
        if (c > 0x00FF) {
            line.append(u"\\u");
            line.push_back(hex[(c >> 12) & 0xF]);
            line.push_back(hex[(c >> 8) & 0xF]);
            line.push_back(hex[(c >> 4) & 0xF]);
            line.push_back(hex[c & 0xF]);
        } else {
            line.push_back(c);
        }
    }
    writer->write(jxx::NEW<jxx::lang::String>(line));
    writer->write(jxx::NEW<jxx::lang::String>(u"\n"));
}

static void writeAsciiToOutputStream(jxx::Ptr<jxx::io::OutputStream> out, const std::u16string& text) {
    for (char16_t c : text) {
        out->write(static_cast<jxx::lang::jint>(static_cast<unsigned char>(c & 0xFF)));
    }
}

static void writeCommentsToOutputStream(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments) {
    if (comments == nullptr) return;
    static const char16_t hex[] = u"0123456789ABCDEF";
    const std::u16string text = comments->utf16();
    std::u16string line(1, u'#');
    for (std::size_t i = 0; i < text.size(); ++i) {
        char16_t c = text[i];
        if (c == u'\r' || c == u'\n') {
            writeAsciiToOutputStream(out, line);
            writeAsciiToOutputStream(out, u"\n");
            line.assign(1, u'#');
            if (c == u'\r' && i + 1 < text.size() && text[i + 1] == u'\n') ++i;
            continue;
        }
        if (c > 0x00FF) {
            line.append(u"\\u");
            line.push_back(hex[(c >> 12) & 0xF]);
            line.push_back(hex[(c >> 8) & 0xF]);
            line.push_back(hex[(c >> 4) & 0xF]);
            line.push_back(hex[c & 0xF]);
        } else {
            line.push_back(c);
        }
    }
    writeAsciiToOutputStream(out, line);
    writeAsciiToOutputStream(out, u"\n");
}

static void writeOnePropertyToWriter(jxx::Ptr<jxx::io::Writer> writer, jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> value) {
    const auto k = saveConvert(key->utf16(), static_cast<jxx::lang::jbool>(true), static_cast<jxx::lang::jbool>(false));
    const auto v = saveConvert(value->utf16(), static_cast<jxx::lang::jbool>(false), static_cast<jxx::lang::jbool>(false));
    writer->write(std::make_shared<jxx::lang::String>(k + u"=" + v + u"\n"));
}

static void writeOnePropertyToOutputStream(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> value) {
    const auto k = saveConvert(key->utf16(), static_cast<jxx::lang::jbool>(true), static_cast<jxx::lang::jbool>(true));
    const auto v = saveConvert(value->utf16(), static_cast<jxx::lang::jbool>(false), static_cast<jxx::lang::jbool>(true));
    writeAsciiToOutputStream(out, k + u"=" + v + u"\n");
}

static std::u16string xmlEscape(const std::u16string& in, jxx::lang::jbool attributeMode) {
    std::u16string out;
    for (char16_t c : in) {
        switch (c) {
            case u'&': out.append(u"&amp;"); break;
            case u'<': out.append(u"&lt;"); break;
            case u'>': out.append(u"&gt;"); break;
            case u'\"': out.append(attributeMode ? u"&quot;" : std::u16string(1, c)); break;
            case u'\'': out.append(attributeMode ? u"&apos;" : std::u16string(1, c)); break;
            default: out.push_back(c); break;
        }
    }
    return out;
}

static std::u16string xmlUnescape(const std::u16string& in) {
    std::u16string out;
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] != u'&') {
            out.push_back(in[i]);
            continue;
        }
        std::size_t semi = in.find(u';', i + 1);
        if (semi == std::u16string::npos) {
            throw jxx::lang::IllegalArgumentException();
        }
        std::u16string entity = in.substr(i + 1, semi - i - 1);
        if (entity == u"amp") out.push_back(u'&');
        else if (entity == u"lt") out.push_back(u'<');
        else if (entity == u"gt") out.push_back(u'>');
        else if (entity == u"quot") out.push_back(u'\"');
        else if (entity == u"apos") out.push_back(u'\'');
        else if (!entity.empty() && entity[0] == u'#') {
            jxx::lang::jint code = 0;
            if (entity.size() > 1 && (entity[1] == u'x' || entity[1] == u'X')) {
                for (std::size_t p = 2; p < entity.size(); ++p) {
                    code = static_cast<jxx::lang::jint>((code << 4) + hexValue(entity[p]));
                }
            } else {
                for (std::size_t p = 1; p < entity.size(); ++p) {
                    if (entity[p] < u'0' || entity[p] > u'9') {
                        throw jxx::lang::IllegalArgumentException();
                    }
                    code = static_cast<jxx::lang::jint>(code * 10 + (entity[p] - u'0'));
                }
            }
            out.push_back(static_cast<char16_t>(code));
        } else {
            throw jxx::lang::IllegalArgumentException();
        }
        i = semi;
    }
    return out;
}

static jxx::lang::jbool startsWithAt(const std::u16string& xml, std::size_t pos, const std::u16string& token) {
    return static_cast<jxx::lang::jbool>(pos + token.size() <= xml.size() && xml.compare(pos, token.size(), token) == 0);
}

static void skipXmlWs(const std::u16string& xml, std::size_t& pos) {
    while (pos < xml.size() && (xml[pos] == u' ' || xml[pos] == u'\t' || xml[pos] == u'\r' || xml[pos] == u'\n')) ++pos;
}

static void requireToken(const std::u16string& xml, std::size_t& pos, const std::u16string& token) {
    if (!startsWithAt(xml, pos, token)) {
        throw jxx::lang::IllegalArgumentException();
    }
    pos += token.size();
}

static std::u16string parseUntil(const std::u16string& xml, std::size_t& pos, const std::u16string& endToken) {
    const std::size_t end = xml.find(endToken, pos);
    if (end == std::u16string::npos) {
        throw jxx::lang::IllegalArgumentException();
    }
    const std::u16string out = xml.substr(pos, end - pos);
    pos = end + endToken.size();
    return out;
}

static void parseXmlDeclarationAndDoctype(const std::u16string& xml, std::size_t& pos) {
    skipXmlWs(xml, pos);
    if (startsWithAt(xml, pos, u"<?xml")) {
        const std::size_t declEnd = xml.find(u"?>", pos);
        if (declEnd == std::u16string::npos) {
            throw jxx::lang::IllegalArgumentException();
        }
        pos = declEnd + 2;
    }
    skipXmlWs(xml, pos);
    if (startsWithAt(xml, pos, u"<!DOCTYPE")) {
        const std::size_t dtdEnd = xml.find(u'>', pos);
        if (dtdEnd == std::u16string::npos) {
            throw jxx::lang::IllegalArgumentException();
        }
        pos = dtdEnd + 1;
    }
    skipXmlWs(xml, pos);
}

static void loadXmlIntoProperties(Properties* props, const std::u16string& xml) {
    std::size_t pos = 0;
    parseXmlDeclarationAndDoctype(xml, pos);
    requireToken(xml, pos, u"<properties>");
    skipXmlWs(xml, pos);
    if (startsWithAt(xml, pos, u"<comment>")) {
        pos += 9;
        (void)xmlUnescape(parseUntil(xml, pos, u"</comment>"));
        skipXmlWs(xml, pos);
    }
    while (startsWithAt(xml, pos, u"<entry")) {
        requireToken(xml, pos, u"<entry");
        skipXmlWs(xml, pos);
        requireToken(xml, pos, u"key=");
        requireToken(xml, pos, u"\"");
        const std::size_t quoteEnd = xml.find(u'\"', pos);
        if (quoteEnd == std::u16string::npos) {
            throw jxx::lang::IllegalArgumentException();
        }
        const std::u16string keyEsc = xml.substr(pos, quoteEnd - pos);
        pos = quoteEnd + 1;
        skipXmlWs(xml, pos);
        requireToken(xml, pos, u">");
        const std::u16string valueEsc = parseUntil(xml, pos, u"</entry>");
        props->setProperty(jxx::NEW<jxx::lang::String>(xmlUnescape(keyEsc)), jxx::NEW<jxx::lang::String>(xmlUnescape(valueEsc)));
        skipXmlWs(xml, pos);
    }
    requireToken(xml, pos, u"</properties>");
    skipXmlWs(xml, pos);
    if (pos != xml.size()) {
        throw jxx::lang::IllegalArgumentException();
    }
}

static void writeXmlDocument(Properties* props, jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment, jxx::Ptr<jxx::lang::String> encoding) {
    const auto enc = (encoding == nullptr) ? jxx::NEW<jxx::lang::String>("UTF-8") : encoding;
    const auto encUtf16 = enc->utf16();
    const auto encUtf8 = enc->utf8();
    if (!(encUtf8 == "UTF-8" || encUtf8 == "UTF8" || encUtf8 == "utf-8" || encUtf8 == "utf8")) {
        throw jxx::lang::IllegalArgumentException();
    }
    writeAsciiToOutputStream(os, u"<?xml version=\"1.0\" encoding=\"");
    writeAsciiToOutputStream(os, encUtf16);
    writeAsciiToOutputStream(os, u"\"?>\n<!DOCTYPE properties SYSTEM \"http://java.sun.com/dtd/properties.dtd\">\n<properties>\n");
    if (comment != nullptr) {
        writeAsciiToOutputStream(os, u"  <comment>");
        writeAsciiToOutputStream(os, xmlEscape(comment->utf16(), static_cast<jxx::lang::jbool>(false)));
        writeAsciiToOutputStream(os, u"</comment>\n");
    }
    auto e = props->keys();
    while (e->hasMoreElements()) {
        auto keyObj = jxx::CAST<jxx::lang::Object>(e->nextElement());
        auto valueObj = jxx::CAST<jxx::lang::Object>(props->get(keyObj));
        auto keyStr = jxx::CAST<jxx::lang::String>(keyObj);
        auto valueStr = jxx::CAST<jxx::lang::String>(valueObj);
        if (keyStr == nullptr || valueStr == nullptr) continue;
        writeAsciiToOutputStream(os, u"  <entry key=\"");
        writeAsciiToOutputStream(os, xmlEscape(keyStr->utf16(), static_cast<jxx::lang::jbool>(true)));
        writeAsciiToOutputStream(os, u"\">");
        writeAsciiToOutputStream(os, xmlEscape(valueStr->utf16(), static_cast<jxx::lang::jbool>(false)));
        writeAsciiToOutputStream(os, u"</entry>\n");
    }
    writeAsciiToOutputStream(os, u"</properties>\n");
    os->flush();
}

} // anonymous namespace

Properties::Properties()
    : Hashtable<jxx::lang::Object, jxx::lang::Object>()
    , defaults(nullptr) {
}

Properties::Properties(jxx::Ptr<Properties> defaults_)
    : Hashtable<jxx::lang::Object, jxx::lang::Object>()
    , defaults(defaults_) {
}

jxx::Ptr<jxx::lang::Object> Properties::setProperty(jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> value) {
    if (key == nullptr || value == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    return this->put(jxx::CAST<jxx::lang::Object>(key), jxx::CAST<jxx::lang::Object>(value));
}

jxx::Ptr<jxx::lang::String> Properties::getProperty(jxx::Ptr<jxx::lang::String> key) {
    if (key == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    auto valueObj = jxx::CAST<jxx::lang::Object>(this->get(jxx::CAST<jxx::lang::Object>(key)));
    auto valueStr = jxx::CAST<jxx::lang::String>(valueObj);
    if (valueStr != nullptr) {
        return valueStr;
    }
    return defaults == nullptr ? nullptr : defaults->getProperty(key);
}

jxx::Ptr<jxx::lang::String> Properties::getProperty(jxx::Ptr<jxx::lang::String> key, jxx::Ptr<jxx::lang::String> defaultValue) {
    auto value = getProperty(key);
    return value == nullptr ? defaultValue : value;
}

void Properties::collectPropertyNames(std::vector<jxx::Ptr<jxx::lang::Object>>& out) {
    if (defaults != nullptr) {
        defaults->collectPropertyNames(out);
    }
    auto e = this->keys();
    while (e->hasMoreElements()) {
        auto key = jxx::CAST<jxx::lang::Object>(e->nextElement());
        if (!containsObjectVector(out, key)) {
            out.push_back(key);
        }
    }
}

void Properties::collectStringPropertyNames(std::vector<jxx::Ptr<jxx::lang::String>>& out) {
    if (defaults != nullptr) {
        defaults->collectStringPropertyNames(out);
    }
    auto e = this->keys();
    while (e->hasMoreElements()) {
        auto keyObj = jxx::CAST<jxx::lang::Object>(e->nextElement());
        auto valueObj = jxx::CAST<jxx::lang::Object>(this->get(keyObj));
        auto keyStr = jxx::CAST<jxx::lang::String>(keyObj);
        auto valueStr = jxx::CAST<jxx::lang::String>(valueObj);
        if (keyStr != nullptr && valueStr != nullptr && !containsStringVector(out, keyStr)) {
            out.push_back(keyStr);
        }
    }
}

jxx::Ptr<Enumeration<jxx::lang::Object>> Properties::propertyNames() {
    std::vector<jxx::Ptr<jxx::lang::Object>> names;
    collectPropertyNames(names);
    return std::make_shared<ObjectEnumerationImpl>(std::move(names));
}

jxx::Ptr<Set<jxx::lang::String>> Properties::stringPropertyNames() {
    std::vector<jxx::Ptr<jxx::lang::String>> names;
    collectStringPropertyNames(names);
    return std::make_shared<StringSetImpl>(std::move(names));
}

void Properties::list(jxx::Ptr<jxx::io::PrintStream> out) {
    if (out == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    out->println(jxx::NEW<jxx::lang::String>("-- listing properties --"));
    auto names = propertyNames();
    while (names->hasMoreElements()) {
        auto keyObj = names->nextElement();
        auto keyStr = jxx::CAST<jxx::lang::String>(keyObj);
        auto valueStr = keyStr == nullptr ? nullptr : getProperty(keyStr);
        if (keyStr == nullptr || valueStr == nullptr) {
            continue;
        }
        out->println(jxx::NEW<jxx::lang::String>(keyStr->utf16() + u"=" + abbreviateForList(valueStr->utf16())));
    }
}

void Properties::list(jxx::Ptr<jxx::io::PrintWriter> out) {
    if (out == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    out->println(std::u16string(u"-- listing properties --"));
    auto names = propertyNames();
    while (names->hasMoreElements()) {
        auto keyObj = names->nextElement();
        auto keyStr = jxx::CAST<jxx::lang::String>(keyObj);
        auto valueStr = keyStr == nullptr ? nullptr : getProperty(keyStr);
        if (keyStr == nullptr || valueStr == nullptr) {
            continue;
        }
        out->println(keyStr->utf16() + u"=" + abbreviateForList(valueStr->utf16()));
    }
}

void Properties::load(jxx::Ptr<jxx::io::Reader> reader) {
    if (reader == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    loadIntoProperties(this, readAllFromReader(reader));
}

void Properties::load(jxx::Ptr<jxx::io::InputStream> inStream) {
    if (inStream == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    loadIntoProperties(this, readAllFromInputStreamLatin1(inStream));
}

void Properties::save(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments) {
    if (out == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    store(out, comments);
}

void Properties::store(jxx::Ptr<jxx::io::Writer> writer, jxx::Ptr<jxx::lang::String> comments) {
    if (writer == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    writeCommentsToWriter(writer, comments);
    writer->write(jxx::NEW<jxx::lang::String>(formatDateLine()));
    writer->write(jxx::NEW<jxx::lang::String>(u"\n"));
    auto e = this->keys();
    while (e->hasMoreElements()) {
        auto keyObj = jxx::CAST<jxx::lang::Object>(e->nextElement());
        auto valueObj = jxx::CAST<jxx::lang::Object>(this->get(keyObj));
        auto keyStr = jxx::CAST<jxx::lang::String>(keyObj);
        auto valueStr = jxx::CAST<jxx::lang::String>(valueObj);
        if (keyStr != nullptr && valueStr != nullptr) {
            writeOnePropertyToWriter(writer, keyStr, valueStr);
        }
    }
    writer->flush();
}

void Properties::store(jxx::Ptr<jxx::io::OutputStream> out, jxx::Ptr<jxx::lang::String> comments) {
    if (out == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    writeCommentsToOutputStream(out, comments);
    writeAsciiToOutputStream(out, formatDateLine());
    writeAsciiToOutputStream(out, u"\n");
    auto e = this->keys();
    while (e->hasMoreElements()) {
        auto keyObj = jxx::CAST<jxx::lang::Object>(e->nextElement());
        auto valueObj = jxx::CAST<jxx::lang::Object>(this->get(keyObj));
        auto keyStr = jxx::CAST<jxx::lang::String>(keyObj);
        auto valueStr = jxx::CAST<jxx::lang::String>(valueObj);
        if (keyStr != nullptr && valueStr != nullptr) {
            writeOnePropertyToOutputStream(out, keyStr, valueStr);
        }
    }
    out->flush();
}

void Properties::loadFromXML(jxx::Ptr<jxx::io::InputStream> in) {
    if (in == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    loadXmlIntoProperties(this, readAllFromInputStreamUtf8(in));
}

void Properties::storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment) {
    if (os == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    writeXmlDocument(this, os, comment, jxx::NEW<jxx::lang::String>("UTF-8"));
}

void Properties::storeToXML(jxx::Ptr<jxx::io::OutputStream> os, jxx::Ptr<jxx::lang::String> comment, jxx::Ptr<jxx::lang::String> encoding) {
    if (os == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    writeXmlDocument(this, os, comment, encoding == nullptr ? jxx::NEW<jxx::lang::String>("UTF-8") : encoding);
}

} // namespace util
} // namespace jxx
