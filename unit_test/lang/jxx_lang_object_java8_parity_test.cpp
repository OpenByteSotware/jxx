#include <gtest/gtest.h>

#include <type_traits>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalMonitorStateException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace {

class ObjectCloneProbe final : public ::jxx::lang::Object {
public:
    using ::jxx::lang::Object::clone;
};

static_assert(std::is_default_constructible_v<::jxx::lang::Object>);
static_assert(std::has_virtual_destructor_v<::jxx::lang::Object>);
static_assert(std::is_same_v<
    decltype(std::declval<const ::jxx::lang::Object&>().equals(
        std::declval<const ::jxx::Ptr<::jxx::lang::Object>&>())),
    ::jxx::lang::jbool>);
static_assert(std::is_same_v<
    decltype(std::declval<const ::jxx::lang::Object&>().hashCode()),
    ::jxx::lang::jint>);
static_assert(std::is_same_v<
    decltype(std::declval<const ::jxx::lang::Object&>().toString()),
    ::jxx::Ptr<::jxx::lang::String>>);

TEST(ObjectJava8Parity, DefaultEqualsUsesReferenceIdentity) {
    const auto first = ::jxx::NEW<::jxx::lang::Object>();
    const auto second = ::jxx::NEW<::jxx::lang::Object>();

    EXPECT_TRUE(first->equals(first));
    EXPECT_FALSE(first->equals(second));
    EXPECT_FALSE(first->equals(nullptr));
}

TEST(ObjectJava8Parity, GetClassAndToStringAreAvailable) {
    const auto object = ::jxx::NEW<::jxx::lang::Object>();

    EXPECT_NE(nullptr, object->getClass());
    const auto text = object->toString();
    ASSERT_NE(nullptr, text);
    EXPECT_NE(std::string::npos, text->utf8().find('@'));
}

TEST(ObjectJava8Parity, BaseCloneRejectsNonCloneableObject) {
    const auto object = ::jxx::NEW<ObjectCloneProbe>();
    EXPECT_THROW(object->clone(), ::jxx::lang::CloneNotSupportedException);
}

TEST(ObjectJava8Parity, WaitRejectsInvalidArguments) {
    const auto object = ::jxx::NEW<::jxx::lang::Object>();
    EXPECT_THROW(object->wait(-1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(object->wait(0, -1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(object->wait(0, 1000000), ::jxx::lang::IllegalArgumentException);
}

TEST(ObjectJava8Parity, NotifyRequiresMonitorOwnership) {
    const auto object = ::jxx::NEW<::jxx::lang::Object>();
    EXPECT_THROW(object->notify(), ::jxx::lang::IllegalMonitorStateException);
    EXPECT_THROW(object->notifyAll(), ::jxx::lang::IllegalMonitorStateException);
}

} // namespace
