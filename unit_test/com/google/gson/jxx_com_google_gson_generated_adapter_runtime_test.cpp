#include <gtest/gtest.h>

#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "lang/jxx.lang.String.h"
#include "generated_binding_adapters.h"

TEST(GsonGeneratedAdapterRuntimeTest, DeserializesApplicationObject) {
    const auto gson = ::jxx::NEW<::com::google::gson::Gson>();
    const auto value = gson->fromJson(
        ::jxx::NEW<::jxx::lang::String>(R"({"port":1373})"),
        ::jxx::test::generated::portJsonAdapter());

    EXPECT_EQ(
        1373,
        ::jxx::test::generated::portFromModel(value));
}

TEST(GsonGeneratedAdapterRuntimeTest, SerializesApplicationObject) {
    const auto gson = ::jxx::NEW<::com::google::gson::Gson>();
    const auto value =
        ::jxx::test::generated::newPortModel(1373);

    const auto json = gson->toJson(
        value,
        ::jxx::test::generated::portJsonAdapter());

    EXPECT_TRUE(json->utf8() == R"({"port":1373})");
}
