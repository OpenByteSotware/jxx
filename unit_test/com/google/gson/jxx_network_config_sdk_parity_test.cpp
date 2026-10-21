#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "io/jxx.io.FileReader.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.HashMap.h"

namespace {
namespace fs = std::filesystem;

using ErasedMap =
    ::com::google::gson::internal::LinkedTreeMap<
        ::jxx::lang::Object,
        ::jxx::lang::Object>;

using NetworkMap =
    ::jxx::util::HashMap<
        ::jxx::lang::String,
        ::com::google::gson::JsonObject>;

::jxx::Ptr<::jxx::lang::String> text(const char* value)
{
    return ::jxx::NEW<::jxx::lang::String>(value);
}

::jxx::Ptr<::jxx::lang::Object> objectKey(const char* value)
{
    return ::jxx::CAST<::jxx::lang::Object>(text(value));
}

::jxx::Ptr<::jxx::lang::String> stringValue(
    const ::jxx::Ptr<ErasedMap>& map,
    const char* key)
{
    if (map == nullptr) return nullptr;
    return ::jxx::CAST<::jxx::lang::String>(
        map->get(objectKey(key)));
}

class NetworkConfigSdkParityTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const auto suite =
            ::testing::UnitTest::GetInstance()->current_test_info();
        const auto name = std::string("jxx_network_config_sdk_") +
            suite->test_suite_name() + "_" + suite->name();
        file_ = fs::temp_directory_path() / (name + ".json");
    }

    void TearDown() override
    {
        std::error_code ignored;
        fs::remove(file_, ignored);
    }

    void writeJson(const std::string& json)
    {
        std::ofstream output(file_, std::ios::binary | std::ios::trunc);
        ASSERT_TRUE(output.is_open());
        output << json;
        output.flush();
        ASSERT_TRUE(output.good());
    }

    ::jxx::Ptr<ErasedMap> parseFile()
    {
        const auto fileReader = ::jxx::NEW<::jxx::io::FileReader>(
            ::jxx::NEW<::jxx::lang::String>(file_.string()));
        const auto jsonReader =
            ::jxx::NEW<::com::google::gson::stream::JsonReader>(
                ::jxx::CAST<::jxx::io::Reader>(fileReader));
        const auto gson = ::jxx::NEW<::com::google::gson::Gson>();

        const auto result = ::jxx::CAST<ErasedMap>(
            gson->fromJson(jsonReader, ErasedMap::Class()));

        jsonReader->close();
        fileReader->close();
        return result;
    }

    fs::path file_;
};

TEST_F(NetworkConfigSdkParityTest, ParsesFileIntoRawNestedLinkedTreeMaps)
{
    writeJson(R"json({
        "group1": {
            "ip_address": "10.20.30.40 /24",
            "subnet_id": "17"
        }
    })json");

    const auto contents = parseFile();
    ASSERT_NE(contents, nullptr);
    ASSERT_EQ(contents->size(), 1);

    const auto group = ::jxx::CAST<ErasedMap>(
        contents->get(objectKey("group1")));
    ASSERT_NE(group, nullptr);
    ASSERT_NE(stringValue(group, "ip_address"), nullptr);
    ASSERT_NE(stringValue(group, "subnet_id"), nullptr);
    EXPECT_EQ(stringValue(group, "ip_address")->utf8(),
              "10.20.30.40 /24");
    EXPECT_EQ(stringValue(group, "subnet_id")->utf8(), "17");
}

TEST_F(NetworkConfigSdkParityTest, IteratesOuterEntriesInInsertionOrder)
{
    writeJson(R"json({
        "first": {"ip_address": "10.1.0.1 /24", "subnet_id": "1"},
        "second": {"ip_address": "10.2.0.1 /24", "subnet_id": "2"}
    })json");

    const auto contents = parseFile();
    ASSERT_NE(contents, nullptr);

    const auto iterator = contents->entrySet()->iterator();
    ASSERT_TRUE(iterator->hasNext());
    EXPECT_EQ(
        ::jxx::CAST<::jxx::lang::String>(
            iterator->next()->getKey())->utf8(),
        "first");
    ASSERT_TRUE(iterator->hasNext());
    EXPECT_EQ(
        ::jxx::CAST<::jxx::lang::String>(
            iterator->next()->getKey())->utf8(),
        "second");
    EXPECT_FALSE(iterator->hasNext());
}

TEST_F(NetworkConfigSdkParityTest, SupportsCaseInsensitiveRedirectorKeyChecks)
{
    writeJson(R"json({
        "NeWgSuB1Ip": {"ip_address": "10.1.2.3 /24", "subnet_id": "11"}
    })json");

    const auto contents = parseFile();
    ASSERT_NE(contents, nullptr);
    const auto iterator = contents->entrySet()->iterator();
    ASSERT_TRUE(iterator->hasNext());

    const auto key = ::jxx::CAST<::jxx::lang::String>(
        iterator->next()->getKey());
    ASSERT_NE(key, nullptr);
    EXPECT_TRUE(key->equalsIgnoreCase(text("newgsub1ip")));
}

TEST_F(NetworkConfigSdkParityTest, SplitsCidrSuffixUsingRankOneJxxArray)
{
    const auto address = text("192.168.8.31 /27");
    const auto parts = address->split(text(" /"));

    ASSERT_NE(parts, nullptr);
    ASSERT_GE(parts->length, 1);
    ASSERT_NE((*parts)[0], nullptr);
    EXPECT_EQ((*parts)[0]->utf8(), "192.168.8.31");
}

TEST_F(NetworkConfigSdkParityTest, BuildsJsonObjectWithStringAndBooleanPrimitives)
{
    const auto network =
        ::jxx::NEW<::com::google::gson::JsonObject>();

    network->add(
        text("ip_address"),
        ::jxx::NEW<::com::google::gson::JsonPrimitive>(
            text("10.20.30.40")));
    network->add(
        text("subnet_id"),
        ::jxx::NEW<::com::google::gson::JsonPrimitive>(
            text("17")));
    network->add(
        text("is_redirector"),
        ::jxx::NEW<::com::google::gson::JsonPrimitive>(true));

    ASSERT_NE(network->get(text("ip_address")), nullptr);
    ASSERT_NE(network->get(text("subnet_id")), nullptr);
    ASSERT_NE(network->get(text("is_redirector")), nullptr);
    EXPECT_EQ(network->get(text("ip_address"))->getAsString()->utf8(),
              "10.20.30.40");
    EXPECT_EQ(network->get(text("subnet_id"))->getAsString()->utf8(),
              "17");
    EXPECT_TRUE(network->get(text("is_redirector"))->getAsBoolean());
}

TEST_F(NetworkConfigSdkParityTest, StoresJsonObjectsInTypedHashMap)
{
    const auto networks = ::jxx::NEW<NetworkMap>();
    const auto network =
        ::jxx::NEW<::com::google::gson::JsonObject>();
    network->add(
        text("subnet_id"),
        ::jxx::NEW<::com::google::gson::JsonPrimitive>(text("17")));

    networks->put(text("10.20.30.0"), network);

    ASSERT_EQ(networks->size(), 1);
    const auto stored = networks->get(objectKey("10.20.30.0"));
    ASSERT_NE(stored, nullptr);
    EXPECT_EQ(stored->get(text("subnet_id"))->getAsString()->utf8(),
              "17");
}

TEST_F(NetworkConfigSdkParityTest, EmptyObjectProducesEmptyRawMap)
{
    writeJson("{}");
    const auto contents = parseFile();
    ASSERT_NE(contents, nullptr);
    EXPECT_TRUE(contents->isEmpty());
}

TEST_F(NetworkConfigSdkParityTest, ClosingJsonReaderAlsoClosesUnderlyingReader)
{
    writeJson("{}");

    const auto fileReader = ::jxx::NEW<::jxx::io::FileReader>(
        ::jxx::NEW<::jxx::lang::String>(file_.string()));
    const auto jsonReader =
        ::jxx::NEW<::com::google::gson::stream::JsonReader>(
            ::jxx::CAST<::jxx::io::Reader>(fileReader));

    jsonReader->close();
    EXPECT_NO_THROW(fileReader->close());
}

} // namespace
