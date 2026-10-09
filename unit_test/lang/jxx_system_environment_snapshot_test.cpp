#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.Map.h"

namespace {
TEST(SystemEnvironmentSnapshotParity, NamedPathMatchesSnapshot) {
    const auto environment = ::jxx::lang::System::getenv();
    ASSERT_NE(nullptr, environment);
    const auto path = ::jxx::NEW<::jxx::lang::String>("PATH");
    const auto named = ::jxx::lang::System::getenv(path);
    const auto mapped = environment->get(
        ::jxx::CAST<::jxx::lang::Object>(path));
    if (named == nullptr) {
        EXPECT_EQ(nullptr, mapped);
    } else {
        ASSERT_NE(nullptr, mapped);
        EXPECT_EQ(named->utf8(), mapped->utf8());
    }
}
} // namespace
