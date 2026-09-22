#pragma once

#include <string>
#include <vector>

#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.Flushable.h"
#include "io/jxx.io.Writer.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace com::google::gson::stream {

class JsonWriter final
    : public ::jxx::lang::ClassBase<
          JsonWriter,
          ::jxx::lang::Object,
          ::jxx::io::Closeable,
          ::jxx::io::Flushable> {
public:
    explicit JsonWriter(const ::jxx::Ptr<::jxx::io::Writer>& output);

    void setIndent(const ::jxx::Ptr<::jxx::lang::String>& indent);
    void setLenient(::jxx::lang::jbool value) noexcept;
    ::jxx::lang::jbool isLenient() const noexcept;
    void setHtmlSafe(::jxx::lang::jbool value) noexcept;
    ::jxx::lang::jbool isHtmlSafe() const noexcept;
    void setSerializeNulls(::jxx::lang::jbool value) noexcept;
    ::jxx::lang::jbool getSerializeNulls() const noexcept;

    ::jxx::Ptr<JsonWriter> beginArray();
    ::jxx::Ptr<JsonWriter> endArray();
    ::jxx::Ptr<JsonWriter> beginObject();
    ::jxx::Ptr<JsonWriter> endObject();
    ::jxx::Ptr<JsonWriter> name(const ::jxx::Ptr<::jxx::lang::String>& name);
    ::jxx::Ptr<JsonWriter> value(const ::jxx::Ptr<::jxx::lang::String>& value);
    ::jxx::Ptr<JsonWriter> value(::jxx::lang::jbool value);
    ::jxx::Ptr<JsonWriter> value(::jxx::lang::jchar value);
    ::jxx::Ptr<JsonWriter> value(::jxx::lang::jdouble value);
    ::jxx::Ptr<JsonWriter> value(::jxx::lang::jlong value);
    ::jxx::Ptr<JsonWriter> value(::jxx::lang::jint value);
    ::jxx::Ptr<JsonWriter> value(::jxx::lang::jfloat value);
    ::jxx::Ptr<JsonWriter> nullValue();
    ::jxx::Ptr<JsonWriter> jsonValue(const ::jxx::Ptr<::jxx::lang::String>& value);
    ::jxx::Ptr<JsonWriter> cancelName();

    void flush() override;
    void close() override;

private:
    enum class Scope { ARRAY, OBJECT };
    struct Frame {
        Scope scope;
        ::jxx::lang::jbool first;
        ::jxx::lang::jbool expectName;
    };

    void ensureOpen_() const;
    void beforeName_();
    void beforeValue_();
    void afterValue_();
    void newline_();
    void writeRaw_(const std::string& value);
    void writeString_(const std::string& value);
    ::jxx::Ptr<JsonWriter> self_();

    ::jxx::Ptr<::jxx::io::Writer> output_;
    std::vector<Frame> stack_;
    std::string indent_;
    ::jxx::lang::jbool lenient_ = false;
    ::jxx::lang::jbool htmlSafe_ = false;
    ::jxx::lang::jbool serializeNulls_ = true;
    ::jxx::lang::jbool closed_ = false;
    ::jxx::lang::jbool rootWritten_ = false;
    ::jxx::lang::jbool deferredName_ = false;
    std::string name_;
};

} // namespace com::google::gson::stream
