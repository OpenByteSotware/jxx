#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <type_traits>

#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/ref/jxx.lang.ref.PhantomReference.h"
#include "lang/ref/jxx.lang.ref.Reference.h"
#include "lang/ref/jxx.lang.ref.ReferenceQueue.h"
#include "lang/ref/jxx.lang.ref.SoftReference.h"
#include "lang/ref/jxx.lang.ref.WeakReference.h"

namespace {

using Object = ::jxx::lang::Object;
using Reference = ::jxx::lang::ref::Reference<Object>;
using Queue = ::jxx::lang::ref::ReferenceQueue<Object>;
using WeakReference = ::jxx::lang::ref::WeakReference<Object>;
using SoftReference = ::jxx::lang::ref::SoftReference<Object>;
using PhantomReference = ::jxx::lang::ref::PhantomReference<Object>;

TEST(JxxLangRefTest, HasJava8Hierarchy)
{
    EXPECT_TRUE((std::is_base_of_v<Object, Reference>));
    EXPECT_TRUE((std::is_base_of_v<Reference, WeakReference>));
    EXPECT_TRUE((std::is_base_of_v<Reference, SoftReference>));
    EXPECT_TRUE((std::is_base_of_v<Reference, PhantomReference>));
    EXPECT_TRUE((std::is_base_of_v<Object, Queue>));
}

TEST(JxxLangRefTest, WeakReferenceDoesNotRetainReferent)
{
    auto referent = ::jxx::NEW<::jxx::lang::String>("value");
    const auto reference = ::jxx::NEW<
        ::jxx::lang::ref::WeakReference<::jxx::lang::String>>(referent);
    EXPECT_EQ(referent, reference->get());
    referent.reset();
    EXPECT_EQ(nullptr, reference->get());
}

TEST(JxxLangRefTest, QueueObservesExpiredWeakReference)
{
    const auto queue = ::jxx::NEW<Queue>();
    auto referent = ::jxx::NEW<Object>();
    const auto reference = ::jxx::NEW<WeakReference>(referent, queue);
    referent.reset();
    const auto queued = queue->remove(250);
    EXPECT_EQ(reference, queued);
    EXPECT_FALSE(reference->isEnqueued());
    EXPECT_FALSE(reference->enqueue());
    EXPECT_EQ(nullptr, reference->get());
}

TEST(JxxLangRefTest, ManualEnqueueIsIdempotent)
{
    const auto queue = ::jxx::NEW<Queue>();
    const auto reference = ::jxx::NEW<WeakReference>(
        ::jxx::NEW<Object>(), queue);
    EXPECT_TRUE(reference->enqueue());
    EXPECT_FALSE(reference->enqueue());
    EXPECT_EQ(reference, queue->poll());
    EXPECT_EQ(nullptr, queue->poll());
}

TEST(JxxLangRefTest, PhantomGetAlwaysReturnsNull)
{
    const auto queue = ::jxx::NEW<Queue>();
    const auto referent = ::jxx::NEW<Object>();
    const auto reference = ::jxx::NEW<PhantomReference>(referent, queue);
    EXPECT_EQ(nullptr, reference->get());
}

TEST(JxxLangRefTest, SoftReferenceRetainsUntilCleared)
{
    auto referent = ::jxx::NEW<Object>();
    const auto reference = ::jxx::NEW<SoftReference>(referent);
    referent.reset();
    EXPECT_NE(nullptr, reference->get());
    reference->clear();
    EXPECT_EQ(nullptr, reference->get());
}

TEST(JxxLangRefTest, TimedRemoveReturnsNull)
{
    const auto queue = ::jxx::NEW<Queue>();
    const auto start = std::chrono::steady_clock::now();
    EXPECT_EQ(nullptr, queue->remove(20));
    EXPECT_GE(std::chrono::steady_clock::now() - start,
        std::chrono::milliseconds(15));
}

TEST(JxxLangRefTest, ClassInitializationRegistersErasedPublicTypes)
{
    ::jxx::initialize();
    EXPECT_NE(nullptr, Reference::Class());
    EXPECT_NE(nullptr, Queue::Class());
    EXPECT_NE(nullptr, WeakReference::Class());
    EXPECT_NE(nullptr, SoftReference::Class());
    EXPECT_NE(nullptr, PhantomReference::Class());
}

} // namespace
