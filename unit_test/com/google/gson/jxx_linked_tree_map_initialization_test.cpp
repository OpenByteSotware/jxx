#include <gtest/gtest.h>

#include "com/google/gson/internal/jxx.com.google.gson.internal.ClassInitialization.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "lang/jxx.lang.Object.h"

namespace
{
    using ErasedLinkedTreeMap =
        ::com::google::gson::internal::LinkedTreeMap<
            ::jxx::lang::Object, ::jxx::lang::Object>;

    TEST(LinkedTreeMapInitialization, RegistersErasedClassMetadata)
    {
        ::com::google::gson::internal::initializeClasses();
        const auto type = ErasedLinkedTreeMap::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(LinkedTreeMapSerializationInitialization, RegistersInheritedSerializableType)
    {
        ::com::google::gson::internal::initializeClasses();
        const auto metadata =
            ::jxx::io::serialization::SerializationClassRegistry::resolve(
                ErasedLinkedTreeMap::Class()->getName()->utf8());
        EXPECT_EQ(metadata.serialVersionUID, 1);
        EXPECT_TRUE(static_cast<bool>(metadata.deserializationFactory));
    }
}
