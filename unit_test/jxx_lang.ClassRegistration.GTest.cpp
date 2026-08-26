#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassRegistration.h"

namespace jxx::test::registration {

class TestInterface
    : public jxx::lang::ClassInfo<TestInterface> {
public:
    virtual ~TestInterface() = default;
    virtual void execute() = 0;
};

class TestClass final
    : public jxx::lang::Object
    , public jxx::lang::ClassInfo<TestClass>
    , public virtual TestInterface {
public:
    TestClass() = default;
    void execute() override {}

protected:
    JXX_OBJECT_CLONE(TestClass)
};

} // namespace jxx::test::registration

namespace jxx::lang {
template <>
struct ClassInfo<jxx::test::registration::TestClass> {
    using JavaSuper = Object;
    static constexpr jint modifiers = 0x0001 | 0x0010;
    static constexpr jbool isEnum = false;
    static constexpr jbool isAnnotation = false;
    static constexpr jbool isSynthetic = false;
    static std::vector<jxx::Ptr<ClassAny>> interfaces() {
        return { jxx::test::registration::TestInterface::Class() };
    }
};
} // namespace jxx::lang

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
