#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"

#include <cctype>
#include <cstdlib>
#include <limits>

#include "com/google/gson/stream/jxx.com.google.gson.stream.MalformedJsonException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx_types.h"

namespace com::google::gson::stream {
namespace {
[[noreturn]] void malformed(const char* message) {
    throw MalformedJsonException(
        ::jxx::NEW<::jxx::lang::String>(message));
}
}

JsonReader::JsonReader(
    const ::jxx::Ptr<::jxx::io::Reader>& input)
    : input_(input) {
    if (input_ == nullptr) throw ::jxx::lang::NullPointerException();
}

void JsonReader::setLenient(::jxx::lang::jbool value) noexcept { lenient_ = value; }
::jxx::lang::jbool JsonReader::isLenient() const noexcept { return lenient_; }

void JsonReader::ensureOpen_() const {
    if (closed_) throw ::jxx::lang::IllegalStateException();
}

void JsonReader::readAll_() {
    if (loaded_) return;
    ensureOpen_();
    for (;;) {
        const auto value = input_->read();
        if (value < 0) break;
        const auto code = static_cast<unsigned int>(value);
        if (code <= 0x7FU) json_.push_back(static_cast<char>(code));
        else if (code <= 0x7FFU) {
            json_.push_back(static_cast<char>(0xC0U | (code >> 6U)));
            json_.push_back(static_cast<char>(0x80U | (code & 0x3FU)));
        } else {
            json_.push_back(static_cast<char>(0xE0U | (code >> 12U)));
            json_.push_back(static_cast<char>(0x80U | ((code >> 6U) & 0x3FU)));
            json_.push_back(static_cast<char>(0x80U | (code & 0x3FU)));
        }
    }
    loaded_ = true;
}

void JsonReader::skipWhitespace_() {
    readAll_();
    while (position_ < json_.size() &&
           std::isspace(static_cast<unsigned char>(json_[position_]))) {
        ++position_;
    }
}

char JsonReader::current_() const {
    return position_ < json_.size() ? json_[position_] : '\0';
}

char JsonReader::take_() {
    if (position_ >= json_.size()) malformed("Unexpected end of JSON input");
    return json_[position_++];
}

void JsonReader::expect_(char expected) {
    skipWhitespace_();
    if (take_() != expected) malformed("Unexpected JSON token");
}

void JsonReader::beforeValue_() {
    skipWhitespace_();
    if (stack_.empty()) return;
    auto& frame = stack_.back();
    if (frame.scope == Scope::ARRAY) {
        if (!frame.first) {
            if (current_() == ',') { ++position_; skipWhitespace_(); }
            else malformed("Expected comma between array values");
        }
        frame.first = false;
    } else if (frame.expectName) {
        malformed("Expected a JSON object member name");
    }
}

void JsonReader::afterValue_() {
    if (stack_.empty()) return;
    auto& frame = stack_.back();
    if (frame.scope == Scope::ARRAY) ++frame.index;
    else frame.expectName = true;
}

std::string JsonReader::readString_() {
    skipWhitespace_();
    const auto quote = take_();
    if (quote != '"' && !(lenient_ && quote == '\'')) malformed("Expected a quoted JSON string");
    std::string result;
    while (position_ < json_.size()) {
        const auto current = take_();
        if (current == quote) return result;
        if (current != '\\') { result.push_back(current); continue; }
        const auto escaped = take_();
        switch (escaped) {
            case '"': result.push_back('"'); break;
            case '\\': result.push_back('\\'); break;
            case '/': result.push_back('/'); break;
            case 'b': result.push_back('\b'); break;
            case 'f': result.push_back('\f'); break;
            case 'n': result.push_back('\n'); break;
            case 'r': result.push_back('\r'); break;
            case 't': result.push_back('\t'); break;
            case 'u': {
                if (position_ + 4U > json_.size()) {
                    malformed("Incomplete JSON unicode escape");
                }
                unsigned int codePoint = 0U;
                for (int index = 0; index < 4; ++index) {
                    const auto digit = take_();
                    codePoint <<= 4U;
                    if (digit >= '0' && digit <= '9') codePoint |= static_cast<unsigned int>(digit - '0');
                    else if (digit >= 'a' && digit <= 'f') codePoint |= static_cast<unsigned int>(digit - 'a' + 10);
                    else if (digit >= 'A' && digit <= 'F') codePoint |= static_cast<unsigned int>(digit - 'A' + 10);
                    else malformed("Invalid JSON unicode escape");
                }
                if (codePoint >= 0xD800U && codePoint <= 0xDBFFU) {
                    if (position_ + 6U > json_.size() || take_() != '\\' || take_() != 'u') {
                        malformed("Missing JSON low surrogate");
                    }
                    unsigned int low = 0U;
                    for (int index = 0; index < 4; ++index) {
                        const auto digit = take_();
                        low <<= 4U;
                        if (digit >= '0' && digit <= '9') low |= static_cast<unsigned int>(digit - '0');
                        else if (digit >= 'a' && digit <= 'f') low |= static_cast<unsigned int>(digit - 'a' + 10);
                        else if (digit >= 'A' && digit <= 'F') low |= static_cast<unsigned int>(digit - 'A' + 10);
                        else malformed("Invalid JSON unicode escape");
                    }
                    if (low < 0xDC00U || low > 0xDFFFU) malformed("Invalid JSON low surrogate");
                    codePoint = 0x10000U + ((codePoint - 0xD800U) << 10U) + (low - 0xDC00U);
                } else if (codePoint >= 0xDC00U && codePoint <= 0xDFFFU) {
                    malformed("Unexpected JSON low surrogate");
                }
                if (codePoint <= 0x7FU) result.push_back(static_cast<char>(codePoint));
                else if (codePoint <= 0x7FFU) {
                    result.push_back(static_cast<char>(0xC0U | (codePoint >> 6U)));
                    result.push_back(static_cast<char>(0x80U | (codePoint & 0x3FU)));
                } else if (codePoint <= 0xFFFFU) {
                    result.push_back(static_cast<char>(0xE0U | (codePoint >> 12U)));
                    result.push_back(static_cast<char>(0x80U | ((codePoint >> 6U) & 0x3FU)));
                    result.push_back(static_cast<char>(0x80U | (codePoint & 0x3FU)));
                } else {
                    result.push_back(static_cast<char>(0xF0U | (codePoint >> 18U)));
                    result.push_back(static_cast<char>(0x80U | ((codePoint >> 12U) & 0x3FU)));
                    result.push_back(static_cast<char>(0x80U | ((codePoint >> 6U) & 0x3FU)));
                    result.push_back(static_cast<char>(0x80U | (codePoint & 0x3FU)));
                }
                break;
            }
            default: malformed("Unsupported JSON escape sequence");
        }
    }
    malformed("Unterminated JSON string");
}

std::string JsonReader::readLiteral_() {
    skipWhitespace_();
    const auto start = position_;
    while (position_ < json_.size()) {
        const auto current = json_[position_];
        if (std::isspace(static_cast<unsigned char>(current)) ||
            current == ',' || current == ']' || current == '}') break;
        ++position_;
    }
    if (start == position_) malformed("Expected JSON value");
    return json_.substr(start, position_ - start);
}

JsonToken JsonReader::peek() {
    ensureOpen_();
    skipWhitespace_();
    if (!stack_.empty()) {
        auto& frame = stack_.back();
        if (frame.scope == Scope::ARRAY) {
            auto probe = position_;
            while (probe < json_.size() && std::isspace(static_cast<unsigned char>(json_[probe]))) ++probe;
            if (!frame.first && probe < json_.size() && json_[probe] == ',') {
                ++probe;
                while (probe < json_.size() && std::isspace(static_cast<unsigned char>(json_[probe]))) ++probe;
            }
            if (probe < json_.size() && json_[probe] == ']') return JsonToken::END_ARRAY;
        } else {
            auto probe = position_;
            while (probe < json_.size() && std::isspace(static_cast<unsigned char>(json_[probe]))) ++probe;
            if (frame.expectName) {
                if (!frame.first && probe < json_.size() && json_[probe] == ',') {
                    ++probe;
                    while (probe < json_.size() && std::isspace(static_cast<unsigned char>(json_[probe]))) ++probe;
                }
                if (probe < json_.size() && json_[probe] == '}') return JsonToken::END_OBJECT;
                return JsonToken::NAME;
            }
        }
    }
    const auto current = current_();
    if (current == '\0') return JsonToken::END_DOCUMENT;
    if (current == '{') return JsonToken::BEGIN_OBJECT;
    if (current == '[') return JsonToken::BEGIN_ARRAY;
    if (current == '"' || (lenient_ && current == '\'')) return JsonToken::STRING;
    if (json_.compare(position_, 4U, "true") == 0 || json_.compare(position_, 5U, "false") == 0) return JsonToken::BOOLEAN;
    if (json_.compare(position_, 4U, "null") == 0) return JsonToken::NULL_VALUE;
    return JsonToken::NUMBER;
}

void JsonReader::beginArray() {
    beforeValue_();
    expect_('[');
    stack_.push_back({Scope::ARRAY, true, false, 0, ""});
}

void JsonReader::endArray() {
    if (stack_.empty() || stack_.back().scope != Scope::ARRAY) malformed("Not in a JSON array");
    skipWhitespace_();
    if (current_() == ',') { ++position_; skipWhitespace_(); }
    expect_(']');
    stack_.pop_back();
    afterValue_();
}

void JsonReader::beginObject() {
    beforeValue_();
    expect_('{');
    stack_.push_back({Scope::OBJECT, true, true, 0, ""});
}

void JsonReader::endObject() {
    if (stack_.empty() || stack_.back().scope != Scope::OBJECT) malformed("Not in a JSON object");
    skipWhitespace_();
    if (current_() == ',') { ++position_; skipWhitespace_(); }
    expect_('}');
    stack_.pop_back();
    afterValue_();
}

::jxx::lang::jbool JsonReader::hasNext() {
    const auto token = peek();
    return token != JsonToken::END_ARRAY &&
           token != JsonToken::END_OBJECT &&
           token != JsonToken::END_DOCUMENT;
}

::jxx::Ptr<::jxx::lang::String> JsonReader::nextName() {
    previousPath_ = getPath()->utf8();
    if (stack_.empty() || stack_.back().scope != Scope::OBJECT || !stack_.back().expectName) {
        malformed("Expected a JSON object member name");
    }
    auto& frame = stack_.back();
    skipWhitespace_();
    if (!frame.first) {
        if (current_() == ',') { ++position_; skipWhitespace_(); }
        else malformed("Expected comma between object members");
    }
    frame.name = readString_();
    skipWhitespace_();
    expect_(':');
    frame.first = false;
    frame.expectName = false;
    return ::jxx::NEW<::jxx::lang::String>(frame.name);
}

void JsonReader::promoteNameToValue() {
    ensureOpen_();
    if (stack_.empty() || stack_.back().scope != Scope::OBJECT || !stack_.back().expectName) {
        malformed("Expected a JSON name");
    }
    previousPath_ = getPath()->utf8();
    auto& frame = stack_.back();
    skipWhitespace_();
    if (!frame.first) {
        if (current_() == ',') { ++position_; skipWhitespace_(); }
        else malformed("Expected comma between object members");
    }
    frame.name = readString_();
    skipWhitespace_();
    expect_(':');
    frame.first = false;
    frame.expectName = false;
    promotedName_ = true;
}

::jxx::Ptr<::jxx::lang::String> JsonReader::nextString() {
    if (promotedName_) {
        promotedName_ = false;
        return ::jxx::NEW<::jxx::lang::String>(stack_.back().name);
    }
    beforeValue_();
    const auto token = peek();
    std::string value;
    if (token == JsonToken::STRING) value = readString_();
    else if (token == JsonToken::NUMBER || token == JsonToken::BOOLEAN) value = readLiteral_();
    else malformed("Expected a string-compatible JSON value");
    afterValue_();
    return ::jxx::NEW<::jxx::lang::String>(value);
}

::jxx::lang::jbool JsonReader::nextBoolean() {
    beforeValue_();
    const auto value = readLiteral_();
    if (value != "true" && value != "false") malformed("Expected a JSON boolean");
    afterValue_();
    return value == "true";
}

void JsonReader::nextNull() {
    beforeValue_();
    if (readLiteral_() != "null") malformed("Expected JSON null");
    afterValue_();
}

::jxx::lang::jdouble JsonReader::nextDouble() {
    const auto value = nextString()->utf8();
    char* end = nullptr;
    const auto result = std::strtod(value.c_str(), &end);
    if (end == value.c_str() || *end != '\0') malformed("Expected a JSON number");
    return result;
}

::jxx::lang::jfloat JsonReader::nextFloat() { return static_cast<::jxx::lang::jfloat>(nextDouble()); }
::jxx::lang::jshort JsonReader::nextShort() { const auto value=nextLong(); if(value<std::numeric_limits<::jxx::lang::jshort>::min()||value>std::numeric_limits<::jxx::lang::jshort>::max()) malformed("JSON short is out of range"); return static_cast<::jxx::lang::jshort>(value); }
::jxx::lang::jbyte JsonReader::nextByte() { const auto value=nextLong(); if(value<std::numeric_limits<::jxx::lang::jbyte>::min()||value>std::numeric_limits<::jxx::lang::jbyte>::max()) malformed("JSON byte is out of range"); return static_cast<::jxx::lang::jbyte>(value); }

::jxx::lang::jlong JsonReader::nextLong() {
    const auto value = nextString()->utf8();
    char* end = nullptr;
    const auto result = std::strtoll(value.c_str(), &end, 10);
    if (end == value.c_str() || *end != '\0') malformed("Expected a JSON integer");
    return static_cast<::jxx::lang::jlong>(result);
}

::jxx::lang::jint JsonReader::nextInt() {
    const auto value = nextLong();
    if (value < std::numeric_limits<::jxx::lang::jint>::min() ||
        value > std::numeric_limits<::jxx::lang::jint>::max()) {
        malformed("JSON integer is out of range");
    }
    return static_cast<::jxx::lang::jint>(value);
}

void JsonReader::skipValueInternal_() {
    const auto token = peek();
    if (token == JsonToken::BEGIN_ARRAY) {
        beginArray();
        while (hasNext()) skipValueInternal_();
        endArray();
    } else if (token == JsonToken::BEGIN_OBJECT) {
        beginObject();
        while (hasNext()) { nextName(); skipValueInternal_(); }
        endObject();
    } else if (token == JsonToken::NULL_VALUE) nextNull();
    else if (token == JsonToken::BOOLEAN) { (void)nextBoolean(); }
    else { (void)nextString(); }
}

void JsonReader::skipValue() { skipValueInternal_(); }

void JsonReader::close() {
    if (!closed_) {
        input_->close();
        closed_ = true;
        stack_.clear();
        json_.clear();
    }
}

::jxx::Ptr<::jxx::lang::String> JsonReader::getPreviousPath() const {
    return ::jxx::NEW<::jxx::lang::String>(previousPath_.empty() ? "$" : previousPath_);
}

::jxx::Ptr<::jxx::lang::String> JsonReader::getPath() const {
    std::string path = "$";
    for (const auto& frame : stack_) {
        if (frame.scope == Scope::ARRAY) path += "[" + std::to_string(frame.index) + "]";
        else if (!frame.name.empty()) path += "." + frame.name;
    }
    return ::jxx::NEW<::jxx::lang::String>(path);
}

} // namespace com::google::gson::stream
