#include <limits>
#include <memory>
#include <string>
#include <type_traits>
/*
#include <gtest/gtest.h>

#include "io/jxx.io.Serializable.h"

#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"


    using jxx::lang::Object;
    using jxx::lang::String;
    using jxx::lang::jbool;
    using jxx::lang::jfloat;
    using jxx::lang::jint;

    using jxx::util::AbstractSet;
    using jxx::util::HashSet;
    using jxx::util::Iterator;

    using StringHashSet = HashSet<String>;

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
        const jxx::Ptr<StringHashSet>& value) {

        return jxx::CAST<Object>(value);
    }

    TEST(HashSetTest, ImplementsExpectedJava8Hierarchy) {
        static_assert(
            std::is_base_of_v<
            AbstractSet<String>,
            StringHashSet>,
            "HashSet<E> must extend AbstractSet<E>");

        static_assert(
            std::is_base_of_v<
            jxx::lang::Cloneable,
            StringHashSet>,
            "HashSet<E> must implement Cloneable");

        static_assert(
            std::is_base_of_v<
            jxx::io::Serializable,
            StringHashSet>,
            "HashSet<E> must implement Serializable");

        SUCCEED();
    }

  
    
    TEST(HashSetTest, DefaultConstructorCreatesEmptySet) {
        auto set = jxx::NEW<StringHashSet>();

        ASSERT_NE(set, nullptr);
        EXPECT_EQ(set->size(), 0);
        EXPECT_TRUE(set->isEmpty());
    }

    TEST(HashSetTest, InitialCapacityConstructorCreatesEmptySet) {
        auto set = jxx::NEW<StringHashSet>(
            static_cast<jint>(32));

        ASSERT_NE(set, nullptr);
        EXPECT_EQ(set->size(), 0);
        EXPECT_TRUE(set->isEmpty());
    }

    TEST(HashSetTest, ZeroInitialCapacityIsAccepted) {
        auto set = jxx::NEW<StringHashSet>(
            static_cast<jint>(0));

        ASSERT_NE(set, nullptr);
        EXPECT_TRUE(set->isEmpty());
    }

    TEST(HashSetTest, NegativeInitialCapacityThrows) {
        EXPECT_THROW(
            jxx::NEW<StringHashSet>(
                static_cast<jint>(-1)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(HashSetTest, CapacityAndLoadFactorConstructorCreatesEmptySet) {
        auto set = jxx::NEW<StringHashSet>(
            static_cast<jint>(64),
            static_cast<jfloat>(0.50f));

        ASSERT_NE(set, nullptr);
        EXPECT_TRUE(set->isEmpty());
        EXPECT_EQ(set->size(), 0);
    }

    TEST(HashSetTest, ZeroLoadFactorThrows) {
        EXPECT_THROW(
            jxx::NEW<StringHashSet>(
                static_cast<jint>(16),
                static_cast<jfloat>(0.0f)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(HashSetTest, NegativeLoadFactorThrows) {
        EXPECT_THROW(
            jxx::NEW<StringHashSet>(
                static_cast<jint>(16),
                static_cast<jfloat>(-0.75f)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(HashSetTest, NaNLoadFactorThrows) {
        const jfloat nanValue =
            std::numeric_limits<jfloat>::quiet_NaN();

        EXPECT_THROW(
            jxx::NEW<StringHashSet>(
                static_cast<jint>(16),
                nanValue),
            jxx::lang::IllegalArgumentException);
    }

    TEST(HashSetTest, PositiveInfinityLoadFactorIsAccepted) {
        const jfloat infinity =
            std::numeric_limits<jfloat>::infinity();

        auto set = jxx::NEW<StringHashSet>(
            static_cast<jint>(16),
            infinity);

        ASSERT_NE(set, nullptr);
        EXPECT_TRUE(set->isEmpty());
    }

     TEST(HashSetTest, AddNewElementReturnsTrue) {
        auto set = jxx::NEW<StringHashSet>();

        EXPECT_TRUE(set->add(S("alpha")));
        EXPECT_EQ(set->size(), 1);
        EXPECT_FALSE(set->isEmpty());
    }

    TEST(HashSetTest, AddDuplicateElementReturnsFalse) {
        auto set = jxx::NEW<StringHashSet>();

        EXPECT_TRUE(set->add(S("duplicate")));
        EXPECT_FALSE(set->add(S("duplicate")));

        EXPECT_EQ(set->size(), 1);
    }

    TEST(HashSetTest, AddUsesStringEqualsRatherThanPointerIdentity) {
        auto set = jxx::NEW<StringHashSet>();

        auto first = S("same-value");
        auto second = S("same-value");

        ASSERT_NE(first.get(), second.get());
        ASSERT_TRUE(first->equals(second));

        EXPECT_TRUE(set->add(first));
        EXPECT_FALSE(set->add(second));

        EXPECT_EQ(set->size(), 1);
    }

    TEST(HashSetTest, AddDifferentElementsIncreasesSize) {
        auto set = jxx::NEW<StringHashSet>();

        EXPECT_TRUE(set->add(S("one")));
        EXPECT_TRUE(set->add(S("two")));
        EXPECT_TRUE(set->add(S("three")));

        EXPECT_EQ(set->size(), 3);
    }

    TEST(HashSetTest, AddAllowsOneNullElement) {
        auto set = jxx::NEW<StringHashSet>();

        jxx::Ptr<String> nullElement = nullptr;

        EXPECT_TRUE(set->add(nullElement));
        EXPECT_FALSE(set->add(nullElement));

        EXPECT_EQ(set->size(), 1);
        EXPECT_TRUE(set->contains(nullptr));
    }


    TEST(HashSetTest, ContainsReturnsTrueForStoredElement) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("stored"));

        EXPECT_TRUE(
            set->contains(
                asObject(S("stored"))));
    }

    TEST(HashSetTest, ContainsReturnsFalseForMissingElement) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("stored"));

        EXPECT_FALSE(
            set->contains(
                asObject(S("missing"))));
    }

    TEST(HashSetTest, ContainsUsesEqualsAndHashCodeSemantics) {
        auto set = jxx::NEW<StringHashSet>();

        auto inserted = S("value");
        auto lookup = S("value");

        ASSERT_NE(inserted.get(), lookup.get());

        set->add(inserted);

        EXPECT_TRUE(
            set->contains(
                asObject(lookup)));
    }

    TEST(HashSetTest, ContainsNullReturnsFalseWhenNullNotPresent) {
        auto set = jxx::NEW<StringHashSet>();

        EXPECT_FALSE(set->contains(nullptr));
    }

    TEST(HashSetTest, ContainsNullReturnsTrueWhenNullPresent) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(nullptr);

        EXPECT_TRUE(set->contains(nullptr));
    }

 
    TEST(HashSetTest, RemoveExistingElementReturnsTrue) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("remove-me"));

        EXPECT_TRUE(
            set->remove(
                asObject(S("remove-me"))));

        EXPECT_EQ(set->size(), 0);
        EXPECT_TRUE(set->isEmpty());
    }

    TEST(HashSetTest, RemoveMissingElementReturnsFalse) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("stored"));

        EXPECT_FALSE(
            set->remove(
                asObject(S("missing"))));

        EXPECT_EQ(set->size(), 1);
    }

    TEST(HashSetTest, RemoveUsesEqualsInsteadOfPointerIdentity) {
        auto set = jxx::NEW<StringHashSet>();

        auto inserted = S("equal-value");
        auto removalKey = S("equal-value");

        ASSERT_NE(inserted.get(), removalKey.get());

        set->add(inserted);

        EXPECT_TRUE(
            set->remove(
                asObject(removalKey)));

        EXPECT_TRUE(set->isEmpty());
    }

    TEST(HashSetTest, RemoveNullWorksWhenNullPresent) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(nullptr);

        EXPECT_TRUE(set->remove(nullptr));
        EXPECT_FALSE(set->contains(nullptr));
        EXPECT_TRUE(set->isEmpty());
    }

    TEST(HashSetTest, RemoveNullReturnsFalseWhenAbsent) {
        auto set = jxx::NEW<StringHashSet>();

        EXPECT_FALSE(set->remove(nullptr));
    }


    TEST(HashSetTest, ClearRemovesAllElements) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));
        set->add(S("two"));
        set->add(S("three"));
        set->add(nullptr);

        ASSERT_EQ(set->size(), 4);

        set->clear();

        EXPECT_EQ(set->size(), 0);
        EXPECT_TRUE(set->isEmpty());

        EXPECT_FALSE(
            set->contains(
                asObject(S("one"))));

        EXPECT_FALSE(set->contains(nullptr));
    }

    TEST(HashSetTest, ClearOnEmptySetIsSafe) {
        auto set = jxx::NEW<StringHashSet>();

        EXPECT_NO_THROW(set->clear());
        EXPECT_TRUE(set->isEmpty());
    }



    TEST(HashSetTest, IteratorTraversesAllElements) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));
        set->add(S("two"));
        set->add(S("three"));

        auto iterator = set->iterator();

        ASSERT_NE(iterator, nullptr);

        jint count = 0;

        while (iterator->hasNext()) {
            auto value = iterator->next();

            ASSERT_NE(value, nullptr);
            ++count;
        }

        EXPECT_EQ(count, 3);
    }

    TEST(HashSetTest, IteratorDoesNotPromiseIterationOrder) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));
        set->add(S("two"));
        set->add(S("three"));

        auto iterator = set->iterator();

        jint count = 0;

        while (iterator->hasNext()) {
            ASSERT_NE(iterator->next(), nullptr);
            ++count;
        }

    
        EXPECT_EQ(count, set->size());
    }

    TEST(HashSetTest, EmptyIteratorHasNoNextElement) {
        auto set = jxx::NEW<StringHashSet>();
        auto iterator = set->iterator();

        ASSERT_NE(iterator, nullptr);
        EXPECT_FALSE(iterator->hasNext());
    }

    TEST(HashSetTest, IteratorNextThrowsWhenExhausted) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("only"));

        auto iterator = set->iterator();

        ASSERT_TRUE(iterator->hasNext());
        ASSERT_NE(iterator->next(), nullptr);
        EXPECT_FALSE(iterator->hasNext());

        EXPECT_THROW(
            iterator->next(),
            jxx::util::NoSuchElementException);
    }


    TEST(HashSetTest, IteratorRemoveDeletesLastReturnedElement) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));
        set->add(S("two"));

        auto iterator = set->iterator();

        ASSERT_TRUE(iterator->hasNext());

        auto returned = iterator->next();

        ASSERT_NE(returned, nullptr);

        iterator->remove();

        EXPECT_EQ(set->size(), 1);

        EXPECT_FALSE(
            set->contains(
                asObject(returned)));
    }

    TEST(HashSetTest, IteratorRemoveBeforeNextThrows) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));

        auto iterator = set->iterator();

        EXPECT_THROW(
            iterator->remove(),
            jxx::lang::IllegalStateException);
    }

    TEST(HashSetTest, IteratorRemoveTwiceForSameElementThrows) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));

        auto iterator = set->iterator();

        ASSERT_NE(iterator->next(), nullptr);

        iterator->remove();

        EXPECT_THROW(
            iterator->remove(),
            jxx::lang::IllegalStateException);
    }

    TEST(HashSetTest, IteratorCanContinueAfterItsOwnRemove) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));
        set->add(S("two"));
        set->add(S("three"));

        auto iterator = set->iterator();

        ASSERT_TRUE(iterator->hasNext());
        ASSERT_NE(iterator->next(), nullptr);

        iterator->remove();

        EXPECT_EQ(set->size(), 2);

        jint remainingSnapshotElements = 0;

        while (iterator->hasNext()) {
            ASSERT_NE(iterator->next(), nullptr);
            ++remainingSnapshotElements;
        }

        EXPECT_EQ(remainingSnapshotElements, 2);
    }
   

    TEST(HashSetTest, IteratorNextDetectsExternalAdd) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));
        set->add(S("two"));

        auto iterator = set->iterator();

        set->add(S("three"));

        EXPECT_THROW(
            iterator->next(),
            jxx::util::ConcurrentModificationException);
    }

    TEST(HashSetTest, IteratorNextDetectsExternalRemove) {
        auto set = jxx::NEW<StringHashSet>();

        auto one = S("one");
        auto two = S("two");

        set->add(one);
        set->add(two);

        auto iterator = set->iterator();

        set->remove(asObject(one));

        EXPECT_THROW(
            iterator->next(),
            jxx::util::ConcurrentModificationException);
    }

    TEST(HashSetTest, IteratorNextDetectsExternalClear) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));
        set->add(S("two"));

        auto iterator = set->iterator();

        set->clear();

        EXPECT_THROW(
            iterator->next(),
            jxx::util::ConcurrentModificationException);
    }

    TEST(HashSetTest, IteratorRemoveDetectsExternalModification) {
        auto set = jxx::NEW<StringHashSet>();

        set->add(S("one"));
        set->add(S("two"));

        auto iterator = set->iterator();

        ASSERT_NE(iterator->next(), nullptr);

        set->add(S("three"));

        EXPECT_THROW(
            iterator->remove(),
            jxx::util::ConcurrentModificationException);
    }
    */