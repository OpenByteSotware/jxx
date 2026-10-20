#include <gtest/gtest.h>

#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.ProcessBuilder.h"
#include "lang/jxx.lang.Void.h"
#include "jxx.initialize.h"

namespace
{
    template <typename T>
    void expectClassMetadata()
    {
        const auto type = T::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(LangClassInitialization, RegistersPublicNestedAndVoidClasses)
    {
        ::jxx::initialize();

        expectClassMetadata<::jxx::lang::Character>();
        expectClassMetadata<::jxx::lang::Character::Subset>();
        expectClassMetadata<::jxx::lang::Character::UnicodeBlock>();
        expectClassMetadata<::jxx::lang::ProcessBuilder>();
        expectClassMetadata<::jxx::lang::ProcessBuilder::Redirect>();
        expectClassMetadata<::jxx::lang::Void>();
    }
}
