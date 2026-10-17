#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.ArrayBlockingQueue.h"
#include "util/concurrent/jxx.util.concurrent.LinkedBlockingQueue.h"
#include "util/concurrent/jxx.util.concurrent.LinkedBlockingDeque.h"
namespace {
using String=::jxx::lang::String;
template<class Q> ::jxx::Ptr<Q> roundTrip(const ::jxx::Ptr<Q>& v){auto b=::jxx::NEW<::jxx::io::ByteArrayOutputStream>();auto o=::jxx::NEW<::jxx::io::ObjectOutputStream>(b);v->writeObject(o);o->flush();auto i=::jxx::NEW<::jxx::io::ObjectInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(b->toByteArray()));auto r=::jxx::NEW<Q>(1);r->readObject(i);return r;}
TEST(ArrayBlockingQueueSerialization,RoundTripsOrderCapacityAndMutability){using Q=::jxx::util::concurrent::ArrayBlockingQueue<String>;auto q=::jxx::NEW<Q>(3,true);q->offer(::jxx::NEW<String>("one"));q->offer(::jxx::NEW<String>("two"));auto r=roundTrip(q);EXPECT_EQ(1,r->remainingCapacity());EXPECT_EQ("one",r->poll()->utf8());EXPECT_EQ("two",r->poll()->utf8());EXPECT_TRUE(r->offer(::jxx::NEW<String>("three")));}
TEST(LinkedBlockingQueueSerialization,RoundTripsOrderCapacityAndMutability){using Q=::jxx::util::concurrent::LinkedBlockingQueue<String>;auto q=::jxx::NEW<Q>(3);q->offer(::jxx::NEW<String>("one"));q->offer(::jxx::NEW<String>("two"));auto r=roundTrip(q);EXPECT_EQ(1,r->remainingCapacity());EXPECT_EQ("one",r->poll()->utf8());EXPECT_EQ("two",r->poll()->utf8());}
TEST(LinkedBlockingDequeSerialization,RoundTripsOrderCapacityAndMutability){using Q=::jxx::util::concurrent::LinkedBlockingDeque<String>;auto q=::jxx::NEW<Q>(3);q->offerLast(::jxx::NEW<String>("one"));q->offerLast(::jxx::NEW<String>("two"));auto r=roundTrip(q);EXPECT_EQ(1,r->remainingCapacity());EXPECT_EQ("one",r->pollFirst()->utf8());EXPECT_EQ("two",r->pollFirst()->utf8());}
TEST(BlockingQueueSerialization,NoDataRestoresEmptyUsableState){using A=::jxx::util::concurrent::ArrayBlockingQueue<String>;using Q=::jxx::util::concurrent::LinkedBlockingQueue<String>;using D=::jxx::util::concurrent::LinkedBlockingDeque<String>;auto a=::jxx::NEW<A>(2);auto q=::jxx::NEW<Q>(2);auto d=::jxx::NEW<D>(2);a->readObjectNoData();q->readObjectNoData();d->readObjectNoData();EXPECT_TRUE(a->offer(::jxx::NEW<String>("a")));EXPECT_TRUE(q->offer(::jxx::NEW<String>("q")));EXPECT_TRUE(d->offer(::jxx::NEW<String>("d")));}
}
