#include <atomic>
#include <limits>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Dictionary.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Hashtable.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Set.h"

namespace {

    using jxx::lang::Object;
    using jxx::lang::String;
    using jxx::lang::jbool;
    using jxx::lang::jfloat;
    using jxx::lang::jint;

    using jxx::util::Collection;
    using jxx::util::Dictionary;
    using jxx::util::Enumeration;
    using jxx::util::Hashtable;
    using jxx::util::Iterator;
    using jxx::util::Map;
    using jxx::util::MapEntry;
    using jxx::util::Set;

    using StringHashtable =
        Hashtable<String, String>;

    static jxx::Ptr<String> S(const char* value) {
        return jxx::NEW<String>(value);
    }

    static jxx::Ptr<String> S(const std::string& value) {
        return jxx::NEW<String>(value);
    }

    static jxx::Ptr<Object> asObject(
        const jxx::Ptr<String>& value) {

        return jxx::CAST<Object>(value);
    }

    static jxx::Ptr<Object> asObject(
        const jxx::Ptr<StringHashtable>& value) {

        return jxx::CAST<Object>(value);
    }

    static std::string textOf(
        const jxx::Ptr<String>& value) {

        return value == nullptr
            ? std::string()
            : value->utf8();
    }

    static std::set<std::string> collectKeys(
        const jxx::Ptr<StringHashtable>& table) {

        std::set<std::string> result;

        auto enumeration = table->keys();

        while (enumeration->hasMoreElements()) {
            auto key = enumeration->nextElement();

            if (key != nullptr) {
                result.insert(key->utf8());
            }
        }

        return result;
    }

    static std::multiset<std::string> collectValues(
        const jxx::Ptr<StringHashtable>& table) {

        std::multiset<std::string> result;

        auto enumeration = table->elements();

        while (enumeration->hasMoreElements()) {
            auto value = enumeration->nextElement();

            if (value != nullptr) {
                result.insert(value->utf8());
            }
        }

        return result;
    }

    /*
     * Type hierarchy
     */

    TEST(HashtableTest, MatchesJxxJavaHierarchy) {
        static_assert(
            std::is_base_of_v<
            Dictionary<String, String>,
            StringHashtable>,
            "Hashtable<K,V> must extend Dictionary<K,V>");

        static_assert(
            std::is_base_of_v<
            Map<String, String>,
            StringHashtable>,
            "Hashtable<K,V> must implement Map<K,V>");

        static_assert(
            std::is_base_of_v<
            jxx::lang::Cloneable,
            StringHashtable>,
            "Hashtable<K,V> must implement Cloneable");

        static_assert(
            std::is_base_of_v<
            jxx::io::Serializable,
            StringHashtable>,
            "Hashtable<K,V> must implement Serializable");

        SUCCEED();
    }

    /*
     * Constructors
     */

    TEST(HashtableTest, DefaultConstructorCreatesEmptyTable) {
        auto table = jxx::NEW<StringHashtable>();

        ASSERT_NE(table, nullptr);
        EXPECT_EQ(table->size(), 0);
        EXPECT_TRUE(table->isEmpty());
    }

    TEST(HashtableTest, InitialCapacityConstructorCreatesEmptyTable) {
        auto table =
            jxx::NEW<StringHashtable>(
                static_cast<jint>(32));

        ASSERT_NE(table, nullptr);
        EXPECT_EQ(table->size(), 0);
        EXPECT_TRUE(table->isEmpty());
    }

    TEST(HashtableTest, ZeroInitialCapacityIsAccepted) {
        auto table =
            jxx::NEW<StringHashtable>(
                static_cast<jint>(0));

        ASSERT_NE(table, nullptr);
        EXPECT_TRUE(table->isEmpty());

        EXPECT_EQ(
            table->put(S("key"), S("value")),
            nullptr);

        EXPECT_EQ(table->size(), 1);
    }

    TEST(HashtableTest, NegativeInitialCapacityThrows) {
        EXPECT_THROW(
            jxx::NEW<StringHashtable>(
                static_cast<jint>(-1)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(HashtableTest, CapacityAndLoadFactorConstructorWorks) {
        auto table =
            jxx::NEW<StringHashtable>(
                static_cast<jint>(17),
                static_cast<float>(0.50f));

        ASSERT_NE(table, nullptr);
        EXPECT_TRUE(table->isEmpty());

        EXPECT_EQ(
            table->put(S("key"), S("value")),
            nullptr);
    }

    TEST(HashtableTest, ZeroLoadFactorThrows) {
        EXPECT_THROW(
            jxx::NEW<StringHashtable>(
                static_cast<jint>(11),
                0.0f),
            jxx::lang::IllegalArgumentException);
    }

    TEST(HashtableTest, NegativeLoadFactorThrows) {
        EXPECT_THROW(
            jxx::NEW<StringHashtable>(
                static_cast<jint>(11),
                -0.75f),
            jxx::lang::IllegalArgumentException);
    }

    TEST(HashtableTest, NaNLoadFactorThrowsForJavaParity) {
        const float nanValue =
            std::numeric_limits<float>::quiet_NaN();

        /*
         * Java rejects NaN because !(loadFactor > 0) is true.
         */
        EXPECT_THROW(
            jxx::NEW<StringHashtable>(
                static_cast<jint>(11),
                nanValue),
            jxx::lang::IllegalArgumentException);
    }

    /*
     * put/get
     */

    TEST(HashtableTest, PutAddsNewMapping) {
        auto table = jxx::NEW<StringHashtable>();

        auto previous =
            table->put(S("key"), S("value"));

        EXPECT_EQ(previous, nullptr);
        EXPECT_EQ(table->size(), 1);
        EXPECT_FALSE(table->isEmpty());

        EXPECT_EQ(
            textOf(table->get(asObject(S("key")))),
            "value");
    }

    TEST(HashtableTest, PutReplacesExistingMapping) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("first"));

        auto previous =
            table->put(S("key"), S("second"));

        ASSERT_NE(previous, nullptr);
        EXPECT_EQ(previous->utf8(), "first");
        EXPECT_EQ(table->size(), 1);

        EXPECT_EQ(
            textOf(table->get(asObject(S("key")))),
            "second");
    }

    TEST(HashtableTest, PutUsesEqualsAndHashCodeForKeys) {
        auto table = jxx::NEW<StringHashtable>();

        auto insertedKey = S("same-key");
        auto lookupKey = S("same-key");

        ASSERT_NE(insertedKey.get(), lookupKey.get());
        ASSERT_TRUE(insertedKey->equals(lookupKey));

        table->put(insertedKey, S("value"));

        EXPECT_EQ(
            textOf(table->get(asObject(lookupKey))),
            "value");
    }

    TEST(HashtableTest, PutNullKeyThrows) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_THROW(
            table->put(nullptr, S("value")),
            jxx::lang::NullPointerException);
    }

    TEST(HashtableTest, PutNullValueThrows) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_THROW(
            table->put(S("key"), nullptr),
            jxx::lang::NullPointerException);
    }

    TEST(HashtableTest, GetMissingKeyReturnsNull) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_EQ(
            table->get(asObject(S("missing"))),
            nullptr);
    }

    TEST(HashtableTest, GetNullKeyReturnsNullInCurrentImplementation) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_EQ(table->get(nullptr), nullptr);
    }

    /*
     * containsKey / containsValue / contains
     */

    TEST(HashtableTest, ContainsKeyFindsStoredKey) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        EXPECT_TRUE(
            table->containsKey(
                asObject(S("key"))));

        EXPECT_FALSE(
            table->containsKey(
                asObject(S("missing"))));
    }

    TEST(HashtableTest, ContainsKeyUsesLogicalEquality) {
        auto table = jxx::NEW<StringHashtable>();

        auto first = S("equal-key");
        auto second = S("equal-key");

        table->put(first, S("value"));

        EXPECT_TRUE(
            table->containsKey(
                asObject(second)));
    }

    TEST(HashtableTest, ContainsValueFindsStoredValue) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        EXPECT_TRUE(
            table->containsValue(
                asObject(S("value"))));

        EXPECT_FALSE(
            table->containsValue(
                asObject(S("missing"))));
    }

    TEST(HashtableTest, ContainsDelegatesToContainsValue) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        EXPECT_TRUE(
            table->contains(
                asObject(S("value"))));

        EXPECT_FALSE(
            table->contains(
                asObject(S("key"))));
    }

    TEST(HashtableTest, ContainsValueNullThrows) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_THROW(
            table->containsValue(nullptr),
            jxx::lang::NullPointerException);
    }

    /*
     * remove
     */

    TEST(HashtableTest, RemoveExistingMappingReturnsOldValue) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        auto removed =
            table->remove(
                asObject(S("key")));

        ASSERT_NE(removed, nullptr);
        EXPECT_EQ(removed->utf8(), "value");

        EXPECT_EQ(table->size(), 0);
        EXPECT_FALSE(
            table->containsKey(
                asObject(S("key"))));
    }

    TEST(HashtableTest, RemoveMissingKeyReturnsNull) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_EQ(
            table->remove(
                asObject(S("missing"))),
            nullptr);
    }

    TEST(HashtableTest, RemoveNullKeyReturnsNull) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_EQ(table->remove(nullptr), nullptr);
    }

    TEST(HashtableTest, ConditionalRemoveRequiresMatchingValue) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        EXPECT_FALSE(
            table->remove(
                asObject(S("key")),
                asObject(S("wrong"))));

        EXPECT_TRUE(
            table->containsKey(
                asObject(S("key"))));

        EXPECT_TRUE(
            table->remove(
                asObject(S("key")),
                asObject(S("value"))));

        EXPECT_FALSE(
            table->containsKey(
                asObject(S("key"))));
    }

    TEST(HashtableTest, ConditionalRemoveNullArgumentsReturnFalse) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_FALSE(
            table->remove(
                nullptr,
                asObject(S("value"))));

        EXPECT_FALSE(
            table->remove(
                asObject(S("key")),
                nullptr));
    }

    /*
     * clear
     */

    TEST(HashtableTest, ClearRemovesAllMappings) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));
        table->put(S("three"), S("3"));

        ASSERT_EQ(table->size(), 3);

        table->clear();

        EXPECT_EQ(table->size(), 0);
        EXPECT_TRUE(table->isEmpty());

        EXPECT_EQ(
            table->get(asObject(S("one"))),
            nullptr);
    }

    TEST(HashtableTest, ClearEmptyTableIsSafe) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_NO_THROW(table->clear());
        EXPECT_TRUE(table->isEmpty());
    }

    /*
     * Rehash behavior
     */

    TEST(HashtableTest, RehashPreservesMappings) {
        /*
         * A capacity of one with a 0.75 load factor forces frequent rehashing.
         */
        auto table =
            jxx::NEW<StringHashtable>(
                static_cast<jint>(1),
                0.75f);

        for (jint i = 0; i < 100; ++i) {
            table->put(
                S("key-" + std::to_string(i)),
                S("value-" + std::to_string(i)));
        }

        EXPECT_EQ(table->size(), 100);

        for (jint i = 0; i < 100; ++i) {
            auto value = table->get(
                asObject(
                    S("key-" + std::to_string(i))));

            ASSERT_NE(value, nullptr);
            EXPECT_EQ(
                value->utf8(),
                "value-" + std::to_string(i));
        }
    }

    /*
     * getOrDefault / putIfAbsent
     */

    TEST(HashtableTest, GetOrDefaultReturnsStoredValue) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        auto result = table->getOrDefault(
            asObject(S("key")),
            S("default"));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "value");
    }

    TEST(HashtableTest, GetOrDefaultReturnsDefaultForMissingKey) {
        auto table = jxx::NEW<StringHashtable>();

        auto result = table->getOrDefault(
            asObject(S("missing")),
            S("default"));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "default");
    }

    TEST(HashtableTest, PutIfAbsentAddsMissingMapping) {
        auto table = jxx::NEW<StringHashtable>();

        auto previous =
            table->putIfAbsent(
                S("key"),
                S("value"));

        EXPECT_EQ(previous, nullptr);

        EXPECT_EQ(
            textOf(table->get(asObject(S("key")))),
            "value");
    }

    TEST(HashtableTest, PutIfAbsentDoesNotReplaceExistingValue) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("first"));

        auto previous =
            table->putIfAbsent(
                S("key"),
                S("second"));

        ASSERT_NE(previous, nullptr);
        EXPECT_EQ(previous->utf8(), "first");

        EXPECT_EQ(
            textOf(table->get(asObject(S("key")))),
            "first");
    }

    TEST(HashtableTest, PutIfAbsentRejectsNullKeyAndValue) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_THROW(
            table->putIfAbsent(
                nullptr,
                S("value")),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            table->putIfAbsent(
                S("key"),
                nullptr),
            jxx::lang::NullPointerException);
    }

    /*
     * replace
     */

    TEST(HashtableTest, ReplaceExistingValueReturnsOldValue) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("old"));

        auto previous =
            table->replace(
                S("key"),
                S("new"));

        ASSERT_NE(previous, nullptr);
        EXPECT_EQ(previous->utf8(), "old");

        EXPECT_EQ(
            textOf(table->get(asObject(S("key")))),
            "new");
    }

    TEST(HashtableTest, ReplaceMissingKeyReturnsNull) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_EQ(
            table->replace(
                S("missing"),
                S("value")),
            nullptr);
    }

    TEST(HashtableTest, ConditionalReplaceRequiresMatchingOldValue) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("old"));

        EXPECT_FALSE(
            table->replace(
                S("key"),
                S("wrong"),
                S("new")));

        EXPECT_EQ(
            textOf(table->get(asObject(S("key")))),
            "old");

        EXPECT_TRUE(
            table->replace(
                S("key"),
                S("old"),
                S("new")));

        EXPECT_EQ(
            textOf(table->get(asObject(S("key")))),
            "new");
    }

    TEST(HashtableTest, ReplaceRejectsNullArguments) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_THROW(
            table->replace(
                nullptr,
                S("value")),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            table->replace(
                S("key"),
                nullptr),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            table->replace(
                nullptr,
                S("old"),
                S("new")),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            table->replace(
                S("key"),
                nullptr,
                S("new")),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            table->replace(
                S("key"),
                S("old"),
                nullptr),
            jxx::lang::NullPointerException);
    }

    /*
     * Enumerations
     */

    TEST(HashtableTest, KeysEnumerationReturnsAllKeys) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));
        table->put(S("three"), S("3"));

        const auto keys = collectKeys(table);

        EXPECT_EQ(keys.size(), 3U);
        EXPECT_EQ(keys.count("one"), 1U);
        EXPECT_EQ(keys.count("two"), 1U);
        EXPECT_EQ(keys.count("three"), 1U);
    }

    TEST(HashtableTest, ElementsEnumerationReturnsAllValues) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("shared"));
        table->put(S("two"), S("shared"));
        table->put(S("three"), S("different"));

        const auto values = collectValues(table);

        EXPECT_EQ(values.size(), 3U);
        EXPECT_EQ(values.count("shared"), 2U);
        EXPECT_EQ(values.count("different"), 1U);
    }

    TEST(HashtableTest, EmptyEnumerationsHaveNoElements) {
        auto table = jxx::NEW<StringHashtable>();

        auto keys = table->keys();
        auto values = table->elements();

        ASSERT_NE(keys, nullptr);
        ASSERT_NE(values, nullptr);

        EXPECT_FALSE(keys->hasMoreElements());
        EXPECT_FALSE(values->hasMoreElements());
    }

    TEST(HashtableTest, EnumerationThrowsWhenExhausted) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        auto keys = table->keys();

        ASSERT_TRUE(keys->hasMoreElements());
        ASSERT_NE(keys->nextElement(), nullptr);
        EXPECT_FALSE(keys->hasMoreElements());

        EXPECT_THROW(
            keys->nextElement(),
            jxx::util::NoSuchElementException);
    }

    TEST(HashtableTest, EnumerationIsSnapshotBased) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));

        auto keys = table->keys();

        /*
         * The implementation snapshots before returning the Enumeration.
         */
        table->put(S("three"), S("3"));

        jint count = 0;

        while (keys->hasMoreElements()) {
            keys->nextElement();
            ++count;
        }

        EXPECT_EQ(count, 2);
        EXPECT_EQ(table->size(), 3);
    }

    /*
     * Key-set view
     */

    TEST(HashtableTest, KeySetReflectsTableSize) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));

        auto keys = table->keySet();

        ASSERT_NE(keys, nullptr);
        EXPECT_EQ(keys->size(), 2);

        EXPECT_TRUE(
            keys->contains(
                asObject(S("one"))));

        EXPECT_FALSE(
            keys->contains(
                asObject(S("missing"))));
    }

    TEST(HashtableTest, KeySetRemoveRemovesMapping) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));

        auto keys = table->keySet();

        EXPECT_TRUE(
            keys->remove(
                asObject(S("one"))));

        EXPECT_EQ(table->size(), 1);

        EXPECT_FALSE(
            table->containsKey(
                asObject(S("one"))));
    }

    TEST(HashtableTest, KeySetClearClearsTable) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));

        auto keys = table->keySet();

        keys->clear();

        EXPECT_TRUE(table->isEmpty());
        EXPECT_EQ(keys->size(), 0);
    }

    TEST(HashtableTest, KeySetAddIsUnsupported) {
        auto table = jxx::NEW<StringHashtable>();
        auto keys = table->keySet();

        EXPECT_THROW(
            keys->add(S("unsupported")),
            jxx::lang::UnsupportedOperationException);
    }

    TEST(HashtableTest, KeySetIteratorIsSnapshotBased) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));

        auto iterator =
            table->keySet()->iterator();

        table->put(S("three"), S("3"));

        jint count = 0;

        while (iterator->hasNext()) {
            iterator->next();
            ++count;
        }

        EXPECT_EQ(count, 2);
    }

    TEST(HashtableTest, KeySetIteratorRemoveIsUnsupported) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));

        auto iterator =
            table->keySet()->iterator();

        ASSERT_TRUE(iterator->hasNext());
        iterator->next();

        EXPECT_THROW(
            iterator->remove(),
            jxx::lang::UnsupportedOperationException);
    }

    /*
     * Values view
     */

    TEST(HashtableTest, ValuesViewReflectsTableSize) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("shared"));
        table->put(S("two"), S("shared"));
        table->put(S("three"), S("different"));

        auto values = table->values();

        ASSERT_NE(values, nullptr);
        EXPECT_EQ(values->size(), 3);

        EXPECT_TRUE(
            values->contains(
                asObject(S("shared"))));

        EXPECT_FALSE(
            values->contains(
                asObject(S("missing"))));
    }

    TEST(HashtableTest, ValuesRemoveRemovesOneMatchingMapping) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("shared"));
        table->put(S("two"), S("shared"));
        table->put(S("three"), S("different"));

        auto values = table->values();

        EXPECT_TRUE(
            values->remove(
                asObject(S("shared"))));

        EXPECT_EQ(table->size(), 2);

        /*
         * Only one matching value should be removed.
         */
        EXPECT_TRUE(
            table->containsValue(
                asObject(S("shared"))));
    }

    TEST(HashtableTest, ValuesClearClearsTable) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));

        auto values = table->values();

        values->clear();

        EXPECT_TRUE(table->isEmpty());
    }

    TEST(HashtableTest, ValuesAddIsUnsupported) {
        auto table = jxx::NEW<StringHashtable>();
        auto values = table->values();

        EXPECT_THROW(
            values->add(S("unsupported")),
            jxx::lang::UnsupportedOperationException);
    }

    /*
     * Entry-set view
     */

    TEST(HashtableTest, EntrySetReflectsTableSize) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));

        auto entries = table->entrySet();

        ASSERT_NE(entries, nullptr);
        EXPECT_EQ(entries->size(), 2);
    }

    TEST(HashtableTest, EntryIteratorReturnsMapEntries) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        auto iterator =
            table->entrySet()->iterator();

        ASSERT_TRUE(iterator->hasNext());

        auto entry = iterator->next();

        ASSERT_NE(entry, nullptr);
        ASSERT_NE(entry->getKey(), nullptr);
        ASSERT_NE(entry->getValue(), nullptr);

        EXPECT_EQ(entry->getKey()->utf8(), "key");
        EXPECT_EQ(entry->getValue()->utf8(), "value");
    }

    TEST(HashtableTest, EntrySetValueUpdatesTable) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("old"));

        auto iterator =
            table->entrySet()->iterator();

        auto entry = iterator->next();

        auto previous =
            entry->setValue(S("new"));

        ASSERT_NE(previous, nullptr);
        EXPECT_EQ(previous->utf8(), "old");

        EXPECT_EQ(
            textOf(table->get(asObject(S("key")))),
            "new");
    }

    TEST(HashtableTest, EntrySetAddIsUnsupported) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        auto iterator =
            table->entrySet()->iterator();

        auto entry = iterator->next();

        EXPECT_THROW(
            table->entrySet()->add(entry),
            jxx::lang::UnsupportedOperationException);
    }

    /*
     * clone
     */

    TEST(HashtableTest, CloneReturnsSeparateHashtable) {
        auto original =
            jxx::NEW<StringHashtable>();

        original->put(S("one"), S("1"));
        original->put(S("two"), S("2"));

        auto clonedObject =
            original->clone();

        ASSERT_NE(clonedObject, nullptr);

        auto cloned =
            jxx::CAST<StringHashtable>(
                clonedObject);

        ASSERT_NE(cloned, nullptr);
        EXPECT_NE(cloned.get(), original.get());
        EXPECT_EQ(cloned->size(), original->size());

        EXPECT_EQ(
            textOf(cloned->get(asObject(S("one")))),
            "1");

        EXPECT_EQ(
            textOf(cloned->get(asObject(S("two")))),
            "2");
    }

    TEST(HashtableTest, CloneHasIndependentStorage) {
        auto original =
            jxx::NEW<StringHashtable>();

        original->put(S("one"), S("1"));
        original->put(S("two"), S("2"));

        auto cloned =
            jxx::CAST<StringHashtable>(
                original->clone());

        ASSERT_NE(cloned, nullptr);

        cloned->put(S("three"), S("3"));
        cloned->remove(asObject(S("one")));

        EXPECT_EQ(original->size(), 2);
        EXPECT_EQ(cloned->size(), 2);

        EXPECT_TRUE(
            original->containsKey(
                asObject(S("one"))));

        EXPECT_FALSE(
            original->containsKey(
                asObject(S("three"))));

        EXPECT_FALSE(
            cloned->containsKey(
                asObject(S("one"))));

        EXPECT_TRUE(
            cloned->containsKey(
                asObject(S("three"))));
    }

    TEST(HashtableTest, CloneSharesKeyAndValueReferences) {
        auto original =
            jxx::NEW<StringHashtable>();

        auto key = S("key");
        auto value = S("value");

        original->put(key, value);

        auto cloned =
            jxx::CAST<StringHashtable>(
                original->clone());

        ASSERT_NE(cloned, nullptr);

        auto clonedValue =
            cloned->get(asObject(key));

        EXPECT_EQ(clonedValue.get(), value.get());
    }

    /*
     * equals and hashCode parity checks
     */

    TEST(HashtableTest, HashtableEqualsItself) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("key"), S("value"));

        EXPECT_TRUE(
            table->equals(asObject(table)));
    }

    TEST(HashtableTest, EqualTablesShouldCompareEqual) {
        auto left = jxx::NEW<StringHashtable>();
        auto right = jxx::NEW<StringHashtable>();

        left->put(S("one"), S("1"));
        left->put(S("two"), S("2"));

        right->put(S("two"), S("2"));
        right->put(S("one"), S("1"));

        EXPECT_TRUE(
            left->equals(asObject(right)));

        EXPECT_TRUE(
            right->equals(asObject(left)));
    }

    TEST(HashtableTest, TablesWithDifferentValuesAreNotEqual) {
        auto left = jxx::NEW<StringHashtable>();
        auto right = jxx::NEW<StringHashtable>();

        left->put(S("key"), S("left"));
        right->put(S("key"), S("right"));

        EXPECT_FALSE(
            left->equals(asObject(right)));
    }

    TEST(HashtableTest, TablesWithDifferentKeysAreNotEqual) {
        auto left = jxx::NEW<StringHashtable>();
        auto right = jxx::NEW<StringHashtable>();

        left->put(S("left-key"), S("value"));
        right->put(S("right-key"), S("value"));

        EXPECT_FALSE(
            left->equals(asObject(right)));
    }

    TEST(HashtableTest, HashtableDoesNotEqualNull) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_FALSE(table->equals(nullptr));
    }

    TEST(HashtableTest, EqualTablesMustHaveSameHashCode) {
        auto left = jxx::NEW<StringHashtable>();
        auto right = jxx::NEW<StringHashtable>();

        left->put(S("one"), S("1"));
        left->put(S("two"), S("2"));

        right->put(S("two"), S("2"));
        right->put(S("one"), S("1"));

        ASSERT_TRUE(
            left->equals(asObject(right)));

        /*
         * This test will fail with the current implementation:
         *
         *     return (jint)this;
         *
         * Java Map hashCode must be based on entry hash codes, not the
         * object's memory address.
         */
        EXPECT_EQ(
            left->hashCode(),
            right->hashCode());
    }

    /*
     * Thread-safety smoke test
     */

    TEST(HashtableTest, ConcurrentPutOperationsDoNotLoseMappings) {
        auto table = jxx::NEW<StringHashtable>();

        constexpr int threadCount = 4;
        constexpr int entriesPerThread = 100;

        std::vector<std::thread> threads;

        for (int threadIndex = 0;
            threadIndex < threadCount;
            ++threadIndex) {

            threads.push_back(std::thread(
                [table, threadIndex, entriesPerThread]() {
                   for (int i = 0;
                        i < entriesPerThread;
                        ++i) {

                       const std::string key =
                           "thread-" +
                           std::to_string(threadIndex) +
                           "-key-" +
                           std::to_string(i);

                       const std::string value =
                           "value-" +
                           std::to_string(i);

                       table->put(
                           S(key),
                           S(value));
                   }
                }));
        }

        for (auto& thread : threads) {
            thread.join();
        }

        EXPECT_EQ(
            table->size(),
            threadCount* entriesPerThread);
    }

    /*
     * Serializable argument validation
     */

    TEST(HashtableTest, WriteObjectNullStreamThrows) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_THROW(
            table->writeObject(nullptr),
            jxx::lang::NullPointerException);
    }

    TEST(HashtableTest, ReadObjectNullStreamThrows) {
        auto table = jxx::NEW<StringHashtable>();

        EXPECT_THROW(
            table->readObject(nullptr),
            jxx::lang::NullPointerException);
    }

    TEST(HashtableTest, ReadObjectNoDataClearsTable) {
        auto table = jxx::NEW<StringHashtable>();

        table->put(S("one"), S("1"));
        table->put(S("two"), S("2"));

        ASSERT_EQ(table->size(), 2);

        table->readObjectNoData();

        EXPECT_EQ(table->size(), 0);
        EXPECT_TRUE(table->isEmpty());
    }

} // namespace