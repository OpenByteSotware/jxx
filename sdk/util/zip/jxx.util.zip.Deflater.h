#pragma once

#include <zlib.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::util::zip {

class Deflater final : public ::jxx::lang::ClassBase<Deflater, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Deflater, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static constexpr ::jxx::lang::jint DEFLATED = 8;
    static constexpr ::jxx::lang::jint NO_COMPRESSION = 0;
    static constexpr ::jxx::lang::jint BEST_SPEED = 1;
    static constexpr ::jxx::lang::jint BEST_COMPRESSION = 9;
    static constexpr ::jxx::lang::jint DEFAULT_COMPRESSION = -1;
    static constexpr ::jxx::lang::jint FILTERED = 1;
    static constexpr ::jxx::lang::jint HUFFMAN_ONLY = 2;
    static constexpr ::jxx::lang::jint DEFAULT_STRATEGY = 0;
    static constexpr ::jxx::lang::jint NO_FLUSH = 0;
    static constexpr ::jxx::lang::jint SYNC_FLUSH = 2;
    static constexpr ::jxx::lang::jint FULL_FLUSH = 3;

    Deflater();
    explicit Deflater(::jxx::lang::jint level);
    Deflater(::jxx::lang::jint level, ::jxx::lang::jbool nowrap);
    ~Deflater() override;

    void setInput(const ::jxx::lang::ByteArray& input);
    void setInput(const ::jxx::lang::ByteArray& input, ::jxx::lang::jint offset, ::jxx::lang::jint length);
    void setDictionary(const ::jxx::lang::ByteArray& dictionary);
    void setDictionary(const ::jxx::lang::ByteArray& dictionary, ::jxx::lang::jint offset, ::jxx::lang::jint length);
    void setStrategy(::jxx::lang::jint strategy);
    void setLevel(::jxx::lang::jint level);
    ::jxx::lang::jbool needsInput() const noexcept;
    void finish() noexcept;
    ::jxx::lang::jbool finished() const noexcept;
    ::jxx::lang::jint deflate(const ::jxx::lang::ByteArray& output);
    ::jxx::lang::jint deflate(const ::jxx::lang::ByteArray& output, ::jxx::lang::jint offset, ::jxx::lang::jint length);
    ::jxx::lang::jint deflate(const ::jxx::lang::ByteArray& output, ::jxx::lang::jint offset, ::jxx::lang::jint length, ::jxx::lang::jint flush);
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
    void applyParameters_();

    z_stream stream_{};
    ::jxx::lang::ByteArray input_;
    ::jxx::lang::jint inputOffset_ = 0;
    ::jxx::lang::jint inputLength_ = 0;
    ::jxx::lang::jint level_ = DEFAULT_COMPRESSION;
    ::jxx::lang::jint strategy_ = DEFAULT_STRATEGY;
    bool finishRequested_ = false;
    bool finished_ = false;
    bool ended_ = false;
    bool parametersDirty_ = false;
    bool nowrap_ = false;
};

} // namespace jxx::util::zip
