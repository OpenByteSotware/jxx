#include <atomic>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"

namespace {

class TrackedObject : public jxx::lang::Object {
public:
    TrackedObject()
    {
        ++liveCount;
        ++constructedCount;
    }

    explicit TrackedObject(jxx::lang::jint value)
        : value_(value)
    {
        ++liveCount;
        ++constructedCount;
    }

    TrackedObject(const TrackedObject& other)
        : jxx::lang::Object(other)
        , value_(other.value_)
    {
        ++liveCount;
        ++constructedCount;
    }

    TrackedObject& operator=(const TrackedObject& other)
    {
        if (this != &other) {
            jxx::lang::Object::operator=(other);
            value_ = other.value_;
        }

        return *this;
    }

    ~TrackedObject() override
    {
        --liveCount;
        ++destroyedCount;
    }

    jxx::lang::jint value() const noexcept
    {
        return value_;
    }

    jxx::Ptr<TrackedObject> self()
    {
        return jxx::CAST<TrackedObject>(thisPtr());
    }

    jxx::Ptr<const TrackedObject> self() const
    {
        return std::dynamic_pointer_cast<const TrackedObject>(
            thisPtr());
    }

    static void resetCounts()
    {
        liveCount = 0;
        constructedCount = 0;
        destroyedCount = 0;
    }

    static std::atomic<int> liveCount;
    static std::atomic<int> constructedCount;
    static std::atomic<int> destroyedCount;

private:
    jxx::lang::jint value_ = 0;
};

std::atomic<int> TrackedObject::liveCount{0};
std::atomic<int> TrackedObject::constructedCount{0};
std::atomic<int> TrackedObject::destroyedCount{0};

class DerivedTrackedObject final : public TrackedObject {
public:
    explicit DerivedTrackedObject(jxx::lang::jint value)
        : TrackedObject(value)
    {
    }

    jxx::Ptr<DerivedTrackedObject> derivedSelf()
    {
        return jxx::CAST<DerivedTrackedObject>(thisPtr());
    }
};

class ObjectTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        TrackedObject::resetCounts();
    }

    void TearDown() override
    {
        EXPECT_EQ(TrackedObject::liveCount.load(), 0)
            << "A test retained a TrackedObject instance";

        EXPECT_EQ(
            TrackedObject::constructedCount.load(),
            TrackedObject::destroyedCount.load())
            << "Constructed and destroyed object counts differ";
    }
};

TEST_F(ObjectTest, RootObjectThisPtrReturnsOwningReference)
{
    auto object = jxx::NEW<jxx::lang::Object>();

    const auto self = object->thisPtr();

    ASSERT_NE(self, nullptr);
    EXPECT_EQ(self.get(), object.get());
    EXPECT_TRUE(object->same(self));
}

TEST_F(ObjectTest, ThisPtrUsesTheExistingSharedControlBlock)
{
    auto object = jxx::NEW<TrackedObject>(42);

    const auto first = object->thisPtr();
    const auto second = object->thisPtr();

    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_EQ(first.get(), object.get());
    EXPECT_EQ(second.get(), object.get());

    EXPECT_FALSE(first.owner_before(second));
    EXPECT_FALSE(second.owner_before(first));
    EXPECT_FALSE(first.owner_before(object));
    EXPECT_FALSE(object.owner_before(first));

    EXPECT_EQ(TrackedObject::liveCount.load(), 1);
}

TEST_F(ObjectTest, ThisPtrTemporarilyIncreasesStrongReferenceCount)
{
    auto object = jxx::NEW<TrackedObject>();
    const auto initialCount = object.use_count();

    {
        const auto self = object->thisPtr();
        EXPECT_EQ(self.get(), object.get());
        EXPECT_EQ(object.use_count(), initialCount + 1);
    }

    EXPECT_EQ(object.use_count(), initialCount);
    EXPECT_EQ(TrackedObject::liveCount.load(), 1);
}

TEST_F(ObjectTest, WeakSelfHandleDoesNotKeepObjectAlive)
{
    std::weak_ptr<TrackedObject> observer;

    {
        auto object = jxx::NEW<TrackedObject>(7);
        observer = object;

        ASSERT_FALSE(observer.expired());
        EXPECT_EQ(TrackedObject::liveCount.load(), 1);

        const auto self = object->self();
        ASSERT_NE(self, nullptr);
        EXPECT_EQ(self.get(), object.get());
    }

    EXPECT_TRUE(observer.expired());
    EXPECT_EQ(TrackedObject::liveCount.load(), 0);
    EXPECT_EQ(TrackedObject::constructedCount.load(), 1);
    EXPECT_EQ(TrackedObject::destroyedCount.load(), 1);
}

TEST_F(ObjectTest, ReturnedSelfReferenceKeepsObjectAliveUntilReleased)
{
    std::weak_ptr<TrackedObject> observer;
    jxx::Ptr<TrackedObject> self;

    {
        auto object = jxx::NEW<TrackedObject>(9);
        observer = object;
        self = object->self();

        ASSERT_NE(self, nullptr);
        EXPECT_EQ(self.get(), object.get());
    }

    EXPECT_FALSE(observer.expired());
    EXPECT_EQ(TrackedObject::liveCount.load(), 1);

    self.reset();

    EXPECT_TRUE(observer.expired());
    EXPECT_EQ(TrackedObject::liveCount.load(), 0);
    EXPECT_EQ(TrackedObject::destroyedCount.load(), 1);
}

TEST_F(ObjectTest, DerivedObjectReturnsCorrectBaseAndDerivedReferences)
{
    auto object = jxx::NEW<DerivedTrackedObject>(123);

    const auto baseSelf = object->thisPtr();
    const auto derivedSelf = object->derivedSelf();

    ASSERT_NE(baseSelf, nullptr);
    ASSERT_NE(derivedSelf, nullptr);
    EXPECT_EQ(baseSelf.get(), object.get());
    EXPECT_EQ(derivedSelf.get(), object.get());
    EXPECT_EQ(derivedSelf->value(), 123);

    EXPECT_FALSE(baseSelf.owner_before(derivedSelf));
    EXPECT_FALSE(derivedSelf.owner_before(baseSelf));
}

TEST_F(ObjectTest, ConstObjectCanReturnConstDerivedReference)
{
    auto object = jxx::NEW<TrackedObject>(55);
    const TrackedObject& constObject = *object;

    const auto self = constObject.self();

    static_assert(
        std::is_same_v<
            decltype(self),
            const jxx::Ptr<const TrackedObject>>);

    ASSERT_NE(self, nullptr);
    EXPECT_EQ(self.get(), object.get());
    EXPECT_EQ(self->value(), 55);
}

TEST_F(ObjectTest, CopyCreatedThroughNewGetsIndependentSelfReference)
{
    auto original = jxx::NEW<TrackedObject>(88);
    auto copy = jxx::NEW<TrackedObject>(*original);

    const auto originalSelf = original->self();
    const auto copySelf = copy->self();

    ASSERT_NE(originalSelf, nullptr);
    ASSERT_NE(copySelf, nullptr);
    EXPECT_NE(originalSelf.get(), copySelf.get());
    EXPECT_EQ(originalSelf->value(), 88);
    EXPECT_EQ(copySelf->value(), 88);

    EXPECT_TRUE(
        originalSelf.owner_before(copySelf) ||
        copySelf.owner_before(originalSelf));

    EXPECT_EQ(TrackedObject::liveCount.load(), 2);
}

TEST_F(ObjectTest, RepeatedThisPtrCallsDoNotCreateAnOwnershipCycle)
{
    std::weak_ptr<TrackedObject> observer;

    {
        auto object = jxx::NEW<TrackedObject>();
        observer = object;

        for (int index = 0; index < 10000; ++index) {
            const auto self = object->thisPtr();
            ASSERT_NE(self, nullptr);
            ASSERT_EQ(self.get(), object.get());
        }

        EXPECT_EQ(TrackedObject::liveCount.load(), 1);
    }

    EXPECT_TRUE(observer.expired());
    EXPECT_EQ(TrackedObject::liveCount.load(), 0);
    EXPECT_EQ(TrackedObject::constructedCount.load(), 1);
    EXPECT_EQ(TrackedObject::destroyedCount.load(), 1);
}

TEST_F(ObjectTest, StackAllocatedObjectRejectsCountedSelfReference)
{
    jxx::lang::Object object;

    EXPECT_THROW(
        (void)object.thisPtr(),
        std::logic_error);
}

TEST_F(ObjectTest, DirectMakeSharedCanUseSharedFromThisFallback)
{
    auto object = std::make_shared<TrackedObject>(17);
    std::weak_ptr<TrackedObject> observer = object;

    auto self = object->self();

    ASSERT_NE(self, nullptr);
    EXPECT_EQ(self.get(), object.get());
    EXPECT_FALSE(self.owner_before(object));
    EXPECT_FALSE(object.owner_before(self));

    object.reset();
    EXPECT_FALSE(observer.expired());

    self.reset();
    EXPECT_TRUE(observer.expired());
}

} // namespace
