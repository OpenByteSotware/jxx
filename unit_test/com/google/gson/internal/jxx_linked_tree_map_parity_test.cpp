#include <gtest/gtest.h>

#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"
#include "lang/jxx.lang.String.h"

namespace {
using String = ::jxx::lang::String;
using Map = ::com::google::gson::internal::LinkedTreeMap<String, String>;
}

TEST(LinkedTreeMapParity, NaturalOrderingDefinesKeyIdentity) {
    auto map = ::jxx::NEW<Map>();
    auto first = ::jxx::NEW<String>("alpha");
    auto equivalent = ::jxx::NEW<String>("alpha");
    auto oldValue = ::jxx::NEW<String>("old");
    auto newValue = ::jxx::NEW<String>("new");

    EXPECT_EQ(nullptr, map->put(first, oldValue));
    EXPECT_EQ(oldValue, map->put(equivalent, newValue));
    EXPECT_EQ(1, map->size());
    EXPECT_EQ(newValue, map->get(equivalent));
}

TEST(LinkedTreeMapParity, RejectsNullKeys) {
    auto map = ::jxx::NEW<Map>();
    EXPECT_THROW(map->put(nullptr, ::jxx::NEW<String>("value")),
                 ::jxx::lang::NullPointerException);
}
