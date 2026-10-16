#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.AbstractOwnableSynchronizer.h"

namespace {

class TestSynchronizer final
    : public ::jxx::util::concurrent::locks::AbstractOwnableSynchronizer {
public:
    TestSynchronizer() = default;

    void setOwner(const ::jxx::Ptr<::jxx::lang::Thread>& owner) {
        setExclusiveOwnerThread(owner);
    }

    ::jxx::Ptr<::jxx::lang::Thread> owner() const {
        return getExclusiveOwnerThread();
    }
};

TEST(AbstractOwnableSynchronizerSerialization,
    ReadObjectClearsTransientOwner) {
    auto value = ::jxx::NEW<TestSynchronizer>();
    value->setOwner(::jxx::lang::Thread::currentThread());
    ASSERT_NE(value->owner(), nullptr);

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    value->writeObject(output);
    output->flush();
    auto byteInput = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(byteInput);

    value->readObject(input);

    EXPECT_EQ(nullptr, value->owner());
}

TEST(AbstractOwnableSynchronizerSerialization,
    NoDataClearsTransientOwner) {
    auto value = ::jxx::NEW<TestSynchronizer>();
    value->setOwner(::jxx::lang::Thread::currentThread());
    ASSERT_NE(value->owner(), nullptr);

    value->readObjectNoData();

    EXPECT_EQ(nullptr, value->owner());
}

TEST(AbstractOwnableSynchronizerSerialization,
    RejectsNullStreams) {
    auto value = ::jxx::NEW<TestSynchronizer>();
    EXPECT_THROW(value->writeObject(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(value->readObject(nullptr), ::jxx::lang::NullPointerException);
}

} // namespace
