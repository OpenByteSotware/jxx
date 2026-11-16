#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.ArrayBlockingQueue.h"
#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentHashMap.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentLinkedDeque.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentLinkedQueue.h"
#include "util/concurrent/jxx.util.concurrent.DelayQueue.h"
#include "util/concurrent/jxx.util.concurrent.LinkedBlockingDeque.h"
#include "util/concurrent/jxx.util.concurrent.LinkedBlockingQueue.h"
#include "util/concurrent/jxx.util.concurrent.LinkedTransferQueue.h"
#include "util/concurrent/jxx.util.concurrent.PriorityBlockingQueue.h"
#include "util/concurrent/jxx.util.concurrent.SynchronousQueue.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ClassInitialization.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.StampedLock.h"

namespace {
template <typename T>
void expectUid(::jxx::lang::jlong expected) {
    const auto metadata = ::jxx::io::serialization::SerializationClassRegistry::resolve(
        T::Class()->getName()->utf8());
    EXPECT_EQ(expected, metadata.serialVersionUID);
    ASSERT_TRUE(static_cast<bool>(metadata.deserializationFactory));
    EXPECT_NE(nullptr, metadata.deserializationFactory());
}
}

TEST(UtilConcurrentRegistrationParity, UsesExactSerializationUids) {
    ::jxx::util::concurrent::initializeClasses();
    ::jxx::util::concurrent::locks::initializeClasses();
    using Object = ::jxx::lang::Object;
    expectUid<::jxx::util::concurrent::ConcurrentLinkedQueue<Object>>(196745693267521676LL);
    expectUid<::jxx::util::concurrent::ConcurrentLinkedDeque<Object>>(876323262645176354LL);
    expectUid<::jxx::util::concurrent::PriorityBlockingQueue<Object>>(5595510919245408276LL);
    expectUid<::jxx::util::concurrent::LinkedTransferQueue<Object>>(-3223113410248163686LL);
    expectUid<::jxx::util::concurrent::SynchronousQueue<Object>>(-3223113410248163686LL);
    expectUid<::jxx::util::concurrent::ConcurrentHashMap<Object, Object>>(7249069246763182397LL);
    expectUid<::jxx::util::concurrent::ArrayBlockingQueue<Object>>(-817911632652898426LL);
    expectUid<::jxx::util::concurrent::LinkedBlockingQueue<Object>>(-6903933977591709194LL);
    expectUid<::jxx::util::concurrent::LinkedBlockingDeque<Object>>(-387911632671998426LL);
    expectUid<::jxx::util::concurrent::locks::StampedLock>(-6001602636862214147LL);
}

TEST(UtilConcurrentRegistrationParity, DelayQueueIsNotSerializable) {
    using Queue = ::jxx::util::concurrent::DelayQueue<::jxx::lang::Object>;
    EXPECT_FALSE((std::is_base_of_v<::jxx::io::SerializableI, Queue>));
    ::jxx::util::concurrent::initializeClasses();
    EXPECT_NE(nullptr, Queue::Class());
}
