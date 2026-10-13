#include <gtest/gtest.h>

#include "com/google/gson/reflect/jxx.com.google.gson.reflect.ClassInitialization.h"
#include "com/google/gson/reflect/jxx.com.google.gson.reflect.TypeToken.h"
#include "jxx.initialize.h"

namespace {

auto typeTokenClass()
{
    return ::jxx::lang::ClassBase<
        ::com::google::gson::reflect::TypeToken,
        ::jxx::lang::Object>::Class();
}

} // namespace

TEST(JxxGsonReflectClassInitialization, TypeTokenHasClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(typeTokenClass(), nullptr);
}

TEST(JxxGsonReflectClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = typeTokenClass();

    EXPECT_NO_THROW(::com::google::gson::reflect::initializeClasses());
    EXPECT_NO_THROW(::com::google::gson::reflect::initializeClasses());

    EXPECT_EQ(before, typeTokenClass());
}
