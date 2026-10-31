#include <gtest/gtest.h>
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Enum.h"

namespace {

class TestEnum final : public ::jxx::lang::Enum<TestEnum> {
public:
    using JxxSuper = ::jxx::lang::Enum<TestEnum>;

    TestEnum(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        ::jxx::lang::jint ordinal)
        : JxxSuper(name, ordinal) {}

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return ::jxx::lang::ClassInfo<TestEnum, JxxSuper>::Class();
    }

    static ::jxx::Ptr<::jxx::lang::JxxArray<
        ::jxx::Ptr<TestEnum>, 1U>> values() {
        const auto result = ::jxx::NEW<::jxx::lang::JxxArray<
            ::jxx::Ptr<TestEnum>, 1U>>(
                static_cast<::jxx::lang::jint>(2));
        (*result)[0] = FIRST;
        (*result)[1] = SECOND;
        return result;
    }

    inline static const ::jxx::Ptr<TestEnum> FIRST =
        ::jxx::NEW<TestEnum>(
            ::jxx::NEW<::jxx::lang::String>("FIRST"), 0);
    inline static const ::jxx::Ptr<TestEnum> SECOND =
        ::jxx::NEW<TestEnum>(
            ::jxx::NEW<::jxx::lang::String>("SECOND"), 1);
};

TEST(EnumValueOfArrayAccessTest, UsesCurrentRankOneJxxArrayApi) {
    EXPECT_EQ(TestEnum::SECOND,
        TestEnum::valueOf(
            TestEnum::Class(),
            ::jxx::NEW<::jxx::lang::String>("SECOND")));
}

} // namespace
