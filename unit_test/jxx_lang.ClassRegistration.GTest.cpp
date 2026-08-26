#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassRegistration.h"

namespace jxx::test::registration {

class TestInterface
    : public jxx::lang::ClassInfo<TestInterface> {
public:
    virtual ~TestInterface() = default;
    virtual void execute() = 0;
};

class TestClass final:
    public jxx::lang::ClassBase<TestClass, jxx::lang::Object, TestInterface>
{
public:
    TestClass() = default;
    void execute() override {}

protected:
    JXX_OBJECT_CLONE(TestClass)
};

} // namespace jxx::test::registration


namespace {
using jxx::test::registration::TestClass;
using jxx::test::registration::TestInterface;

TEST(ClassRegistrationTest, RegistersClassExplicitly) {
    auto descriptor = jxx::lang::registerClass<TestClass>();
    ASSERT_NE(descriptor, nullptr);
    EXPECT_FALSE(descriptor->isInterface());
    EXPECT_TRUE(jxx::lang::isClassRegistered<TestClass>());
}

TEST(ClassRegistrationTest, RegistersInterfaceExplicitly) {
    auto descriptor = jxx::lang::registerClass<TestInterface>();
    ASSERT_NE(descriptor, nullptr);
    EXPECT_TRUE(descriptor->isInterface());
    EXPECT_TRUE(jxx::lang::isClassRegistered<TestInterface>());
}

TEST(ClassRegistrationTest, ClassOfReturnsCanonicalDescriptor) {
    EXPECT_EQ(jxx::lang::classOf<TestClass>(), TestClass::Class());
}

TEST(ClassRegistrationTest, NewCreatesRegisteredClass) {
    auto object = jxx::NEW<TestClass>();
    ASSERT_NE(object, nullptr);
    EXPECT_EQ(object->getClass(), TestClass::Class());
}

TEST(ClassRegistrationTest, InterfacePredicateSupportsCrossCast) {
    auto value = jxx::NEW<TestClass>();
    auto object = jxx::CAST<jxx::lang::Object>(value);
    EXPECT_TRUE(TestInterface::Class()->isInstance(object));
}
} // namespace
