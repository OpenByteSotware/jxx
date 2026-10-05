#pragma once

#include <zlib.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::util::zip {
class DataFormatException;

class Inflater final : public ::jxx::lang::ClassBase<Inflater, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Inflater, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    Inflater();
    explicit Inflater(::jxx::lang::jbool nowrap);
    ~Inflater() override;

    void setInput(const ::jxx::lang::ByteArray& input);
    void setInput(const ::jxx::lang::ByteArray& input, ::jxx::lang::jint offset, ::jxx::lang::jint length);
    void setDictionary(const ::jxx::lang::ByteArray& dictionary);
    void setDictionary(const ::jxx::lang::ByteArray& dictionary, ::jxx::lang::jint offset, ::jxx::lang::jint length);
    ::jxx::lang::jint getRemaining() const noexcept;
    ::jxx::lang::jbool needsInput() const noexcept;
    ::jxx::lang::jbool needsDictionary() const noexcept;
    ::jxx::lang::jbool finished() const noexcept;
    ::jxx::lang::jint inflate(const ::jxx::lang::ByteArray& output);
    ::jxx::lang::jint inflate(const ::jxx::lang::ByteArray& output, ::jxx::lang::jint offset, ::jxx::lang::jint length);
    ::jxx::lang::jint getAdler() const;
    ::jxx::lang::jint getTotalIn() const noexcept;
    ::jxx::lang::jlong getBytesRead() const noexcept;
    ::jxx::lang::jint getTotalOut() const noexcept;
    ::jxx::lang::jlong getBytesWritten() const noexcept;
    void reset();
    void end() noexcept;

private:
    void ensureOpen_() const;
    static void checkRange_(const ::jxx::lang::ByteArray& array, ::jxx::lang::jint offset, ::jxx::lang::jint length);

    z_stream stream_{};
    ::jxx::lang::ByteArray input_;
    ::jxx::lang::jint inputOffset_ = 0;
    ::jxx::lang::jint inputLength_ = 0;
    bool dictionaryNeeded_ = false;
    bool finished_ = false;
    bool ended_ = false;
    bool nowrap_ = false;
};

} // namespace jxx::util::zip
