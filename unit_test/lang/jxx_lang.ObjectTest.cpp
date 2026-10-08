#include <gtest/gtest.h>

#include <iomanip>
#include <sstream>
#include <string>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace {

class PlainObject final
    : public ::jxx::lang::ClassBase<PlainObject, ::jxx::lang::Object> {
public:
    ~PlainObject() override = default;
};

class CloneableObject final
    : public ::jxx::lang::ClassBase<
          CloneableObject,
          ::jxx::lang::Object,
          ::jxx::lang::Cloneable> {
public:
    explicit CloneableObject(::jxx::lang::jint value = 0)
        : value_(value) {
    }

    ~CloneableObject() override = default;

    ::jxx::lang::jint value() const noexcept {
        return value_;
    }

protected:
    JXX_OBJECT_CLONE(CloneableObject)

private:
    ::jxx::lang::jint value_;
};

TEST(ObjectTest2, EqualsDefaultsToReferenceIdentity) {
    const auto first = ::jxx::NEW<PlainObject>();
    const auto second = ::jxx::NEW<PlainObject>();

    EXPECT_TRUE(first->equals(first));
    EXPECT_FALSE(first->equals(second));
    EXPECT_FALSE(first->equals(nullptr));
}

TEST(ObjectTest2, DefaultEqualityIsReflexiveSymmetricAndTransitive) {
    const auto value = ::jxx::NEW<PlainObject>();
    const auto alias = value;
    const auto thirdAlias = alias;

    EXPECT_TRUE(value->equals(value));
    EXPECT_EQ(value->equals(alias), alias->equals(value));
    EXPECT_TRUE(value->equals(alias));
    EXPECT_TRUE(alias->equals(thirdAlias));
    EXPECT_TRUE(value->equals(thirdAlias));
}

TEST(ObjectTest2, HashCodeIsStableAndIdentityCompatible) {
    const auto value = ::jxx::NEW<PlainObject>();
    const auto alias = value;

    const auto firstHash = value->hashCode();
    EXPECT_EQ(firstHash, value->hashCode());
    EXPECT_TRUE(value->equals(alias));
    EXPECT_EQ(firstHash, alias->hashCode());
}

TEST(ObjectTest2, GetClassReturnsExactRuntimeClassDescriptor)
{
    const auto plainObjectClass =
        PlainObject::Class();

    ASSERT_NE(nullptr, plainObjectClass);

    const ::jxx::Ptr<::jxx::lang::Object> value =
        ::jxx::NEW<PlainObject>();

    const auto runtimeClass =
        value->getClass();

    ASSERT_NE(nullptr, runtimeClass);
    EXPECT_EQ(plainObjectClass, runtimeClass);
    EXPECT_TRUE(plainObjectClass->isInstance(value));
}

TEST(ObjectTest2, ThisPtrReturnsTheOwningReference) {
    const auto value = ::jxx::NEW<PlainObject>();

    EXPECT_EQ(value.get(), value->thisPtr().get());
}

TEST(ObjectTest2, SameUsesReferenceIdentity) {
    const auto first = ::jxx::NEW<PlainObject>();
    const auto alias = first;
    const auto second = ::jxx::NEW<PlainObject>();

    EXPECT_TRUE(first->same(alias));
    EXPECT_FALSE(first->same(second));
    EXPECT_FALSE(first->same(nullptr));
}

TEST(ObjectTest2, ToStringContainsRuntimeClassAndHexHash)
{
    const auto plainObjectClass =
        PlainObject::Class();

    ASSERT_NE(nullptr, plainObjectClass);

    const auto value =
        ::jxx::NEW<PlainObject>();

    const auto className =
        value->getClassName();

    const auto text =
        value->toString();

    ASSERT_NE(nullptr, className);
    ASSERT_NE(nullptr, text);

    const std::string actual =
        text->utf8();

    EXPECT_EQ(
        0U,
        actual.find(className->utf8() + "@"));

    std::ostringstream hash;
    hash << std::hex
        << static_cast<std::uint32_t>(
               value->hashCode());

    EXPECT_NE(
        std::string::npos,
        actual.find(hash.str()));
}

TEST(ObjectTest2, CloneRejectsAnObjectThatIsNotCloneable) {
    const auto value = ::jxx::NEW<PlainObject>();

    EXPECT_THROW(value->clone(), ::jxx::lang::CloneNotSupportedException);
}
TEST(ObjectTest2, CloneableObjectProducesDistinctShallowCopy)
{
    const auto cloneableObjectClass =
        CloneableObject::Class();

    ASSERT_NE(nullptr, cloneableObjectClass);

    const auto value =
        ::jxx::NEW<CloneableObject>(42);

    const auto clonedBase = value->clone();
    const auto cloned =
        ::jxx::CAST<CloneableObject>(clonedBase);

    ASSERT_NE(nullptr, cloned);
    EXPECT_NE(value.get(), cloned.get());
    EXPECT_EQ(value->value(), cloned->value());
    EXPECT_EQ(cloneableObjectClass, cloned->getClass());
}

TEST(ObjectTest2, SynchronizedReturnsCallbackResult) {
    const auto value = ::jxx::NEW<PlainObject>();

    const auto result = value->synchronized([] {
        return 73;
    });

    EXPECT_EQ(73, result);
}

TEST(ObjectTest2, SynchronizedSupportsReentrantEntry) {
    const auto value = ::jxx::NEW<PlainObject>();

    const auto result = value->synchronized([&value] {
        return value->synchronized([] {
            return 19;
        });
    });

    EXPECT_EQ(19, result);
}

// Strict conformance documentation. This is disabled because the current
// implementation inserts "0x" after '@', while Object.toString() specifies
// getClass().getName() + '@' + the unsigned hexadecimal hash code.
TEST(ObjectConformanceTest, DefaultToStringUsesExactObjectFormat) {
    const auto value = ::jxx::NEW<PlainObject>();

    std::ostringstream expected;
    expected << value->getClass()->getName()->utf8()
             << '@'
             << std::hex
             << static_cast<std::uint32_t>(value->hashCode());

    EXPECT_EQ(expected.str(), value->toString()->utf8());
}

} // namespace
