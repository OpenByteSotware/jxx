#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonWriter.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace com::google::gson::stream {
namespace {
std::string hexEscape(unsigned int value) {
    std::ostringstream stream;
    stream << "\\u" << std::uppercase << std::hex << std::setw(4)
           << std::setfill('0') << value;
    return stream.str();
}
}

JsonWriter::JsonWriter(const ::jxx::Ptr<::jxx::io::Writer>& output)
    : output_(output) {
    if (output_ == nullptr) throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<JsonWriter> JsonWriter::self_() {
    return ::jxx::CAST<JsonWriter>(this->thisPtr());
}

void JsonWriter::setIndent(const ::jxx::Ptr<::jxx::lang::String>& indent) {
    indent_ = indent == nullptr ? "" : indent->utf8();
}
void JsonWriter::setLenient(::jxx::lang::jbool value) noexcept { lenient_ = value; }
::jxx::lang::jbool JsonWriter::isLenient() const noexcept { return lenient_; }
void JsonWriter::setHtmlSafe(::jxx::lang::jbool value) noexcept { htmlSafe_ = value; }
::jxx::lang::jbool JsonWriter::isHtmlSafe() const noexcept { return htmlSafe_; }
void JsonWriter::setSerializeNulls(::jxx::lang::jbool value) noexcept { serializeNulls_ = value; }
::jxx::lang::jbool JsonWriter::getSerializeNulls() const noexcept { return serializeNulls_; }

void JsonWriter::ensureOpen_() const {
    if (closed_) throw ::jxx::lang::IllegalStateException();
}

void JsonWriter::writeRaw_(const std::string& value) {
    output_->write(::jxx::NEW<::jxx::lang::String>(value));
}

void JsonWriter::newline_() {
    if (indent_.empty()) return;
    writeRaw_("\n");
    for (std::size_t index = 0; index < stack_.size(); ++index) writeRaw_(indent_);
}

void JsonWriter::beforeName_() {
    ensureOpen_();
    if (stack_.empty() || stack_.back().scope != Scope::OBJECT || !stack_.back().expectName) {
        throw ::jxx::lang::IllegalStateException();
    }
    auto& frame = stack_.back();
    if (!frame.first) writeRaw_(",");
    newline_();
    frame.first = false;
    frame.expectName = false;
}

void JsonWriter::beforeValue_() {
    ensureOpen_();
    if (stack_.empty()) {
        if (rootWritten_ && !lenient_) throw ::jxx::lang::IllegalStateException();
        rootWritten_ = true;
        return;
    }
    auto& frame = stack_.back();
    if (frame.scope == Scope::ARRAY) {
        if (!frame.first) writeRaw_(",");
        newline_();
        frame.first = false;
        return;
    }
    if (frame.expectName || !deferredName_) throw ::jxx::lang::IllegalStateException();
    beforeName_();
    writeString_(name_);
    writeRaw_(indent_.empty() ? ":" : ": ");
    deferredName_ = false;
    name_.clear();
}

void JsonWriter::afterValue_() {
    if (!stack_.empty() && stack_.back().scope == Scope::OBJECT) {
        stack_.back().expectName = true;
    }
}

void JsonWriter::writeString_(const std::string& value) {
    writeRaw_("\"");
    std::string fragment;
    for (unsigned char current : value) {
        fragment.clear();
        switch (current) {
            case '"': fragment = "\\\""; break;
            case '\\': fragment = "\\\\"; break;
            case '\b': fragment = "\\b"; break;
            case '\f': fragment = "\\f"; break;
            case '\n': fragment = "\\n"; break;
            case '\r': fragment = "\\r"; break;
            case '\t': fragment = "\\t"; break;
            default:
                if (current < 0x20U ||
                    (htmlSafe_ && (current == '<' || current == '>' || current == '&' || current == '=' || current == '\''))) {
                    fragment = hexEscape(current);
                } else {
                    fragment.push_back(static_cast<char>(current));
                }
        }
        writeRaw_(fragment);
    }
    writeRaw_("\"");
}

::jxx::Ptr<JsonWriter> JsonWriter::beginArray() {
    beforeValue_();
    writeRaw_("[");
    stack_.push_back({Scope::ARRAY, true, false});
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::endArray() {
    ensureOpen_();
    if (stack_.empty() || stack_.back().scope != Scope::ARRAY || deferredName_) {
        throw ::jxx::lang::IllegalStateException();
    }
    const auto empty = stack_.back().first;
    stack_.pop_back();
    if (!empty) newline_();
    writeRaw_("]");
    afterValue_();
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::beginObject() {
    beforeValue_();
    writeRaw_("{");
    stack_.push_back({Scope::OBJECT, true, true});
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::endObject() {
    ensureOpen_();
    if (stack_.empty() || stack_.back().scope != Scope::OBJECT || deferredName_ || !stack_.back().expectName) {
        throw ::jxx::lang::IllegalStateException();
    }
    const auto empty = stack_.back().first;
    stack_.pop_back();
    if (!empty) newline_();
    writeRaw_("}");
    afterValue_();
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::name(
    const ::jxx::Ptr<::jxx::lang::String>& nameValue) {
    ensureOpen_();
    if (nameValue == nullptr) throw ::jxx::lang::NullPointerException();
    if (deferredName_ || stack_.empty() || stack_.back().scope != Scope::OBJECT || !stack_.back().expectName) {
        throw ::jxx::lang::IllegalStateException();
    }
    name_ = nameValue->utf8();
    deferredName_ = true;
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::value(
    const ::jxx::Ptr<::jxx::lang::String>& valueString) {
    if (valueString == nullptr) return nullValue();
    beforeValue_();
    writeString_(valueString->utf8());
    afterValue_();
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::value(::jxx::lang::jchar valueCharacter) {
    return value(::jxx::NEW<::jxx::lang::String>(std::u16string(1U, valueCharacter)));
}

::jxx::Ptr<JsonWriter> JsonWriter::value(::jxx::lang::jbool valueBoolean) {
    beforeValue_();
    writeRaw_(valueBoolean ? "true" : "false");
    afterValue_();
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::value(::jxx::lang::jdouble valueDouble) {
    if (!lenient_ && !std::isfinite(valueDouble)) throw ::jxx::lang::IllegalStateException();
    std::ostringstream stream;
    stream << std::setprecision(17) << valueDouble;
    beforeValue_();
    writeRaw_(stream.str());
    afterValue_();
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::value(::jxx::lang::jlong valueLong) {
    beforeValue_();
    writeRaw_(std::to_string(valueLong));
    afterValue_();
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::value(::jxx::lang::jint valueInt) {
    return value(static_cast<::jxx::lang::jlong>(valueInt));
}

::jxx::Ptr<JsonWriter> JsonWriter::value(::jxx::lang::jfloat valueFloat) {
    return value(static_cast<::jxx::lang::jdouble>(valueFloat));
}

::jxx::Ptr<JsonWriter> JsonWriter::nullValue() {
    if (deferredName_ && !serializeNulls_) {
        deferredName_ = false;
        name_.clear();
        return self_();
    }
    beforeValue_();
    writeRaw_("null");
    afterValue_();
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::jsonValue(
    const ::jxx::Ptr<::jxx::lang::String>& valueString) {
    if (valueString == nullptr) return nullValue();
    beforeValue_();
    writeRaw_(valueString->utf8());
    afterValue_();
    return self_();
}

::jxx::Ptr<JsonWriter> JsonWriter::cancelName() {
    ensureOpen_();
    if (!deferredName_) throw ::jxx::lang::IllegalStateException();
    deferredName_ = false;
    name_.clear();
    return self_();
}

void JsonWriter::flush() {
    ensureOpen_();
    output_->flush();
}

void JsonWriter::close() {
    if (closed_) return;
    if (!stack_.empty() || deferredName_) throw ::jxx::lang::IllegalStateException();
    output_->close();
    closed_ = true;
}

} // namespace com::google::gson::stream
