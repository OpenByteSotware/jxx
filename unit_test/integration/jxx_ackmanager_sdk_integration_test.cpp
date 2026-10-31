#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "jxx.initialize.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.JxxArray.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.Timer.h"
#include "util/jxx.util.TimerTask.h"
#include "util/jxx.util.Vector.h"

namespace {

using RawVector = ::jxx::util::Vector<::jxx::lang::Object>;
using FormatArguments = ::jxx::lang::JxxArray<
    ::jxx::Ptr<::jxx::lang::Object>, 1U>;

class AckState final
    : public ::jxx::lang::ClassBase<AckState, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<AckState, JxxSuper>;

    AckState()
        : Super(), lock_(::jxx::NEW<::jxx::lang::Object>()),
          recent_(::jxx::NEW<RawVector>()), sent_(::jxx::NEW<RawVector>())
    {
        recent_->setSize(5);
        recent_->trimToSize();
    }

    void installPacket(const ::jxx::lang::ByteArray& packet)
    {
        lock_->synchronized([&] {
            packet_ = packet;
            ++packetIdentifier_;
            retryCount_ = 0;
            expectingAck_.store(true);
        });
    }

    void retry()
    {
        lock_->synchronized([&] {
            if (!expectingAck_.load() || packet_ == nullptr) return;
            sent_->add(::jxx::CAST<::jxx::lang::Object>(packet_));
            ++retryCount_;
        });
    }

    ::jxx::lang::jbool acknowledge(const ::jxx::lang::ByteArray& incoming)
    {
        return lock_->synchronized([&] {
            if (packet_ == nullptr) return false;
            auto expected = ::jxx::util::Arrays::copyOfRange(packet_, 0, 4);
            auto actual = ::jxx::util::Arrays::copyOfRange(incoming, 0, 4);
            if (!::jxx::util::Arrays::equals(expected, actual)) return false;
            recent_->removeElementAt(0);
            recent_->add(::jxx::CAST<::jxx::lang::Object>(actual));
            sent_->removeAllElements();
            expectingAck_.store(false);
            return true;
        });
    }

    ::jxx::lang::jint retryCount() const
    {
        return lock_->synchronized([&] { return retryCount_; });
    }

    ::jxx::lang::jint sentCount() const { return sent_->size(); }
    ::jxx::lang::jbool expectingAck() const { return expectingAck_.load(); }

private:
    ::jxx::Ptr<::jxx::lang::Object> lock_;
    ::jxx::lang::ByteArray packet_;
    ::jxx::Ptr<RawVector> recent_;
    ::jxx::Ptr<RawVector> sent_;
    ::jxx::lang::jint packetIdentifier_ = 1000;
    ::jxx::lang::jint retryCount_ = 0;
    std::atomic<::jxx::lang::jbool> expectingAck_{false};
};

class RetryTask final : public ::jxx::util::TimerTask {
public:
    explicit RetryTask(const ::jxx::Ptr<AckState>& state) : state_(state) {}
    void run() override { state_->retry(); }
private:
    ::jxx::Ptr<AckState> state_;
};

bool waitForRetries(const ::jxx::Ptr<AckState>& state,
                    ::jxx::lang::jint expected)
{
    for (int attempt = 0; attempt < 200; ++attempt) {
        if (state->retryCount() >= expected) return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    return state->retryCount() >= expected;
}

TEST(AckManagerSdkIntegration, RetryAckAndLoggingPrimitivesWorkTogether)
{
    ::jxx::initialize();
    auto state = ::jxx::NEW<AckState>();
    auto packet = ::jxx::NEW<::jxx::lang::ByteArrayType>(12);
    (*packet)[0] = 0x11;
    (*packet)[1] = 0x22;
    (*packet)[2] = 0x33;
    (*packet)[3] = 0x44;
    (*packet)[4] = 0x02;
    (*packet)[5] = 0x11;
    (*packet)[6] = 0x01;
    state->installPacket(packet);

    auto timer = ::jxx::NEW<::jxx::util::Timer>();
    auto retryTask = ::jxx::NEW<RetryTask>(state);
    timer->schedule(retryTask, 0, 5);
    ASSERT_TRUE(waitForRetries(state, 2));
    EXPECT_TRUE(state->expectingAck());
    EXPECT_GE(state->sentCount(), 2);

    auto acknowledgement = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    ::jxx::lang::System::arraycopy(packet, 0, acknowledgement, 0, 4);
    EXPECT_TRUE(state->acknowledge(acknowledgement));
    EXPECT_FALSE(state->expectingAck());
    EXPECT_EQ(state->sentCount(), 0);

    retryTask->cancel();
    timer->cancel();

    auto arguments = ::jxx::NEW<FormatArguments>(2);
    (*arguments)[0] = ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Integer::toHexString(0x1102));
    (*arguments)[1] = ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<::jxx::lang::String>("192.0.2.10"));
    auto message = ::jxx::lang::String::format(
        ::jxx::NEW<::jxx::lang::String>("message ID %s (%s)"), arguments);
    ASSERT_NE(message, nullptr);
    EXPECT_EQ(message->utf8(), "message ID 1102 (192.0.2.10)");
}

TEST(AckManagerSdkIntegration, MismatchedAckLeavesRetryStateActive)
{
    ::jxx::initialize();
    auto state = ::jxx::NEW<AckState>();
    auto packet = ::jxx::NEW<::jxx::lang::ByteArrayType>(8);
    (*packet)[0] = 1;
    (*packet)[1] = 2;
    (*packet)[2] = 3;
    (*packet)[3] = 4;
    state->installPacket(packet);

    auto wrong = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    (*wrong)[0] = 9;
    EXPECT_FALSE(state->acknowledge(wrong));
    EXPECT_TRUE(state->expectingAck());
}

} // namespace
