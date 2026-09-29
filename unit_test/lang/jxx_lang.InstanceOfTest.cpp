#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace {

class TestMarker
    : public jxx::lang::InterfaceBase<TestMarker> {
public:
    ~TestMarker() override = default;

    virtual void mark() = 0;
};

class TestBase
    : public jxx::lang::ClassBase<TestBase, jxx::lang::Object> {
public:
    ~TestBase() override = default;
};

class TestDerived final
    : public jxx::lang::ClassBase<TestDerived, TestBase, TestMarker> {
public:
    ~TestDerived() override = default;

    void mark() override {
    }
};

class TestSibling final
    : public jxx::lang::ClassBase<TestSibling, TestBase> {
public:
    ~TestSibling() override = default;
};

class Unrelated final
    : public jxx::lang::ClassBase<Unrelated, jxx::lang::Object> {
public:
    ~Unrelated() override = default;
};

TEST(InstanceOfTest, NullReferenceIsFalse) {
    const jxx::Ptr<jxx::lang::Object> value;

    EXPECT_FALSE(jxx::instanceOf<TestDerived>(value));
    EXPECT_FALSE(jxx::instanceOf<TestBase>(value));
    EXPECT_FALSE(jxx::instanceOf<TestMarker>(value));
    EXPECT_FALSE(jxx::instanceOf<jxx::lang::Object>(value));
}

TEST(InstanceOfTest, ExactRuntimeClassMatches) {
    const jxx::Ptr<jxx::lang::Object> value = jxx::NEW<TestDerived>();

    EXPECT_TRUE(jxx::instanceOf<TestDerived>(value));
    EXPECT_TRUE(value->instanceOf(TestDerived::Class()));
}

TEST(InstanceOfTest, SuperclassMatches) {
    const jxx::Ptr<jxx::lang::Object> value = jxx::NEW<TestDerived>();

    EXPECT_TRUE(jxx::instanceOf<TestBase>(value));
    EXPECT_TRUE(value->instanceOf(TestBase::Class()));
    EXPECT_TRUE(jxx::instanceOf<jxx::lang::Object>(value));
}

TEST(InstanceOfTest, ImplementedInterfaceMatches) {
    const jxx::Ptr<jxx::lang::Object> value = jxx::NEW<TestDerived>();

    EXPECT_TRUE(jxx::instanceOf<TestMarker>(value));
    EXPECT_TRUE(value->instanceOf(TestMarker::Class()));
}

TEST(InstanceOfTest, UnrelatedAndSiblingTypesDoNotMatch) {
    const jxx::Ptr<jxx::lang::Object> value = jxx::NEW<TestDerived>();

    EXPECT_FALSE(jxx::instanceOf<TestSibling>(value));
    EXPECT_FALSE(jxx::instanceOf<Unrelated>(value));
    EXPECT_FALSE(value->instanceOf(TestSibling::Class()));
    EXPECT_FALSE(value->instanceOf(Unrelated::Class()));
}

TEST(InstanceOfTest, NullClassDescriptorIsFalse) {
    const jxx::Ptr<jxx::lang::Object> value = jxx::NEW<TestDerived>();
    const jxx::Ptr<jxx::lang::ClassAny> target;

    EXPECT_FALSE(value->instanceOf(target));
}

TEST(InstanceOfTest, CompatibilitySpellingUsesMetadataSemantics) {
    const jxx::Ptr<jxx::lang::Object> value = jxx::NEW<TestDerived>();

    EXPECT_TRUE(jxx::instanceof<TestDerived>(value));
    EXPECT_TRUE(jxx::instanceof<TestBase>(value));
    EXPECT_TRUE(jxx::instanceof<TestMarker>(value));
    EXPECT_FALSE(jxx::instanceof<Unrelated>(value));
}

} // namespace
