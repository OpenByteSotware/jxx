#include "com/google/gson/jxx.com.google.gson.JsonStreamParser.h"

#include <cctype>

#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
#include "com/google/gson/jxx.com.google.gson.JsonSyntaxException.h"
#include "io/jxx.io.StringReader.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace com::google::gson {

JsonStreamParser::JsonStreamParser(
    const ::jxx::Ptr<::jxx::lang::String>& json)
    : reader_(json == nullptr
          ? nullptr
          : ::jxx::CAST<::jxx::io::Reader>(
                ::jxx::NEW<::jxx::io::StringReader>(json))) {
    if (reader_ == nullptr) throw ::jxx::lang::NullPointerException();
}

JsonStreamParser::JsonStreamParser(
    const ::jxx::Ptr<::jxx::io::Reader>& reader)
    : reader_(reader) {
    if (reader_ == nullptr) throw ::jxx::lang::NullPointerException();
}

void JsonStreamParser::load_() {
    if (loaded_) return;
    for (;;) {
        const auto value = reader_->read();
        if (value < 0) break;
        const auto code = static_cast<unsigned int>(value);
        if (code <= 0x7FU) source_.push_back(static_cast<char>(code));
        else if (code <= 0x7FFU) {
            source_.push_back(static_cast<char>(0xC0U | (code >> 6U)));
            source_.push_back(static_cast<char>(0x80U | (code & 0x3FU)));
        } else {
            source_.push_back(static_cast<char>(0xE0U | (code >> 12U)));
            source_.push_back(static_cast<char>(0x80U | ((code >> 6U) & 0x3FU)));
            source_.push_back(static_cast<char>(0x80U | (code & 0x3FU)));
        }
    }
    loaded_ = true;
}

void JsonStreamParser::skipWhitespace_() {
    load_();
    while (position_ < source_.size() &&
           std::isspace(static_cast<unsigned char>(source_[position_]))) ++position_;
}

::jxx::lang::jbool JsonStreamParser::hasNext() {
    skipWhitespace_();
    return position_ < source_.size();
}

std::string JsonStreamParser::nextValue_() {
    skipWhitespace_();
    if (position_ >= source_.size()) throw ::jxx::util::NoSuchElementException();
    const auto start = position_;
    const auto first = source_[position_];
    if (first == '"') {
        ++position_;
        bool escaped = false;
        while (position_ < source_.size()) {
            const auto current = source_[position_++];
            if (escaped) escaped = false;
            else if (current == '\\') escaped = true;
            else if (current == '"') return source_.substr(start, position_ - start);
        }
        throw JsonSyntaxException(::jxx::NEW<::jxx::lang::String>("Unterminated JSON string"));
    }
    if (first == '{' || first == '[') {
        const auto open = first;
        const auto close = open == '{' ? '}' : ']';
        int depth = 0;
        bool quoted = false;
        bool escaped = false;
        while (position_ < source_.size()) {
            const auto current = source_[position_++];
            if (quoted) {
                if (escaped) escaped = false;
                else if (current == '\\') escaped = true;
                else if (current == '"') quoted = false;
                continue;
            }
            if (current == '"') quoted = true;
            else if (current == open) ++depth;
            else if (current == close && --depth == 0) return source_.substr(start, position_ - start);
        }
        throw JsonSyntaxException(::jxx::NEW<::jxx::lang::String>("Unterminated JSON container"));
    }
    while (position_ < source_.size() &&
           !std::isspace(static_cast<unsigned char>(source_[position_]))) ++position_;
    return source_.substr(start, position_ - start);
}

::jxx::Ptr<JsonElement> JsonStreamParser::next() {
    return JsonParser::parseString(
        ::jxx::NEW<::jxx::lang::String>(nextValue_()));
}

void JsonStreamParser::remove() {
    throw ::jxx::lang::UnsupportedOperationException();
}

} // namespace com::google::gson
