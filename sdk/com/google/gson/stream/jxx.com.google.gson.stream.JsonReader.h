#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonToken.h"
#include "io/jxx.io.Closeable.h"
#include "io/jxx.io.Reader.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace com::google::gson::stream {

class JsonReader final
    : public ::jxx::lang::ClassBase<
          JsonReader,
          ::jxx::lang::Object,
          ::jxx::io::Closeable> {
public:
    explicit JsonReader(
        const ::jxx::Ptr<::jxx::io::Reader>& input);

    void setLenient(::jxx::lang::jbool value) noexcept;
    ::jxx::lang::jbool isLenient() const noexcept;

    void beginArray();
    void endArray();
    void beginObject();
    void endObject();
    ::jxx::lang::jbool hasNext();
    JsonToken peek();

    ::jxx::Ptr<::jxx::lang::String> nextName();
    void promoteNameToValue();
    ::jxx::Ptr<::jxx::lang::String> nextString();
    ::jxx::lang::jbool nextBoolean();
    void nextNull();
    ::jxx::lang::jdouble nextDouble();
    ::jxx::lang::jlong nextLong();
    ::jxx::lang::jint nextInt();
    void skipValue();
    ::jxx::Ptr<::jxx::lang::String> getPreviousPath() const;

    void close() override;
    ::jxx::Ptr<::jxx::lang::String> getPath() const;

private:
    enum class Scope { ARRAY, OBJECT };
    struct Frame {
        Scope scope;
        ::jxx::lang::jbool first;
        ::jxx::lang::jbool expectName;
        ::jxx::lang::jint index;
        std::string name;
    };

    void ensureOpen_() const;
    void readAll_();
    void skipWhitespace_();
    void beforeValue_();
    void afterValue_();
    char current_() const;
    char take_();
    std::string readString_();
    std::string readLiteral_();
    void expect_(char expected);
    void skipValueInternal_();

    ::jxx::Ptr<::jxx::io::Reader> input_;
    std::string json_;
    std::size_t position_ = 0U;
    std::vector<Frame> stack_;
    ::jxx::lang::jbool lenient_ = false;
    ::jxx::lang::jbool closed_ = false;
    ::jxx::lang::jbool loaded_ = false;
    std::string previousPath_;
    ::jxx::lang::jbool promotedName_ = false;
};

} // namespace com::google::gson::stream
