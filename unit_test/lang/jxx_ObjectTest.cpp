#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Cloneable.h"

class DummyObject : public jxx::lang::Object {
public:
    jxx::Ptr<DummyObject> self() { return getThis<DummyObject>(); }
};


class CloneableDummy final
    : public jxx::lang::ClassBase<
    CloneableDummy,
    jxx::lang::Object,
    jxx::lang::Cloneable>
{
public:
    using JxxSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<
        CloneableDummy,
        JxxSuper,
        jxx::lang::Cloneable>;

    CloneableDummy() = default;

    explicit CloneableDummy(int value)
        : Super(),
        value(value)
    {
    }

    CloneableDummy(const CloneableDummy&) = default;

    int value = 0;

protected:
    jxx::Ptr<jxx::lang::Object> cloneImpl() const override
    {
        return jxx::CAST<jxx::lang::Object>(
            jxx::NEW<CloneableDummy>(*this));
    }
};

TEST(ObjectNewTest, ReturnsNonNullPointer) {
    auto obj = jxx::NEW<DummyObject>();
    ASSERT_NE(obj, nullptr);
}

TEST(ObjectNewTest, ThisPtrReturnsOwningPointer) {
    auto obj = jxx::NEW<DummyObject>();
    auto self = obj->thisPtr();
    ASSERT_NE(self, nullptr);
    EXPECT_EQ(self.get(), obj.get());
}

TEST(ObjectNewTest, GetThisReturnsCorrectType) {
    auto obj = jxx::NEW<DummyObject>();
    auto self = obj->self();
    ASSERT_NE(self, nullptr);
    EXPECT_EQ(self.get(), obj.get());
}

TEST(ObjectNewTest, SharedFromThisWorks) {
    auto obj = jxx::NEW<DummyObject>();
    auto shared = obj->shared_from_this();
    EXPECT_EQ(shared.get(), obj.get());
}

TEST(ObjectNewTest, ConstructorArgumentsForwarded) {
    auto value = jxx::NEW<CloneableDummy>(42);
    EXPECT_EQ(value->value, 42);
}

TEST(ObjectNewTest, CloneReturnsDistinctObject) {
    auto original = jxx::NEW<CloneableDummy>(123);
    auto clone = jxx::CAST<CloneableDummy>(original->clone());
    ASSERT_NE(clone, nullptr);
    EXPECT_NE(clone.get(), original.get());
    EXPECT_EQ(clone->value, 123);
}

TEST(ObjectOwnershipTest, NewInitializesSelfReference)
{
    auto object =
        jxx::NEW<DummyObject>();

    auto self =
        object->thisPtr();

    ASSERT_NE(self, nullptr);

    EXPECT_EQ(
        self.get(),
        object.get());

    EXPECT_FALSE(
        self.owner_before(object));

    EXPECT_FALSE(
        object.owner_before(self));
}

TEST(ObjectOwnershipTest, SharedPtrConstructionEstablishesOwnership)
{
    auto object =
        jxx::Ptr<DummyObject>(
            new DummyObject());

    auto self =
        object->thisPtr();

    ASSERT_NE(self, nullptr);

    EXPECT_EQ(
        self.get(),
        object.get());

    //
    // Verify both pointers share
    // the same ownership block.
    //
    EXPECT_FALSE(
        self.owner_before(object));

    EXPECT_FALSE(
        object.owner_before(self));
}

TEST(ObjectNewTest, CloneOperationReturnsObject)
{
    auto original =
        jxx::NEW<CloneableDummy>(1);

    jxx::Ptr<jxx::lang::Object> clonedObject;

    ASSERT_NO_THROW(
        clonedObject = original->clone());

    ASSERT_NE(clonedObject, nullptr);
}

TEST(ObjectNewTest, CloneCanBeCastToDerivedType)
{
    auto original =
        jxx::NEW<CloneableDummy>(1);

    auto clonedObject =
        original->clone();

    jxx::Ptr<CloneableDummy> clone;

    ASSERT_NO_THROW(
        clone =
            jxx::CAST<CloneableDummy>(
                clonedObject));

    ASSERT_NE(clone, nullptr);
}

TEST(ObjectNewTest, ClonedObjectHasSharedOwnership)
{
    auto original =
        jxx::NEW<CloneableDummy>(1);

    auto clone =
        jxx::CAST<CloneableDummy>(
            original->clone());

    ASSERT_NE(clone, nullptr);

    jxx::Ptr<jxx::lang::Object> self;

    ASSERT_NO_THROW(
        self = clone->thisPtr());

    ASSERT_NE(self, nullptr);
    EXPECT_EQ(self.get(), clone.get());

    EXPECT_FALSE(self.owner_before(clone));
    EXPECT_FALSE(clone.owner_before(self));
}

TEST(ObjectNewTest, CloneHasValidThisPtr) {
    auto original = jxx::NEW<CloneableDummy>(1);
    auto clone = jxx::CAST<CloneableDummy>(original->clone());
    auto self = clone->thisPtr();
    ASSERT_NE(self, nullptr);
    EXPECT_EQ(self.get(), clone.get());
}

TEST(ObjectNewTest, DefaultEqualsIdentity) {
    auto a = jxx::NEW<DummyObject>();
    auto b = jxx::NEW<DummyObject>();
    EXPECT_TRUE(a->equals(a));
    EXPECT_FALSE(a->equals(b));
}

TEST(ObjectNewTest, SameUsesIdentity) {
    auto a = jxx::NEW<DummyObject>();
    auto b = a;
    auto c = jxx::NEW<DummyObject>();
    EXPECT_TRUE(a->same(b));
    EXPECT_FALSE(a->same(c));
}
TEST(ObjectNewTest, SharedPtrConstructionEstablishesSharedOwnership)
{
    auto object =
        jxx::Ptr<DummyObject>(
            new DummyObject());

    auto self = object->thisPtr();

    ASSERT_NE(self, nullptr);
    EXPECT_EQ(self.get(), object.get());

    EXPECT_FALSE(
        self.owner_before(object));

    EXPECT_FALSE(
        object.owner_before(self));
}
TEST(ObjectNewTest, ManyObjectsMaintainOwnership) {
    std::vector<jxx::Ptr<DummyObject>> values;
    for (int i = 0; i < 10000; ++i) values.push_back(jxx::NEW<DummyObject>());
    for (const auto& value : values) {
        EXPECT_EQ(value->thisPtr().get(), value.get());
    }
}
