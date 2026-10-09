#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

namespace {

class SystemPropertyGuard final {
public:
    explicit SystemPropertyGuard(const char* key)
        : key_(::jxx::NEW<::jxx::lang::String>(key)),
          previous_(::jxx::lang::System::getProperty(key_)) {
    }

    ~SystemPropertyGuard() {
        try {
            if (previous_ == nullptr) {
                (void)::jxx::lang::System::clearProperty(key_);
            }
            else {
                (void)::jxx::lang::System::setProperty(
                    key_,
                    previous_);
            }
        }
        catch (...) {
        }
    }

private:
    ::jxx::Ptr<::jxx::lang::String> key_;
    ::jxx::Ptr<::jxx::lang::String> previous_;
};

TEST(
    SystemPropertyParityTest,
    EquivalentStringInstanceFindsProperty)
{
    SystemPropertyGuard guard(
        "jxx.test.property.value.equality");

    const auto firstKey =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.test.property.value.equality");

    const auto equivalentKey =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.test.property.value.equality");

    ASSERT_NE(firstKey.get(), equivalentKey.get());
    ASSERT_TRUE(firstKey->equals(equivalentKey));
    ASSERT_EQ(firstKey->hashCode(), equivalentKey->hashCode());

    (void)::jxx::lang::System::setProperty(
        firstKey,
        ::jxx::NEW<::jxx::lang::String>(
            "expected-value"));

    const auto actual =
        ::jxx::lang::System::getProperty(
            equivalentKey);

    ASSERT_NE(nullptr, actual);
    EXPECT_EQ("expected-value", actual->utf8());

    const auto removed =
        ::jxx::lang::System::clearProperty(
            equivalentKey);

    ASSERT_NE(nullptr, removed);
    EXPECT_EQ("expected-value", removed->utf8());
    EXPECT_EQ(
        nullptr,
        ::jxx::lang::System::getProperty(firstKey));
}

} // namespace
