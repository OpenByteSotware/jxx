#include <gtest/gtest.h>
#include <vector>

#include "io/jxx.io.BufferedOutputStream.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {

class RecordingOutputStream final
    : public ::jxx::lang::ClassBase<
          RecordingOutputStream,
          ::jxx::io::OutputStream> {
public:
    void write(::jxx::lang::jint value) override {
        ensureOpen_();
        bytes_.push_back(static_cast<::jxx::lang::jbyte>(value));
    }

    void write(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override {
        ensureOpen_();
        for (::jxx::lang::jint index = 0; index < length; ++index) {
            bytes_.push_back((*buffer)[offset + index]);
        }
    }

    void flush() override {
        ensureOpen_();
        ++flushCount_;
    }

    void close() override {
        ++closeCount_;
        closed_ = true;
    }

    ::jxx::lang::jint size() const noexcept {
        return static_cast<::jxx::lang::jint>(bytes_.size());
    }

    ::jxx::lang::jint flushCount() const noexcept { return flushCount_; }
    ::jxx::lang::jint closeCount() const noexcept { return closeCount_; }
    ::jxx::lang::jbool isClosed() const noexcept { return closed_; }

private:
    void ensureOpen_() const {
        if (closed_) throw ::jxx::io::IOException();
    }

    std::vector<::jxx::lang::jbyte> bytes_;
    ::jxx::lang::jint flushCount_ = 0;
    ::jxx::lang::jint closeCount_ = 0;
    ::jxx::lang::jbool closed_ = false;
};

TEST(BufferedOutputStreamCloseTest, CloseFlushesPendingBytesAndClosesDelegate) {
    const auto delegate = ::jxx::NEW<RecordingOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(delegate, 8);

    stream->write(0x41);
    stream->write(0x42);
    EXPECT_EQ(0, delegate->size());

    stream->close();

    EXPECT_EQ(2, delegate->size());
    EXPECT_EQ(1, delegate->flushCount());
    EXPECT_EQ(1, delegate->closeCount());
    EXPECT_TRUE(delegate->isClosed());
}

TEST(BufferedOutputStreamCloseTest, RepeatedCloseIsIdempotent) {
    const auto delegate = ::jxx::NEW<RecordingOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(delegate, 8);

    stream->write(0x41);
    stream->close();
    stream->close();

    EXPECT_EQ(1, delegate->closeCount());
    EXPECT_EQ(1, delegate->flushCount());
    EXPECT_EQ(1, delegate->size());
}

TEST(BufferedOutputStreamCloseTest, SingleByteWriteAfterCloseThrowsIOException) {
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(
        ::jxx::NEW<RecordingOutputStream>(),
        8);
    stream->close();

    EXPECT_THROW(stream->write(0x41), ::jxx::io::IOException);
}

TEST(BufferedOutputStreamCloseTest, ArrayWriteAfterCloseThrowsIOException) {
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(
        ::jxx::NEW<RecordingOutputStream>(),
        8);
    const auto bytes =
        std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(2);
    (*bytes)[0] = 1;
    (*bytes)[1] = 2;
    stream->close();

    EXPECT_THROW(stream->write(bytes, 0, 2), ::jxx::io::IOException);
}

TEST(BufferedOutputStreamCloseTest, FlushAfterCloseThrowsIOException) {
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(
        ::jxx::NEW<RecordingOutputStream>(),
        8);
    stream->close();

    EXPECT_THROW(stream->flush(), ::jxx::io::IOException);
}

} // namespace
