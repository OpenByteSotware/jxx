#include <memory>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Hashtable.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Properties.h"
#include "util/jxx.util.Set.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "io/jxx.io.Writer.h"
#include "io/jxx.io.Reader.h"

namespace {

    using jxx::lang::Object;
    using jxx::lang::String;
    using jxx::util::Enumeration;
    using jxx::util::Iterator;
    using jxx::util::Properties;
    using jxx::util::Set;

    static jxx::Ptr<String> S(const char* value) {
        return jxx::NEW<String>(value);
    }

    static jxx::Ptr<String> S(const std::string& value) {
        return jxx::NEW<String>(value);
    }

    static std::string textOf(
        const jxx::Ptr<String>& value) {

        if (value == nullptr) {
            return {};
        }

        return value->utf8();
    }

    static jxx::Ptr<Object> asObject(
        const jxx::Ptr<String>& value) {

        return jxx::CAST<Object>(value);
    }

    static jxx::Ptr<String> asString(
        const jxx::Ptr<Object>& value) {

        return jxx::CAST<String>(value);
    }

    static std::set<std::string> collectPropertyNames(
        const jxx::Ptr<Properties>& properties) {

        std::set<std::string> result;

        auto names = properties->propertyNames();

        if (names == nullptr) {
            return result;
        }

        while (names->hasMoreElements()) {
            auto object = names->nextElement();
            auto stringValue = asString(object);

            if (stringValue != nullptr) {
                result.insert(stringValue->utf8());
            }
        }

        return result;
    }

    static std::set<std::string> collectStringPropertyNames(
        const jxx::Ptr<Properties>& properties) {

        std::set<std::string> result;

        auto names = properties->stringPropertyNames();

        if (names == nullptr) {
            return result;
        }

        auto iterator = names->iterator();

        while (iterator->hasNext()) {
            auto value = iterator->next();

            if (value != nullptr) {
                result.insert(value->utf8());
            }
        }

        return result;
    }

    /*
     * Class hierarchy
     */

    TEST(PropertiesTest, ExtendsHashtableObjectObject) {
        static_assert(
            std::is_base_of_v<
            jxx::util::Hashtable<Object, Object>,
            Properties>,
            "Properties must extend Hashtable<Object,Object>");

        SUCCEED();
    }

    /*
     * Constructors
     */

    TEST(PropertiesTest, DefaultConstructorCreatesEmptyProperties) {
        auto properties = jxx::NEW<Properties>();

        ASSERT_NE(properties, nullptr);
        EXPECT_EQ(properties->size(), 0);
        EXPECT_TRUE(properties->isEmpty());
    }

    TEST(PropertiesTest, DefaultsConstructorCreatesEmptyLocalTable) {
        auto defaults = jxx::NEW<Properties>();

        defaults->setProperty(
            S("default.key"),
            S("default.value"));

        auto properties =
            jxx::NEW<Properties>(defaults);

        ASSERT_NE(properties, nullptr);

        // Defaults are not copied into the local Hashtable.
        EXPECT_EQ(properties->size(), 0);
        EXPECT_TRUE(properties->isEmpty());

        // They are still visible through getProperty().
        EXPECT_EQ(
            textOf(properties->getProperty(
                S("default.key"))),
            "default.value");
    }

    /*
     * setProperty
     */

    TEST(PropertiesTest, SetPropertyAddsNewProperty) {
        auto properties = jxx::NEW<Properties>();

        auto previous = properties->setProperty(
            S("name"),
            S("value"));

        EXPECT_EQ(previous, nullptr);
        EXPECT_EQ(properties->size(), 1);

        EXPECT_EQ(
            textOf(properties->getProperty(S("name"))),
            "value");
    }

    TEST(PropertiesTest, SetPropertyReplacesExistingProperty) {
        auto properties = jxx::NEW<Properties>();

        properties->setProperty(
            S("name"),
            S("first"));

        auto previous = properties->setProperty(
            S("name"),
            S("second"));

        ASSERT_NE(previous, nullptr);

        auto previousString =
            asString(previous);

        ASSERT_NE(previousString, nullptr);
        EXPECT_EQ(previousString->utf8(), "first");

        EXPECT_EQ(
            textOf(properties->getProperty(S("name"))),
            "second");

        EXPECT_EQ(properties->size(), 1);
    }

    TEST(PropertiesTest, SetPropertyNullKeyThrows) {
        auto properties = jxx::NEW<Properties>();

        EXPECT_THROW(
            properties->setProperty(
                nullptr,
                S("value")),
            jxx::lang::NullPointerException);
    }

    TEST(PropertiesTest, SetPropertyNullValueThrows) {
        auto properties = jxx::NEW<Properties>();

        EXPECT_THROW(
            properties->setProperty(
                S("key"),
                nullptr),
            jxx::lang::NullPointerException);
    }

    /*
     * getProperty
     */

    TEST(PropertiesTest, GetPropertyReturnsLocalValue) {
        auto properties = jxx::NEW<Properties>();

        properties->setProperty(
            S("ip_address"),
            S("192.168.1.10"));

        auto result =
            properties->getProperty(
                S("ip_address"));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "192.168.1.10");
    }

    TEST(PropertiesTest, GetPropertyReturnsNullWhenMissing) {
        auto properties = jxx::NEW<Properties>();

        EXPECT_EQ(
            properties->getProperty(S("missing")),
            nullptr);
    }

    TEST(PropertiesTest, GetPropertyReturnsSuppliedDefaultWhenMissing) {
        auto properties = jxx::NEW<Properties>();

        auto result = properties->getProperty(
            S("missing"),
            S("fallback"));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "fallback");
    }

    TEST(PropertiesTest, GetPropertyDoesNotUseSuppliedDefaultWhenPresent) {
        auto properties = jxx::NEW<Properties>();

        properties->setProperty(
            S("mode"),
            S("production"));

        auto result = properties->getProperty(
            S("mode"),
            S("development"));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "production");
    }

    TEST(PropertiesTest, GetPropertyNullKeyThrows) {
        auto properties = jxx::NEW<Properties>();

        EXPECT_THROW(
            properties->getProperty(nullptr),
            jxx::lang::NullPointerException);
    }

    /*
     * Defaults behavior
     */

    TEST(PropertiesTest, GetPropertyFallsBackToDefaults) {
        auto defaults = jxx::NEW<Properties>();

        defaults->setProperty(
            S("subnet_id"),
            S("42"));

        auto properties =
            jxx::NEW<Properties>(defaults);

        auto result =
            properties->getProperty(
                S("subnet_id"));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "42");
    }

    TEST(PropertiesTest, LocalPropertyOverridesDefaultProperty) {
        auto defaults = jxx::NEW<Properties>();

        defaults->setProperty(
            S("environment"),
            S("default"));

        auto properties =
            jxx::NEW<Properties>(defaults);

        properties->setProperty(
            S("environment"),
            S("local"));

        EXPECT_EQ(
            textOf(properties->getProperty(
                S("environment"))),
            "local");
    }

    TEST(PropertiesTest, MultipleDefaultLevelsAreSearched) {
        auto rootDefaults = jxx::NEW<Properties>();

        rootDefaults->setProperty(
            S("root"),
            S("root-value"));

        auto middleDefaults =
            jxx::NEW<Properties>(rootDefaults);

        middleDefaults->setProperty(
            S("middle"),
            S("middle-value"));

        auto properties =
            jxx::NEW<Properties>(middleDefaults);

        EXPECT_EQ(
            textOf(properties->getProperty(S("root"))),
            "root-value");

        EXPECT_EQ(
            textOf(properties->getProperty(S("middle"))),
            "middle-value");
    }

    TEST(PropertiesTest, NearestDefaultOverridesEarlierDefault) {
        auto rootDefaults = jxx::NEW<Properties>();

        rootDefaults->setProperty(
            S("shared"),
            S("root"));

        auto middleDefaults =
            jxx::NEW<Properties>(rootDefaults);

        middleDefaults->setProperty(
            S("shared"),
            S("middle"));

        auto properties =
            jxx::NEW<Properties>(middleDefaults);

        EXPECT_EQ(
            textOf(properties->getProperty(S("shared"))),
            "middle");
    }

    /*
     * Inherited Hashtable behavior
     */

    TEST(PropertiesTest, SetPropertyCanBeReadThroughHashtableGet) {
        auto properties = jxx::NEW<Properties>();

        properties->setProperty(
            S("key"),
            S("value"));

        auto objectValue = properties->get(
            asObject(S("key")));

        auto stringValue =
            asString(objectValue);

        ASSERT_NE(stringValue, nullptr);
        EXPECT_EQ(stringValue->utf8(), "value");
    }

    TEST(PropertiesTest, RemoveInheritedFromHashtableRemovesProperty) {
        auto properties = jxx::NEW<Properties>();

        properties->setProperty(
            S("key"),
            S("value"));

        auto removed = properties->remove(
            asObject(S("key")));

        auto removedString =
            asString(removed);

        ASSERT_NE(removedString, nullptr);
        EXPECT_EQ(removedString->utf8(), "value");

        EXPECT_EQ(
            properties->getProperty(S("key")),
            nullptr);
    }

    TEST(PropertiesTest, ClearRemovesAllLocalProperties) {
        auto properties = jxx::NEW<Properties>();

        properties->setProperty(S("one"), S("1"));
        properties->setProperty(S("two"), S("2"));
        properties->setProperty(S("three"), S("3"));

        ASSERT_EQ(properties->size(), 3);

        properties->clear();

        EXPECT_EQ(properties->size(), 0);
        EXPECT_TRUE(properties->isEmpty());
    }

    TEST(PropertiesTest, ClearDoesNotClearDefaults) {
        auto defaults = jxx::NEW<Properties>();

        defaults->setProperty(
            S("default"),
            S("still-present"));

        auto properties =
            jxx::NEW<Properties>(defaults);

        properties->setProperty(
            S("local"),
            S("removed"));

        properties->clear();

        EXPECT_EQ(
            properties->getProperty(S("local")),
            nullptr);

        EXPECT_EQ(
            textOf(properties->getProperty(S("default"))),
            "still-present");
    }

    /*
     * propertyNames
     */

    TEST(PropertiesTest, PropertyNamesIncludesLocalNames) {
        auto properties = jxx::NEW<Properties>();

        properties->setProperty(S("one"), S("1"));
        properties->setProperty(S("two"), S("2"));

        const auto names =
            collectPropertyNames(properties);

        EXPECT_EQ(names.size(), 2U);
        EXPECT_EQ(names.count("one"), 1U);
        EXPECT_EQ(names.count("two"), 1U);
    }

    TEST(PropertiesTest, PropertyNamesIncludesDefaultNames) {
        auto defaults = jxx::NEW<Properties>();

        defaults->setProperty(
            S("default.one"),
            S("1"));

        defaults->setProperty(
            S("default.two"),
            S("2"));

        auto properties =
            jxx::NEW<Properties>(defaults);

        properties->setProperty(
            S("local"),
            S("3"));

        const auto names =
            collectPropertyNames(properties);

        EXPECT_EQ(names.size(), 3U);
        EXPECT_EQ(names.count("default.one"), 1U);
        EXPECT_EQ(names.count("default.two"), 1U);
        EXPECT_EQ(names.count("local"), 1U);
    }

    TEST(PropertiesTest, PropertyNamesDoesNotDuplicateOverriddenName) {
        auto defaults = jxx::NEW<Properties>();

        defaults->setProperty(
            S("shared"),
            S("default"));

        auto properties =
            jxx::NEW<Properties>(defaults);

        properties->setProperty(
            S("shared"),
            S("local"));

        const auto names =
            collectPropertyNames(properties);

        EXPECT_EQ(names.size(), 1U);
        EXPECT_EQ(names.count("shared"), 1U);
    }

    /*
     * stringPropertyNames
     */

    TEST(PropertiesTest, StringPropertyNamesIncludesLocalStringNames) {
        auto properties = jxx::NEW<Properties>();

        properties->setProperty(S("one"), S("1"));
        properties->setProperty(S("two"), S("2"));

        const auto names =
            collectStringPropertyNames(properties);

        EXPECT_EQ(names.size(), 2U);
        EXPECT_EQ(names.count("one"), 1U);
        EXPECT_EQ(names.count("two"), 1U);
    }

    TEST(PropertiesTest, StringPropertyNamesIncludesDefaults) {
        auto defaults = jxx::NEW<Properties>();

        defaults->setProperty(
            S("default"),
            S("default-value"));

        auto properties =
            jxx::NEW<Properties>(defaults);

        properties->setProperty(
            S("local"),
            S("local-value"));

        const auto names =
            collectStringPropertyNames(properties);

        EXPECT_EQ(names.size(), 2U);
        EXPECT_EQ(names.count("default"), 1U);
        EXPECT_EQ(names.count("local"), 1U);
    }

    TEST(PropertiesTest, StringPropertyNamesDoesNotDuplicateOverride) {
        auto defaults = jxx::NEW<Properties>();

        defaults->setProperty(
            S("shared"),
            S("default"));

        auto properties =
            jxx::NEW<Properties>(defaults);

        properties->setProperty(
            S("shared"),
            S("local"));

        const auto names =
            collectStringPropertyNames(properties);

        EXPECT_EQ(names.size(), 1U);
        EXPECT_EQ(names.count("shared"), 1U);
    }

    /*
     * Null I/O validation
     */

    TEST(PropertiesTest, LoadNullReaderThrows) {
        auto properties = jxx::NEW<Properties>();

        jxx::Ptr<jxx::io::Reader> reader = nullptr;

        EXPECT_THROW(
            properties->load(reader),
            jxx::lang::NullPointerException);
    }

    TEST(PropertiesTest, LoadNullInputStreamThrows) {
        auto properties = jxx::NEW<Properties>();

        jxx::Ptr<jxx::io::InputStream> stream = nullptr;

        EXPECT_THROW(
            properties->load(stream),
            jxx::lang::NullPointerException);
    }

    TEST(PropertiesTest, StoreNullWriterThrows) {
        auto properties = jxx::NEW<Properties>();

        jxx::Ptr<jxx::io::Writer> writer = nullptr;

        EXPECT_THROW(
            properties->store(
                writer,
                S("comment")),
            jxx::lang::NullPointerException);
    }

    TEST(PropertiesTest, StoreNullOutputStreamThrows) {
        auto properties = jxx::NEW<Properties>();

        jxx::Ptr<jxx::io::OutputStream> stream = nullptr;

        EXPECT_THROW(
            properties->store(
                stream,
                S("comment")),
            jxx::lang::NullPointerException);
    }

    TEST(PropertiesTest, LoadFromXmlNullStreamThrows) {
        auto properties = jxx::NEW<Properties>();

        jxx::Ptr<jxx::io::InputStream> stream = nullptr;

        EXPECT_THROW(
            properties->loadFromXML(stream),
            jxx::lang::NullPointerException);
    }

    TEST(PropertiesTest, StoreToXmlNullStreamThrows) {
        auto properties = jxx::NEW<Properties>();

        jxx::Ptr<jxx::io::OutputStream> stream = nullptr;

        EXPECT_THROW(
            properties->storeToXML(
                stream,
                S("comment")),
            jxx::lang::NullPointerException);
    }

    TEST(PropertiesTest, StoreToXmlNullEncodingThrows) {
        auto properties = jxx::NEW<Properties>();

        /*
         * The null output stream is also invalid, so this test only verifies
         * that the method rejects invalid input. Use a real memory output
         * stream to isolate the encoding argument when one is available.
         */
        jxx::Ptr<jxx::io::OutputStream> stream = nullptr;

        EXPECT_THROW(
            properties->storeToXML(
                stream,
                S("comment"),
                nullptr),
            jxx::lang::NullPointerException);
    }

    TEST(PropertiesTest, GetPropertyIgnoresNonStringValue) {
        auto properties = jxx::NEW<Properties>();

        auto key = S("object-value");

        properties->put(
            asObject(key),
            jxx::NEW<Object>());

        EXPECT_EQ(
            properties->getProperty(key),
            nullptr);
    }

} // namespace
