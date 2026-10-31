#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Enum.h"
#include "lang/jxx.lang.String.h"

namespace
{

    class TestEnum final : public ::jxx::lang::Enum<TestEnum>
    {
    public:
        using JxxSuper = ::jxx::lang::Enum<TestEnum>;

        TestEnum(
            const ::jxx::Ptr<::jxx::lang::String>& name,
            ::jxx::lang::jint ordinal)
            : JxxSuper(name, ordinal)
        {
        }

        static ::jxx::Ptr<::jxx::lang::ClassAny> Class()
        {
            return ::jxx::lang::ClassInfo<TestEnum, JxxSuper>::Class();
        }

        static ::jxx::Ptr<
            ::jxx::JxxArray<::jxx::Ptr<TestEnum>, 1U>> values()
        {
            auto result = ::jxx::NEW<
                ::jxx::JxxArray<::jxx::Ptr<TestEnum>, 1U>>(
                    static_cast<::jxx::lang::jint>(2));

            (*result)(0) = FIRST;
            (*result)(1) = SECOND;
            return result;
        }

        static const ::jxx::Ptr<TestEnum> FIRST;
        static const ::jxx::Ptr<TestEnum> SECOND;
    };

    const ::jxx::Ptr<TestEnum> TestEnum::FIRST =
        ::jxx::NEW<TestEnum>(
            ::jxx::NEW<::jxx::lang::String>("FIRST"),
            static_cast<::jxx::lang::jint>(0));

    const ::jxx::Ptr<TestEnum> TestEnum::SECOND =
        ::jxx::NEW<TestEnum>(
            ::jxx::NEW<::jxx::lang::String>("SECOND"),
            static_cast<::jxx::lang::jint>(1));

    TEST(EnumGenericArrayAccessTest, ValueOfReadsCurrentRankOneJxxArray)
    {
        EXPECT_EQ(
            TestEnum::SECOND,
            TestEnum::valueOf(
                TestEnum::Class(),
                ::jxx::NEW<::jxx::lang::String>("SECOND")));
    }

} // namespace
