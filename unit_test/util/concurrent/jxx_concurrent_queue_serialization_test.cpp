#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentLinkedQueue.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentLinkedDeque.h"
#include "util/concurrent/jxx.util.concurrent.LinkedTransferQueue.h"
#include "util/concurrent/jxx.util.concurrent.SynchronousQueue.h"

using Object = ::jxx::lang::Object;

template<typename Q> ::jxx::Ptr<Q> roundTrip(const ::jxx::Ptr<Q>& source) {
    auto bytes=::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto out=::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    out->writeObject(source); out->flush();
    auto in=::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    return std::dynamic_pointer_cast<Q>(in->readObject());
}

TEST(ConcurrentQueueSerialization, PreservesFifoOrder) {
    ::jxx::initialize();
    using Q=::jxx::util::concurrent::ConcurrentLinkedQueue<Object>;
    auto q=::jxx::NEW<Q>();
    q->offer(::jxx::NEW<::jxx::lang::String>("first"));
    q->offer(::jxx::NEW<::jxx::lang::String>("second"));
    auto restored=roundTrip(q);
    ASSERT_NE(restored,nullptr);
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(restored->poll())->utf8(),"first");
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(restored->poll())->utf8(),"second");
}

TEST(SynchronousQueueSerialization, RestoresEmptyTransientState) {
    ::jxx::initialize();
    using Q=::jxx::util::concurrent::SynchronousQueue<Object>;
    auto restored=roundTrip(::jxx::NEW<Q>(true));
    ASSERT_NE(restored,nullptr);
    EXPECT_TRUE(restored->isEmpty());
}
