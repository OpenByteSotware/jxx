#include <gtest/gtest.h>

#include <vector>

#include "io/jxx.io.BufferedOutputStream.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {

class FailingOutputStream final
    : public ::jxx::lang::ClassBase<
          FailingOutputStream,
          ::jxx::io::OutputStream> {
public:
    void failNextWrite() noexcept { failWrite_ = true; }
    void failNextFlush() noexcept { failFlush_ = true; }
    void failNextClose() noexcept { failClose_ = true; }

    void write(::jxx::lang::jint value) override {
        if (failWrite_) {
            failWrite_ = false;
            ++writeFailureCount_;
            throw ::jxx::io::IOException("write failure");
        }
        bytes_.push_back(static_cast<::jxx::lang::jbyte>(value));
    }

    void write(
        const ::jxx::lang::ByteArray& buffer,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override {
        if (failWrite_) {
            failWrite_ = false;
            ++writeFailureCount_;
            throw ::jxx::io::IOException("write failure");
        }
        for (::jxx::lang::jint index = 0; index < length; ++index) {
            bytes_.push_back((*buffer)[offset + index]);
        }
    }

    void flush() override {
        ++flushAttemptCount_;
        if (failFlush_) {
            failFlush_ = false;
            ++flushFailureCount_;
            throw ::jxx::io::IOException("flush failure");
        }
    }

    void close() override {
        ++closeAttemptCount_;
        if (failClose_) {
            failClose_ = false;
            ++closeFailureCount_;
            throw ::jxx::io::IOException("close failure");
        }
        closed_ = true;
    }

    ::jxx::lang::jint size() const noexcept {
        return static_cast<::jxx::lang::jint>(bytes_.size());
    }
    ::jxx::lang::jint writeFailureCount() const noexcept { return writeFailureCount_; }
    ::jxx::lang::jint flushAttemptCount() const noexcept { return flushAttemptCount_; }
    ::jxx::lang::jint flushFailureCount() const noexcept { return flushFailureCount_; }
    ::jxx::lang::jint closeAttemptCount() const noexcept { return closeAttemptCount_; }
    ::jxx::lang::jint closeFailureCount() const noexcept { return closeFailureCount_; }
    ::jxx::lang::jbool isClosed() const noexcept { return closed_; }

private:
    std::vector<::jxx::lang::jbyte> bytes_;
    ::jxx::lang::jint writeFailureCount_ = 0;
    ::jxx::lang::jint flushAttemptCount_ = 0;
    ::jxx::lang::jint flushFailureCount_ = 0;
    ::jxx::lang::jint closeAttemptCount_ = 0;
    ::jxx::lang::jint closeFailureCount_ = 0;
    ::jxx::lang::jbool failWrite_ = false;
    ::jxx::lang::jbool failFlush_ = false;
    ::jxx::lang::jbool failClose_ = false;
    ::jxx::lang::jbool closed_ = false;
};

TEST(BufferedOutputStreamFailureTest, FlushWriteFailureRetainsPendingBytesForRetry) {
    const auto delegate = ::jxx::NEW<FailingOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(delegate, 8);
    stream->write(0x41);
    stream->write(0x42);
    delegate->failNextWrite();

    EXPECT_THROW(stream->flush(), ::jxx::io::IOException);
    EXPECT_EQ(0, delegate->size());
    EXPECT_EQ(1, delegate->writeFailureCount());

    stream->flush();
    EXPECT_EQ(2, delegate->size());
    EXPECT_EQ(1, delegate->flushAttemptCount());
    stream->close();
}

TEST(BufferedOutputStreamFailureTest, DelegateFlushFailurePropagatesAfterBufferWasDrained) {
    const auto delegate = ::jxx::NEW<FailingOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(delegate, 8);
    stream->write(0x41);
    delegate->failNextFlush();

    EXPECT_THROW(stream->flush(), ::jxx::io::IOException);
    EXPECT_EQ(1, delegate->size());
    EXPECT_EQ(1, delegate->flushFailureCount());

    stream->flush();
    EXPECT_EQ(1, delegate->size());
    EXPECT_EQ(2, delegate->flushAttemptCount());
    stream->close();
}

TEST(BufferedOutputStreamFailureTest, CloseAttemptsDelegateCloseWhenBufferWriteFails) {
    const auto delegate = ::jxx::NEW<FailingOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(delegate, 8);
    stream->write(0x41);
    delegate->failNextWrite();

    EXPECT_THROW(stream->close(), ::jxx::io::IOException);
    EXPECT_EQ(1, delegate->writeFailureCount());
    EXPECT_EQ(1, delegate->closeAttemptCount());
    EXPECT_TRUE(delegate->isClosed());

    stream->close();
    EXPECT_EQ(1, delegate->closeAttemptCount());
}

TEST(BufferedOutputStreamFailureTest, CloseAttemptsDelegateCloseWhenDelegateFlushFails) {
    const auto delegate = ::jxx::NEW<FailingOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(delegate, 8);
    stream->write(0x41);
    delegate->failNextFlush();

    EXPECT_THROW(stream->close(), ::jxx::io::IOException);
    EXPECT_EQ(1, delegate->size());
    EXPECT_EQ(1, delegate->flushFailureCount());
    EXPECT_EQ(1, delegate->closeAttemptCount());
    EXPECT_TRUE(delegate->isClosed());

    stream->close();
    EXPECT_EQ(1, delegate->closeAttemptCount());
}

TEST(BufferedOutputStreamFailureTest, DelegateCloseFailurePropagatesAndCloseRemainsIdempotent) {
    const auto delegate = ::jxx::NEW<FailingOutputStream>();
    const auto stream = ::jxx::NEW<::jxx::io::BufferedOutputStream>(delegate, 8);
    stream->write(0x41);
    delegate->failNextClose();

    EXPECT_THROW(stream->close(), ::jxx::io::IOException);
    EXPECT_EQ(1, delegate->size());
    EXPECT_EQ(1, delegate->closeFailureCount());
    EXPECT_EQ(1, delegate->closeAttemptCount());

    stream->close();
    EXPECT_EQ(1, delegate->closeAttemptCount());
}

} // namespace
