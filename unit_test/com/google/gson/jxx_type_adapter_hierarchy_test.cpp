#include <gtest/gtest.h>
#include <type_traits>

#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.NullSafeTypeAdapter.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.TreeTypeAdapter.h"
#include "lang/jxx.lang.Object.h"

TEST(TypeAdapterHierarchyParity, IsAbstractClassNotInterface) {
    using Adapter = ::com::google::gson::TypeAdapter;
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, Adapter>));
    EXPECT_TRUE(std::is_abstract_v<Adapter>);
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::InterfaceBase<Adapter>, Adapter>));
}

TEST(TypeAdapterHierarchyParity, ImplementationsExtendTypeAdapter) {
    EXPECT_TRUE((std::is_base_of_v<
        ::com::google::gson::TypeAdapter,
        ::com::google::gson::internal::NullSafeTypeAdapter>));
    EXPECT_TRUE((std::is_base_of_v<
        ::com::google::gson::TypeAdapter,
        ::com::google::gson::internal::TreeTypeAdapter>));
}
