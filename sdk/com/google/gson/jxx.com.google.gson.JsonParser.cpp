#include "com/google/gson/jxx.com.google.gson.JsonParser.h"

#include <cctype>
#include <cstdlib>
#include <string>

#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "com/google/gson/jxx.com.google.gson.JsonSyntaxException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "io/jxx.io.Reader.h"

namespace com::google::gson {
namespace {

class Parser final {
public:
    explicit Parser(const std::string& source)
        : source_(source) {}

    ::jxx::Ptr<JsonElement> parse() {
        skipWhitespace_();
        const auto value = parseValue_();
        skipWhitespace_();
        if (position_ != source_.size()) fail_("Trailing JSON content");
        return value;
    }

private:
    ::jxx::Ptr<JsonElement> parseValue_() {
        if (position_ >= source_.size()) fail_("Expected JSON value");
        const char current = source_[position_];
        if (current == 'n') { consumeLiteral_("null"); return JsonNull::INSTANCE(); }
        if (current == 't') { consumeLiteral_("true"); return ::jxx::NEW<JsonPrimitive>(true); }
        if (current == 'f') { consumeLiteral_("false"); return ::jxx::NEW<JsonPrimitive>(false); }
        if (current == '"') return ::jxx::NEW<JsonPrimitive>(parseString_());
        if (current == '[') return parseArray_();
        if (current == '{') return parseObject_();
        if (current == '-' || std::isdigit(static_cast<unsigned char>(current))) return parseNumber_();
        fail_("Unexpected JSON token");
    }

    ::jxx::Ptr<JsonArray> parseArray_() {
        ++position_;
        const auto result = ::jxx::NEW<JsonArray>();
        skipWhitespace_();
        if (consumeIf_(']')) return result;
        for (;;) {
            skipWhitespace_();
            result->add(parseValue_());
            skipWhitespace_();
            if (consumeIf_(']')) return result;
            expect_(',');
        }
    }

    ::jxx::Ptr<JsonObject> parseObject_() {
        ++position_;
        const auto result = ::jxx::NEW<JsonObject>();
        skipWhitespace_();
        if (consumeIf_('}')) return result;
        for (;;) {
            skipWhitespace_();
            if (position_ >= source_.size() || source_[position_] != '"') {
                fail_("Expected object member name");
            }
            const auto name = parseString_();
            skipWhitespace_();
            expect_(':');
            skipWhitespace_();
            result->add(name, parseValue_());
            skipWhitespace_();
            if (consumeIf_('}')) return result;
            expect_(',');
        }
    }

    ::jxx::Ptr<JsonPrimitive> parseNumber_() {
        const std::size_t start = position_;
        if (consumeIf_('-') && position_ >= source_.size()) fail_("Invalid number");
        if (consumeIf_('0')) {
        } else {
            if (position_ >= source_.size() || !std::isdigit(static_cast<unsigned char>(source_[position_]))) fail_("Invalid number");
            while (position_ < source_.size() && std::isdigit(static_cast<unsigned char>(source_[position_]))) ++position_;
        }
        bool decimal = false;
        if (consumeIf_('.')) {
            decimal = true;
            if (position_ >= source_.size() || !std::isdigit(static_cast<unsigned char>(source_[position_]))) fail_("Invalid number");
            while (position_ < source_.size() && std::isdigit(static_cast<unsigned char>(source_[position_]))) ++position_;
        }
        if (position_ < source_.size() && (source_[position_] == 'e' || source_[position_] == 'E')) {
            decimal = true;
            ++position_;
            if (position_ < source_.size() && (source_[position_] == '+' || source_[position_] == '-')) ++position_;
            if (position_ >= source_.size() || !std::isdigit(static_cast<unsigned char>(source_[position_]))) fail_("Invalid exponent");
            while (position_ < source_.size() && std::isdigit(static_cast<unsigned char>(source_[position_]))) ++position_;
        }
        const auto token = source_.substr(start, position_ - start);
        try {
            if (decimal) return ::jxx::NEW<JsonPrimitive>(std::stod(token));
            return ::jxx::NEW<JsonPrimitive>(static_cast<::jxx::lang::jlong>(std::stoll(token)));
        } catch (...) {
            fail_("Number outside supported range");
        }
    }

    ::jxx::Ptr<::jxx::lang::String> parseString_() {
        expect_('"');
        std::string value;
        while (position_ < source_.size()) {
            const char current = source_[position_++];
            if (current == '"') return ::jxx::NEW<::jxx::lang::String>(value);
            if (static_cast<unsigned char>(current) < 0x20U) fail_("Control character in string");
            if (current != '\\') { value.push_back(current); continue; }
            if (position_ >= source_.size()) fail_("Incomplete escape");
            const char escaped = source_[position_++];
            switch (escaped) {
                case '"': value.push_back('"'); break;
                case '\\': value.push_back('\\'); break;
                case '/': value.push_back('/'); break;
                case 'b': value.push_back('\b'); break;
                case 'f': value.push_back('\f'); break;
                case 'n': value.push_back('\n'); break;
                case 'r': value.push_back('\r'); break;
                case 't': value.push_back('\t'); break;
                default: fail_("Unsupported string escape");
            }
        }
        fail_("Unterminated string");
    }

    void consumeLiteral_(const char* literal) {
        const std::string expected(literal);
        if (source_.compare(position_, expected.size(), expected) != 0) fail_("Invalid literal");
        position_ += expected.size();
    }
    void skipWhitespace_() { while (position_ < source_.size() && std::isspace(static_cast<unsigned char>(source_[position_]))) ++position_; }
    bool consumeIf_(char value) { if (position_ < source_.size() && source_[position_] == value) { ++position_; return true; } return false; }
    void expect_(char value) { if (!consumeIf_(value)) fail_("Unexpected JSON delimiter"); }
    [[noreturn]] void fail_(const char* message) const { throw JsonSyntaxException(::jxx::NEW<::jxx::lang::String>(message)); }

    const std::string& source_;
    std::size_t position_ = 0;
};

} // namespace

::jxx::Ptr<JsonElement> JsonParser::parseString(
    const ::jxx::Ptr<::jxx::lang::String>& json) {
    if (json == nullptr) throw ::jxx::lang::NullPointerException();
    return Parser(json->utf8()).parse();
}

::jxx::Ptr<JsonElement> JsonParser::parseReader(
    const ::jxx::Ptr<::jxx::io::Reader>& reader) {
    if (reader == nullptr) throw ::jxx::lang::NullPointerException();
    std::string text;
    for (;;) {
        const auto value = reader->read();
        if (value < 0) break;
        const auto code = static_cast<unsigned int>(value);
        if (code <= 0x7FU) text.push_back(static_cast<char>(code));
        else if (code <= 0x7FFU) {
            text.push_back(static_cast<char>(0xC0U | (code >> 6U)));
            text.push_back(static_cast<char>(0x80U | (code & 0x3FU)));
        } else {
            text.push_back(static_cast<char>(0xE0U | (code >> 12U)));
            text.push_back(static_cast<char>(0x80U | ((code >> 6U) & 0x3FU)));
            text.push_back(static_cast<char>(0x80U | (code & 0x3FU)));
        }
    }
    return parseString(::jxx::NEW<::jxx::lang::String>(text));
}

} // namespace com::google::gson
