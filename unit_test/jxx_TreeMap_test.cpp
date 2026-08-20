#include <memory>
#include <set>
#include <string>
#include <type_traits>
#include <vector>
#include <gtest/gtest.h>
/*
#include "lang/jxx.lang.buildin_array.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Set.h"
#include "util/jxx.util.TreeMap.h"

    using jxx::lang::Object;
    using jxx::lang::String;
    using jxx::lang::jbool;
    using jxx::lang::jint;

    using jxx::util::AbstractMap;
    using jxx::util::ComparatorSuper;
    using jxx::util::Iterator;
    using jxx::util::MapEntry;
    using jxx::util::Set;
    using jxx::util::TreeMap;

    using StringTreeMap = TreeMap<jxx::lang::String, jxx::lang::String>;

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
        const jxx::Ptr<StringTreeMap>& value) {

        return jxx::CAST<Object>(value);
    }

    static std::string textOf(
        const jxx::Ptr<String>& value) {

        return value == nullptr
            ? std::string()
            : value->utf8();
    }

    static std::vector<std::string> collectEntryKeys(
        const jxx::Ptr<StringTreeMap>& map) {

        std::vector<std::string> result;

        auto entrySet = map->entrySet();

        if (entrySet == nullptr) {
            return result;
        }

        auto iterator = entrySet->iterator();

        while (iterator->hasNext()) {
            auto entry = iterator->next();

            if (entry != nullptr &&
                entry->getKey() != nullptr) {

                result.push_back(
                    entry->getKey()->utf8());
            }
        }

        return result;
    }


    class ReverseStringComparator final
        : public virtual ComparatorSuper<String> {
    public:
        virtual ~ReverseStringComparator() = default;

        virtual jint compareSuper(
            jxx::Ptr<String> left,
            jxx::Ptr<String> right) override {

            if (left == nullptr ||
                right == nullptr) {

                throw jxx::lang::NullPointerException();
            }

            return right->compareTo(left);
        }
    };

    TEST(TreeMapTest, DefaultConstructorCreatesEmptyMap) {
        auto map = jxx::NEW<StringTreeMap>();

        ASSERT_NE(map, nullptr);
        EXPECT_EQ(map->size(), 0);
        EXPECT_TRUE(map->isEmpty());
        EXPECT_EQ(map->comparator(), nullptr);
    }

    TEST(TreeMapTest, ComparatorConstructorCreatesEmptyMap) {
        auto comparator =
            jxx::NEW<ReverseStringComparator>();

        auto map =
            jxx::NEW<StringTreeMap>(comparator);

        ASSERT_NE(map, nullptr);
        EXPECT_TRUE(map->isEmpty());
        EXPECT_EQ(
            map->comparator().get(),
            comparator.get());
    }

    TEST(TreeMapTest, TreeMapCopyConstructorCopiesMappings) {
        auto source = jxx::NEW<StringTreeMap>();

        source->put(S("one"), S("1"));
        source->put(S("two"), S("2"));

        auto copy =
            jxx::NEW<StringTreeMap>(source);

        ASSERT_NE(copy, nullptr);
        EXPECT_NE(copy.get(), source.get());
        EXPECT_EQ(copy->size(), 2);

        EXPECT_EQ(
            textOf(copy->get(asObject(S("one")))),
            "1");

        EXPECT_EQ(
            textOf(copy->get(asObject(S("two")))),
            "2");
    }

    TEST(TreeMapTest, NullTreeMapCopyConstructorThrows) {
        jxx::Ptr<StringTreeMap> source = nullptr;

        EXPECT_THROW(
            jxx::NEW<StringTreeMap>(source),
            jxx::lang::NullPointerException);
    }



    TEST(TreeMapTest, PutAddsMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        auto previous =
            map->put(S("key"), S("value"));

        EXPECT_EQ(previous, nullptr);
        EXPECT_EQ(map->size(), 1);
        EXPECT_FALSE(map->isEmpty());

        EXPECT_EQ(
            textOf(map->get(asObject(S("key")))),
            "value");
    }

    TEST(TreeMapTest, PutReplacesExistingValue) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("key"), S("old"));

        auto previous =
            map->put(S("key"), S("new"));

        ASSERT_NE(previous, nullptr);
        EXPECT_EQ(previous->utf8(), "old");
        EXPECT_EQ(map->size(), 1);

        EXPECT_EQ(
            textOf(map->get(asObject(S("key")))),
            "new");
    }

    TEST(TreeMapTest, PutUsesComparisonInsteadOfPointerIdentity) {
        auto map = jxx::NEW<StringTreeMap>();

        auto firstKey = S("same-key");
        auto secondKey = S("same-key");

        ASSERT_NE(firstKey.get(), secondKey.get());
        ASSERT_EQ(firstKey->compareTo(secondKey), 0);

        EXPECT_EQ(
            map->put(firstKey, S("first")),
            nullptr);

        auto previous =
            map->put(secondKey, S("second"));

        ASSERT_NE(previous, nullptr);
        EXPECT_EQ(previous->utf8(), "first");
        EXPECT_EQ(map->size(), 1);

        EXPECT_EQ(
            textOf(map->get(asObject(firstKey))),
            "second");
    }

    TEST(TreeMapTest, PutRejectsNullKey) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_THROW(
            map->put(nullptr, S("value")),
            jxx::lang::NullPointerException);
    }

    TEST(TreeMapTest, PutAllowsNullValueInCurrentImplementation) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_EQ(
            map->put(S("key"), nullptr),
            nullptr);

        EXPECT_TRUE(
            map->containsKey(asObject(S("key"))));

        EXPECT_EQ(
            map->get(asObject(S("key"))),
            nullptr);
    }

    TEST(TreeMapTest, GetMissingKeyReturnsNull) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_EQ(
            map->get(asObject(S("missing"))),
            nullptr);
    }

    TEST(TreeMapTest, GetNullKeyReturnsNullInCurrentImplementation) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_EQ(map->get(nullptr), nullptr);
    }


    TEST(TreeMapTest, ContainsKeyFindsStoredKey) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("alpha"), S("value"));

        EXPECT_TRUE(
            map->containsKey(
                asObject(S("alpha"))));

        EXPECT_FALSE(
            map->containsKey(
                asObject(S("missing"))));
    }

    TEST(TreeMapTest, ContainsKeyUsesNaturalComparison) {
        auto map = jxx::NEW<StringTreeMap>();

        auto insertedKey = S("logical-key");
        auto lookupKey = S("logical-key");

        map->put(insertedKey, S("value"));

        EXPECT_TRUE(
            map->containsKey(
                asObject(lookupKey)));
    }

    TEST(TreeMapTest, ContainsValueFindsStoredValue) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("key"), S("value"));

        EXPECT_TRUE(
            map->containsValue(
                asObject(S("value"))));

        EXPECT_FALSE(
            map->containsValue(
                asObject(S("missing"))));
    }

    TEST(TreeMapTest, ContainsValueUsesEquals) {
        auto map = jxx::NEW<StringTreeMap>();

        auto insertedValue = S("equal-value");
        auto lookupValue = S("equal-value");

        ASSERT_NE(
            insertedValue.get(),
            lookupValue.get());

        map->put(S("key"), insertedValue);

        EXPECT_TRUE(
            map->containsValue(
                asObject(lookupValue)));
    }

    TEST(TreeMapTest, ContainsValueSupportsNullValue) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("key"), nullptr);

        EXPECT_TRUE(map->containsValue(nullptr));
    }

    TEST(TreeMapTest, RemoveExistingKeyReturnsOldValue) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("key"), S("value"));

        auto removed =
            map->remove(asObject(S("key")));

        ASSERT_NE(removed, nullptr);
        EXPECT_EQ(removed->utf8(), "value");
        EXPECT_TRUE(map->isEmpty());
    }

    TEST(TreeMapTest, RemoveMissingKeyReturnsNull) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_EQ(
            map->remove(asObject(S("missing"))),
            nullptr);
    }

    TEST(TreeMapTest, RemoveNullKeyReturnsNullInCurrentImplementation) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_EQ(map->remove(nullptr), nullptr);
    }

    TEST(TreeMapTest, ClearRemovesAllMappings) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("one"), S("1"));
        map->put(S("two"), S("2"));
        map->put(S("three"), S("3"));

        ASSERT_EQ(map->size(), 3);

        map->clear();

        EXPECT_EQ(map->size(), 0);
        EXPECT_TRUE(map->isEmpty());
    }

    TEST(TreeMapTest, ClearEmptyMapIsSafe) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_NO_THROW(map->clear());
        EXPECT_TRUE(map->isEmpty());
    }


    TEST(TreeMapTest, EntryIterationUsesNaturalKeyOrder) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("delta"), S("4"));
        map->put(S("alpha"), S("1"));
        map->put(S("charlie"), S("3"));
        map->put(S("bravo"), S("2"));

        const auto keys =
            collectEntryKeys(map);

        ASSERT_EQ(keys.size(), 4U);

        EXPECT_EQ(keys[0], "alpha");
        EXPECT_EQ(keys[1], "bravo");
        EXPECT_EQ(keys[2], "charlie");
        EXPECT_EQ(keys[3], "delta");
    }

    TEST(TreeMapTest, FirstAndLastKeyUseNaturalOrdering) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("delta"), S("4"));
        map->put(S("alpha"), S("1"));
        map->put(S("charlie"), S("3"));

        ASSERT_NE(map->firstKey(), nullptr);
        ASSERT_NE(map->lastKey(), nullptr);

        EXPECT_EQ(
            map->firstKey()->utf8(),
            "alpha");

        EXPECT_EQ(
            map->lastKey()->utf8(),
            "delta");
    }

    TEST(TreeMapTest, FirstKeyOnEmptyMapThrows) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_THROW(
            map->firstKey(),
            jxx::util::NoSuchElementException);
    }

    TEST(TreeMapTest, LastKeyOnEmptyMapThrows) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_THROW(
            map->lastKey(),
            jxx::util::NoSuchElementException);
    }


    TEST(TreeMapTest, CustomComparatorChangesOrdering) {
        auto comparator =
            jxx::NEW<ReverseStringComparator>();

        auto map =
            jxx::NEW<StringTreeMap>(comparator);

        map->put(S("alpha"), S("1"));
        map->put(S("charlie"), S("3"));
        map->put(S("bravo"), S("2"));

        EXPECT_EQ(
            map->firstKey()->utf8(),
            "charlie");

        EXPECT_EQ(
            map->lastKey()->utf8(),
            "alpha");

        const auto keys =
            collectEntryKeys(map);

        ASSERT_EQ(keys.size(), 3U);

        EXPECT_EQ(keys[0], "charlie");
        EXPECT_EQ(keys[1], "bravo");
        EXPECT_EQ(keys[2], "alpha");
    }

    TEST(TreeMapTest, ComparatorMethodReturnsConfiguredComparator) {
        auto comparator =
            jxx::NEW<ReverseStringComparator>();

        auto map =
            jxx::NEW<StringTreeMap>(comparator);

        ASSERT_NE(map->comparator(), nullptr);

        EXPECT_EQ(
            map->comparator().get(),
            comparator.get());
    }



    TEST(TreeMapTest, FirstEntryReturnsLowestMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("bravo"), S("2"));
        map->put(S("alpha"), S("1"));
        map->put(S("charlie"), S("3"));

        auto entry = map->firstEntry();

        ASSERT_NE(entry, nullptr);
        ASSERT_NE(entry->getKey(), nullptr);
        ASSERT_NE(entry->getValue(), nullptr);

        EXPECT_EQ(entry->getKey()->utf8(), "alpha");
        EXPECT_EQ(entry->getValue()->utf8(), "1");
    }

    TEST(TreeMapTest, LastEntryReturnsHighestMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("bravo"), S("2"));
        map->put(S("alpha"), S("1"));
        map->put(S("charlie"), S("3"));

        auto entry = map->lastEntry();

        ASSERT_NE(entry, nullptr);
        ASSERT_NE(entry->getKey(), nullptr);
        ASSERT_NE(entry->getValue(), nullptr);

        EXPECT_EQ(entry->getKey()->utf8(), "charlie");
        EXPECT_EQ(entry->getValue()->utf8(), "3");
    }

    TEST(TreeMapTest, FirstAndLastEntryReturnNullWhenEmpty) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_EQ(map->firstEntry(), nullptr);
        EXPECT_EQ(map->lastEntry(), nullptr);
    }

    TEST(TreeMapTest, EntrySetValueUpdatesMap) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("key"), S("old"));

        auto entry = map->firstEntry();

        ASSERT_NE(entry, nullptr);

        auto previous =
            entry->setValue(S("new"));

        ASSERT_NE(previous, nullptr);
        EXPECT_EQ(previous->utf8(), "old");

        EXPECT_EQ(
            textOf(map->get(asObject(S("key")))),
            "new");
    }



    TEST(TreeMapTest, LowerKeyReturnsStrictlyLowerKey) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));
        map->put(S("e"), S("5"));

        EXPECT_EQ(map->lowerKey(S("a")), nullptr);

        ASSERT_NE(map->lowerKey(S("c")), nullptr);
        EXPECT_EQ(map->lowerKey(S("c"))->utf8(), "a");

        ASSERT_NE(map->lowerKey(S("d")), nullptr);
        EXPECT_EQ(map->lowerKey(S("d"))->utf8(), "c");

        ASSERT_NE(map->lowerKey(S("z")), nullptr);
        EXPECT_EQ(map->lowerKey(S("z"))->utf8(), "e");
    }

    TEST(TreeMapTest, FloorKeyReturnsEqualOrLowerKey) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));
        map->put(S("e"), S("5"));

        EXPECT_EQ(map->floorKey(S("0")), nullptr);

        ASSERT_NE(map->floorKey(S("c")), nullptr);
        EXPECT_EQ(map->floorKey(S("c"))->utf8(), "c");

        ASSERT_NE(map->floorKey(S("d")), nullptr);
        EXPECT_EQ(map->floorKey(S("d"))->utf8(), "c");
    }

    TEST(TreeMapTest, CeilingKeyReturnsEqualOrHigherKey) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));
        map->put(S("e"), S("5"));

        ASSERT_NE(map->ceilingKey(S("c")), nullptr);
        EXPECT_EQ(map->ceilingKey(S("c"))->utf8(), "c");

        ASSERT_NE(map->ceilingKey(S("d")), nullptr);
        EXPECT_EQ(map->ceilingKey(S("d"))->utf8(), "e");

        EXPECT_EQ(map->ceilingKey(S("z")), nullptr);
    }

    TEST(TreeMapTest, HigherKeyReturnsStrictlyHigherKey) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));
        map->put(S("e"), S("5"));

        ASSERT_NE(map->higherKey(S("a")), nullptr);
        EXPECT_EQ(map->higherKey(S("a"))->utf8(), "c");

        ASSERT_NE(map->higherKey(S("d")), nullptr);
        EXPECT_EQ(map->higherKey(S("d"))->utf8(), "e");

        EXPECT_EQ(map->higherKey(S("e")), nullptr);
    }

    TEST(TreeMapTest, NavigationOperationsRejectNullKey) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));

        EXPECT_THROW(
            map->lowerKey(nullptr),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            map->floorKey(nullptr),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            map->ceilingKey(nullptr),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            map->higherKey(nullptr),
            jxx::lang::NullPointerException);
    }

  
    TEST(TreeMapTest, LowerEntryReturnsExpectedMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));
        map->put(S("e"), S("5"));

        auto entry = map->lowerEntry(S("d"));

        ASSERT_NE(entry, nullptr);
        EXPECT_EQ(entry->getKey()->utf8(), "c");
        EXPECT_EQ(entry->getValue()->utf8(), "3");
    }

    TEST(TreeMapTest, FloorEntryReturnsExpectedMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));
        map->put(S("e"), S("5"));

        auto entry = map->floorEntry(S("c"));

        ASSERT_NE(entry, nullptr);
        EXPECT_EQ(entry->getKey()->utf8(), "c");
        EXPECT_EQ(entry->getValue()->utf8(), "3");
    }

    TEST(TreeMapTest, CeilingEntryReturnsExpectedMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));
        map->put(S("e"), S("5"));

        auto entry = map->ceilingEntry(S("d"));

        ASSERT_NE(entry, nullptr);
        EXPECT_EQ(entry->getKey()->utf8(), "e");
        EXPECT_EQ(entry->getValue()->utf8(), "5");
    }

    TEST(TreeMapTest, HigherEntryReturnsExpectedMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));
        map->put(S("e"), S("5"));

        auto entry = map->higherEntry(S("c"));

        ASSERT_NE(entry, nullptr);
        EXPECT_EQ(entry->getKey()->utf8(), "e");
        EXPECT_EQ(entry->getValue()->utf8(), "5");
    }

 

    TEST(TreeMapTest, PollFirstEntryRemovesLowestMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("b"), S("2"));
        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));

        auto entry = map->pollFirstEntry();

        ASSERT_NE(entry, nullptr);
        ASSERT_NE(entry->getKey(), nullptr);

        EXPECT_EQ(entry->getKey()->utf8(), "a");

        ASSERT_NE(entry->getValue(), nullptr);
        EXPECT_EQ(entry->getValue()->utf8(), "1");

        EXPECT_EQ(map->size(), 2);
        EXPECT_FALSE(
            map->containsKey(asObject(S("a"))));
    }

    TEST(TreeMapTest, PollLastEntryRemovesHighestMapping) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("b"), S("2"));
        map->put(S("a"), S("1"));
        map->put(S("c"), S("3"));

        auto entry = map->pollLastEntry();

        ASSERT_NE(entry, nullptr);
        ASSERT_NE(entry->getKey(), nullptr);

        EXPECT_EQ(entry->getKey()->utf8(), "c");

        ASSERT_NE(entry->getValue(), nullptr);
        EXPECT_EQ(entry->getValue()->utf8(), "3");

        EXPECT_EQ(map->size(), 2);
        EXPECT_FALSE(
            map->containsKey(asObject(S("c"))));
    }

    TEST(TreeMapTest, PollEntriesReturnNullWhenEmpty) {
        auto map = jxx::NEW<StringTreeMap>();

        EXPECT_EQ(map->pollFirstEntry(), nullptr);
        EXPECT_EQ(map->pollLastEntry(), nullptr);
    }



    TEST(TreeMapTest, EntrySetIteratorTraversesMappingsInOrder) {
        auto map = jxx::NEW<StringTreeMap>();

        map->put(S("c"), S("3"));
        map->put(S("a"), S("1"));
        map->put(S("b"), S("2"));

        auto iterator =
            map->entrySet()->iterator();

        ASSERT_TRUE(iterator->hasNext());

        auto first = iterator->next();

        ASSERT_NE(first, nullptr);
        ASSERT_NE(first->getKey(), nullptr);
        EXPECT_EQ(first->getKey()->utf8(), "a");

        ASSERT_TRUE(iterator->hasNext());

        auto second = iterator->next();

        ASSERT_NE(second, nullptr);
        EXPECT_EQ(second->getKey()->utf8(), "b");

        ASSERT_TRUE(iterator->hasNext());

        auto third = iterator->next();

        ASSERT_NE(third, nullptr);
        EXPECT_EQ(third->getKey()->utf8(), "c");

        EXPECT_FALSE(iterator->hasNext());
    }
    */