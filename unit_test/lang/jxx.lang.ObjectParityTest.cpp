#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <thread>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalMonitorStateException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace {

class PlainObject final
    : public ::jxx::lang::ClassBase<PlainObject, ::jxx::lang::Object> {
};

class CloneableObject final
    : public ::jxx::lang::ClassBase<
          CloneableObject,
          ::jxx::lang::Object,
          ::jxx::lang::Cloneable> {
public:
    explicit CloneableObject(::jxx::lang::jint value = 0) : value_(value) {}
    ::jxx::lang::jint value() const noexcept { return value_; }
protected:
    JXX_OBJECT_CLONE(CloneableObject)
private:
    ::jxx::lang::jint value_;
};

TEST(ObjectParityTest, EqualityAndHashCodeUseIdentityByDefault) {
    const auto first = ::jxx::NEW<PlainObject>();
    const auto alias = first;
    const auto second = ::jxx::NEW<PlainObject>();
    EXPECT_TRUE(first->equals(alias));
    EXPECT_FALSE(first->equals(second));
    EXPECT_FALSE(first->equals(nullptr));
    EXPECT_EQ(first->hashCode(), alias->hashCode());
    EXPECT_EQ(first->hashCode(), first->hashCode());
}

TEST(ObjectParityTest, GetClassReturnsExactRuntimeClass) {
    const ::jxx::Ptr<::jxx::lang::Object> value = ::jxx::NEW<PlainObject>();
    EXPECT_EQ(PlainObject::Class(), value->getClass());
}

TEST(ObjectParityTest, DefaultToStringUsesExactClassAtHexHashFormat) {
    const auto value = ::jxx::NEW<PlainObject>();
    std::ostringstream expected;
    expected << value->getClass()->getName()->utf8()
             << '@' << std::hex
             << static_cast<std::uint32_t>(value->hashCode());
    EXPECT_EQ(expected.str(), value->toString()->utf8());
}

TEST(ObjectParityTest, CloneRequiresCloneableAndMakesShallowCopy) {
    const auto plain = ::jxx::NEW<PlainObject>();
    EXPECT_THROW(plain->clone(), ::jxx::lang::CloneNotSupportedException);

    const auto source = ::jxx::NEW<CloneableObject>(42);
    const auto clone = ::jxx::CAST<CloneableObject>(source->clone());
    ASSERT_NE(nullptr, clone);
    EXPECT_NE(source.get(), clone.get());
    EXPECT_EQ(source->getClass(), clone->getClass());
    EXPECT_EQ(42, clone->value());
}

TEST(ObjectParityTest, MonitorOperationsRequireOwnership) {
    const auto value = ::jxx::NEW<PlainObject>();
    EXPECT_THROW(value->notify(), ::jxx::lang::IllegalMonitorStateException);
    EXPECT_THROW(value->notifyAll(), ::jxx::lang::IllegalMonitorStateException);
    EXPECT_THROW(value->wait(1), ::jxx::lang::IllegalMonitorStateException);
}

TEST(ObjectParityTest, InvalidWaitArgumentsThrowIllegalArgumentException) {
    const auto value = ::jxx::NEW<PlainObject>();
    value->synchronized([&value] {
        EXPECT_THROW(value->wait(-1), ::jxx::lang::IllegalArgumentException);
        EXPECT_THROW(value->wait(0, -1), ::jxx::lang::IllegalArgumentException);
        EXPECT_THROW(value->wait(0, 1000000), ::jxx::lang::IllegalArgumentException);
    });
}

TEST(ObjectParityTest, TimedWaitReleasesAndReacquiresMonitor) {
    const auto value = ::jxx::NEW<PlainObject>();
    std::atomic<bool> entered{false};

    std::thread notifier([&] {
        while (!entered.load()) {
            std::this_thread::yield();
        }
        value->synchronized([&value] { value->notifyAll(); });
    });

    value->synchronized([&] {
        entered.store(true);
        value->wait(1000);
        // Ownership must have been restored before wait returns.
        value->notifyAll();
    });

    notifier.join();
}

TEST(ObjectParityTest, ReentrantWaitRestoresMonitorDepth) {
    const auto value = ::jxx::NEW<PlainObject>();
    value->synchronized([&] {
        value->synchronized([&] {
            value->wait(1);
            value->notifyAll();
        });
    });
}

} // namespace
