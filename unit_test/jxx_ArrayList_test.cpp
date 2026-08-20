#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.RandomAccess.h"
#include "util/jxx.util.Spliterator.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.PredicateSuper.h"
#include "util/function/jxx.util.function.UnaryOperator.h"

namespace {

    using jxx::lang::Object;
    using jxx::lang::String;
    using jxx::lang::jbool;
    using jxx::lang::jint;

    using jxx::util::AbstractList;
    using jxx::util::ArrayList;
    using jxx::util::ComparatorSuper;
    using jxx::util::List;
    using jxx::util::RandomAccess;
    using jxx::util::Spliterator;

    using StringArrayList = ArrayList<String>;

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
        const jxx::Ptr<StringArrayList>& value) {

        return jxx::CAST<Object>(value);
    }

    static std::string textOf(
        const jxx::Ptr<String>& value) {

        return value == nullptr
            ? std::string()
            : value->utf8();
    }

    /*
     * Functional-interface helpers.
     *
     * Adjust the method names only if your PredicateSuper,
     * UnaryOperator, or Consumer interfaces use different signatures.
     */

    class StartsWithRemovePredicate final
        : public virtual
        jxx::util::function::PredicateSuper<String> {
    private:
        std::string prefix_;

    public:
        explicit StartsWithRemovePredicate(
            std::string prefix)
            : prefix_(std::move(prefix)) {}

        virtual ~StartsWithRemovePredicate() = default;

        virtual jbool test(
            jxx::Ptr<String> value) override {

            if (value == nullptr) {
                return static_cast<jbool>(false);
            }

            const std::string text = value->utf8();

            return static_cast<jbool>(
                text.rfind(prefix_, 0) == 0);
        }
    };

    class UpperCaseOperator final
        : public virtual
        jxx::util::function::UnaryOperator<String> {
    public:
        virtual ~UpperCaseOperator() = default;

        virtual jxx::Ptr<String> apply(
            jxx::Ptr<String> value) override {

            if (value == nullptr) {
                return nullptr;
            }

            return value->toUpperCase();
        }
    };

    class StringComparator final
        : public virtual ComparatorSuper<String> {
    public:
        virtual ~StringComparator() = default;

        virtual jint compareSuper(
            jxx::Ptr<String> left,
            jxx::Ptr<String> right) override {

            if (left == nullptr || right == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            return left->compareTo(right);
        }
    };

    class ReverseStringComparator final
        : public virtual ComparatorSuper<String> {
    public:
        virtual ~ReverseStringComparator() = default;

        virtual jint compareSuper(
            jxx::Ptr<String> left,
            jxx::Ptr<String> right) override {

            if (left == nullptr || right == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            return right->compareTo(left);
        }
    };

    class CollectingConsumer final
        : public virtual
        jxx::util::function::Consumer<String> {
    public:
        std::vector<std::string> values;

        virtual ~CollectingConsumer() = default;

        virtual void accept(
            jxx::Ptr<String> value) override {

            values.push_back(
                value == nullptr
                ? std::string("<null>")
                : value->utf8());
        }
    };

    TEST(ArrayListTest, DefaultConstructorCreatesEmptyList) {
        auto list = jxx::NEW<StringArrayList>();

        ASSERT_NE(list, nullptr);
        EXPECT_EQ(list->size(), 0);
        EXPECT_TRUE(list->isEmpty());
    }

    TEST(ArrayListTest, InitialCapacityConstructorCreatesEmptyList) {
        auto list =
            jxx::NEW<StringArrayList>(
                static_cast<jint>(32));

        ASSERT_NE(list, nullptr);
        EXPECT_EQ(list->size(), 0);
        EXPECT_TRUE(list->isEmpty());
    }

    TEST(ArrayListTest, ZeroInitialCapacityIsAccepted) {
        auto list =
            jxx::NEW<StringArrayList>(
                static_cast<jint>(0));

        ASSERT_NE(list, nullptr);
        EXPECT_TRUE(list->isEmpty());

        EXPECT_TRUE(list->add(S("value")));
        EXPECT_EQ(list->size(), 1);
    }

    TEST(ArrayListTest, NegativeInitialCapacityThrows) {
        EXPECT_THROW(
            jxx::NEW<StringArrayList>(
                static_cast<jint>(-1)),
            jxx::lang::IllegalArgumentException);
    }

    /*
     * Add and get
     */

    TEST(ArrayListTest, AddAppendsElement) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_TRUE(list->add(S("alpha")));

        ASSERT_EQ(list->size(), 1);
        ASSERT_NE(list->get(0), nullptr);
        EXPECT_EQ(list->get(0)->utf8(), "alpha");
    }

    TEST(ArrayListTest, AddPreservesInsertionOrder) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("three"));

        ASSERT_EQ(list->size(), 3);

        EXPECT_EQ(list->get(0)->utf8(), "one");
        EXPECT_EQ(list->get(1)->utf8(), "two");
        EXPECT_EQ(list->get(2)->utf8(), "three");
    }

    TEST(ArrayListTest, AddAllowsDuplicateElements) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_TRUE(list->add(S("duplicate")));
        EXPECT_TRUE(list->add(S("duplicate")));

        EXPECT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(0)->utf8(), "duplicate");
        EXPECT_EQ(list->get(1)->utf8(), "duplicate");
    }

    TEST(ArrayListTest, AddAllowsNullElements) {
        auto list = jxx::NEW<StringArrayList>();

        jxx::Ptr<String> nullValue = nullptr;

        EXPECT_TRUE(list->add(nullValue));
        EXPECT_EQ(list->size(), 1);
        EXPECT_EQ(list->get(0), nullptr);
        EXPECT_TRUE(list->contains(nullptr));
    }

    /*
     * Indexed insertion
     */

    TEST(ArrayListTest, AddAtBeginningShiftsElementsRight) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("two"));
        list->add(S("three"));

        list->add(0, S("one"));

        ASSERT_EQ(list->size(), 3);
        EXPECT_EQ(list->get(0)->utf8(), "one");
        EXPECT_EQ(list->get(1)->utf8(), "two");
        EXPECT_EQ(list->get(2)->utf8(), "three");
    }

    TEST(ArrayListTest, AddAtMiddleShiftsElementsRight) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("three"));

        list->add(1, S("two"));

        ASSERT_EQ(list->size(), 3);
        EXPECT_EQ(list->get(0)->utf8(), "one");
        EXPECT_EQ(list->get(1)->utf8(), "two");
        EXPECT_EQ(list->get(2)->utf8(), "three");
    }

    TEST(ArrayListTest, AddAtEndIsAllowed) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(1, S("two"));

        ASSERT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(1)->utf8(), "two");
    }

    TEST(ArrayListTest, AddRejectsNegativeIndex) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_THROW(
            list->add(-1, S("value")),
            jxx::lang::IndexOutOfBoundsException);
    }

    TEST(ArrayListTest, AddRejectsIndexAboveSize) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_THROW(
            list->add(1, S("value")),
            jxx::lang::IndexOutOfBoundsException);
    }

    /*
     * get and set
     */

    TEST(ArrayListTest, GetReturnsRequestedElement) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("zero"));
        list->add(S("one"));

        EXPECT_EQ(list->get(0)->utf8(), "zero");
        EXPECT_EQ(list->get(1)->utf8(), "one");
    }

    TEST(ArrayListTest, GetRejectsInvalidIndex) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("value"));

        EXPECT_THROW(
            list->get(-1),
            jxx::lang::IndexOutOfBoundsException);

        EXPECT_THROW(
            list->get(1),
            jxx::lang::IndexOutOfBoundsException);
    }

    TEST(ArrayListTest, SetReplacesElementAndReturnsOldValue) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("old"));

        auto oldValue =
            list->set(0, S("new"));

        ASSERT_NE(oldValue, nullptr);
        EXPECT_EQ(oldValue->utf8(), "old");

        ASSERT_NE(list->get(0), nullptr);
        EXPECT_EQ(list->get(0)->utf8(), "new");
        EXPECT_EQ(list->size(), 1);
    }

    TEST(ArrayListTest, SetCanReplaceElementWithNull) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("value"));

        auto oldValue =
            list->set(0, nullptr);

        ASSERT_NE(oldValue, nullptr);
        EXPECT_EQ(oldValue->utf8(), "value");
        EXPECT_EQ(list->get(0), nullptr);
    }

    TEST(ArrayListTest, SetRejectsInvalidIndex) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_THROW(
            list->set(0, S("value")),
            jxx::lang::IndexOutOfBoundsException);
    }

    /*
     * Searching
     */

    TEST(ArrayListTest, ContainsUsesLogicalEquality) {
        auto list = jxx::NEW<StringArrayList>();

        auto stored = S("value");
        auto lookup = S("value");

        ASSERT_NE(stored.get(), lookup.get());

        list->add(stored);

        EXPECT_TRUE(
            list->contains(asObject(lookup)));
    }

    TEST(ArrayListTest, ContainsReturnsFalseForMissingElement) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("stored"));

        EXPECT_FALSE(
            list->contains(
                asObject(S("missing"))));
    }

    TEST(ArrayListTest, IndexOfReturnsFirstOccurrence) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("one"));

        EXPECT_EQ(
            list->indexOf(asObject(S("one"))),
            0);

        EXPECT_EQ(
            list->indexOf(asObject(S("two"))),
            1);

        EXPECT_EQ(
            list->indexOf(asObject(S("missing"))),
            -1);
    }

    TEST(ArrayListTest, LastIndexOfReturnsFinalOccurrence) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("one"));

        EXPECT_EQ(
            list->lastIndexOf(asObject(S("one"))),
            2);

        EXPECT_EQ(
            list->lastIndexOf(asObject(S("two"))),
            1);

        EXPECT_EQ(
            list->lastIndexOf(asObject(S("missing"))),
            -1);
    }

    TEST(ArrayListTest, IndexMethodsHandleNull) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("first"));
        list->add(nullptr);
        list->add(S("middle"));
        list->add(nullptr);

        EXPECT_EQ(list->indexOf(nullptr), 1);
        EXPECT_EQ(list->lastIndexOf(nullptr), 3);
    }

    /*
     * Removal
     */

    TEST(ArrayListTest, RemoveByIndexReturnsRemovedElement) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("three"));

        auto removed = list->remove(1);

        ASSERT_NE(removed, nullptr);
        EXPECT_EQ(removed->utf8(), "two");

        ASSERT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(0)->utf8(), "one");
        EXPECT_EQ(list->get(1)->utf8(), "three");
    }

    TEST(ArrayListTest, RemoveFirstElementShiftsRemainingElements) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("three"));

        auto removed = list->remove(0);

        ASSERT_NE(removed, nullptr);
        EXPECT_EQ(removed->utf8(), "one");

        ASSERT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(0)->utf8(), "two");
        EXPECT_EQ(list->get(1)->utf8(), "three");
    }

    TEST(ArrayListTest, RemoveLastElementWorks) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));

        auto removed = list->remove(1);

        ASSERT_NE(removed, nullptr);
        EXPECT_EQ(removed->utf8(), "two");

        ASSERT_EQ(list->size(), 1);
        EXPECT_EQ(list->get(0)->utf8(), "one");
    }

    TEST(ArrayListTest, RemoveByIndexRejectsInvalidIndex) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_THROW(
            list->remove(0),
            jxx::lang::IndexOutOfBoundsException);

        list->add(S("value"));

        EXPECT_THROW(
            list->remove(-1),
            jxx::lang::IndexOutOfBoundsException);

        EXPECT_THROW(
            list->remove(1),
            jxx::lang::IndexOutOfBoundsException);
    }

    TEST(ArrayListTest, RemoveByObjectRemovesFirstOccurrence) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("one"));

        EXPECT_TRUE(
            list->remove(
                asObject(S("one"))));

        ASSERT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(0)->utf8(), "two");
        EXPECT_EQ(list->get(1)->utf8(), "one");
    }

    TEST(ArrayListTest, RemoveMissingObjectReturnsFalse) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("stored"));

        EXPECT_FALSE(
            list->remove(
                asObject(S("missing"))));

        EXPECT_EQ(list->size(), 1);
    }

    TEST(ArrayListTest, RemoveNullElementWorks) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(nullptr);
        list->add(S("two"));

        EXPECT_TRUE(list->remove(nullptr));

        ASSERT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(0)->utf8(), "one");
        EXPECT_EQ(list->get(1)->utf8(), "two");
    }

    /*
     * Clear
     */

    TEST(ArrayListTest, ClearRemovesAllElements) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(nullptr);

        ASSERT_EQ(list->size(), 3);

        list->clear();

        EXPECT_TRUE(list->isEmpty());
        EXPECT_EQ(list->size(), 0);
    }

    TEST(ArrayListTest, ClearEmptyListIsSafe) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_NO_THROW(list->clear());
        EXPECT_TRUE(list->isEmpty());
    }

    /*
     * Capacity operations
     */

    TEST(ArrayListTest, EnsureCapacityPreservesElements) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));

        list->ensureCapacity(100);

        ASSERT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(0)->utf8(), "one");
        EXPECT_EQ(list->get(1)->utf8(), "two");
    }

    TEST(ArrayListTest, TrimToSizePreservesElements) {
        auto list =
            jxx::NEW<StringArrayList>(
                static_cast<jint>(100));

        list->add(S("one"));
        list->add(S("two"));

        list->trimToSize();

        ASSERT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(0)->utf8(), "one");
        EXPECT_EQ(list->get(1)->utf8(), "two");

        // Verify the list can grow again after trimming.
        EXPECT_TRUE(list->add(S("three")));
        EXPECT_EQ(list->size(), 3);
    }

    /*
     * toArray
     */

    TEST(ArrayListTest, ToArrayReturnsIndependentObjectArray) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(nullptr);

        auto array = list->toArray();

        ASSERT_NE(array, nullptr);
        ASSERT_EQ(array->length, 3U);

        auto first =
            jxx::CAST<String>((*array)[0]);

        auto second =
            jxx::CAST<String>((*array)[1]);

        ASSERT_NE(first, nullptr);
        ASSERT_NE(second, nullptr);

        EXPECT_EQ(first->utf8(), "one");
        EXPECT_EQ(second->utf8(), "two");
        EXPECT_EQ((*array)[2], nullptr);

        (*array)[0] = asObject(S("changed"));

        EXPECT_EQ(list->get(0)->utf8(), "one");
    }

    /*
     * Clone
     */

    TEST(ArrayListTest, CloneReturnsSeparateArrayList) {
        auto original = jxx::NEW<StringArrayList>();

        original->add(S("one"));
        original->add(S("two"));
        original->add(nullptr);

        auto clonedObject = original->clone();

        ASSERT_NE(clonedObject, nullptr);

        auto cloned =
            jxx::CAST<StringArrayList>(
                clonedObject);

        ASSERT_NE(cloned, nullptr);
        EXPECT_NE(cloned.get(), original.get());
        EXPECT_EQ(cloned->size(), original->size());

        EXPECT_EQ(cloned->get(0)->utf8(), "one");
        EXPECT_EQ(cloned->get(1)->utf8(), "two");
        EXPECT_EQ(cloned->get(2), nullptr);
    }

    TEST(ArrayListTest, CloneHasIndependentArrayStorage) {
        auto original = jxx::NEW<StringArrayList>();

        original->add(S("one"));
        original->add(S("two"));

        auto cloned =
            jxx::CAST<StringArrayList>(
                original->clone());

        ASSERT_NE(cloned, nullptr);

        cloned->set(0, S("changed"));
        cloned->add(S("three"));
        cloned->remove(1);

        EXPECT_EQ(original->size(), 2);
        EXPECT_EQ(original->get(0)->utf8(), "one");
        EXPECT_EQ(original->get(1)->utf8(), "two");

        EXPECT_EQ(cloned->size(), 2);
        EXPECT_EQ(cloned->get(0)->utf8(), "changed");
        EXPECT_EQ(cloned->get(1)->utf8(), "three");
    }

    TEST(ArrayListTest, CloneSharesElementReferences) {
        auto original = jxx::NEW<StringArrayList>();
        auto element = S("shared");

        original->add(element);

        auto cloned =
            jxx::CAST<StringArrayList>(
                original->clone());

        ASSERT_NE(cloned, nullptr);
        EXPECT_EQ(
            cloned->get(0).get(),
            element.get());
    }

    /*
     * Functional operations
     */

    TEST(ArrayListTest, RemoveIfRemovesMatchingElements) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("keep-one"));
        list->add(S("remove-one"));
        list->add(S("keep-two"));
        list->add(S("remove-two"));

        auto predicate =
            jxx::NEW<StartsWithRemovePredicate>(
                "remove");

        EXPECT_TRUE(list->removeIf(predicate));

        ASSERT_EQ(list->size(), 2);
        EXPECT_EQ(list->get(0)->utf8(), "keep-one");
        EXPECT_EQ(list->get(1)->utf8(), "keep-two");
    }

    TEST(ArrayListTest, RemoveIfReturnsFalseWhenNothingMatches) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("keep-one"));
        list->add(S("keep-two"));

        auto predicate =
            jxx::NEW<StartsWithRemovePredicate>(
                "remove");

        EXPECT_FALSE(list->removeIf(predicate));
        EXPECT_EQ(list->size(), 2);
    }

    TEST(ArrayListTest, RemoveIfRejectsNullPredicate) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_THROW(
            list->removeIf(nullptr),
            jxx::lang::NullPointerException);
    }

    TEST(ArrayListTest, ReplaceAllTransformsEveryElement) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(nullptr);

        auto operation =
            jxx::NEW<UpperCaseOperator>();

        list->replaceAll(operation);

        ASSERT_EQ(list->size(), 3);
        EXPECT_EQ(list->get(0)->utf8(), "ONE");
        EXPECT_EQ(list->get(1)->utf8(), "TWO");
        EXPECT_EQ(list->get(2), nullptr);
    }

    TEST(ArrayListTest, ReplaceAllRejectsNullOperator) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_THROW(
            list->replaceAll(nullptr),
            jxx::lang::NullPointerException);
    }

    /*
     * Sort
     */

    TEST(ArrayListTest, SortOrdersElementsUsingComparator) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("charlie"));
        list->add(S("alpha"));
        list->add(S("bravo"));

        auto comparator =
            jxx::NEW<StringComparator>();

        list->sort(comparator);

        ASSERT_EQ(list->size(), 3);
        EXPECT_EQ(list->get(0)->utf8(), "alpha");
        EXPECT_EQ(list->get(1)->utf8(), "bravo");
        EXPECT_EQ(list->get(2)->utf8(), "charlie");
    }

    TEST(ArrayListTest, SortSupportsReverseComparator) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("charlie"));
        list->add(S("alpha"));
        list->add(S("bravo"));

        auto comparator =
            jxx::NEW<ReverseStringComparator>();

        list->sort(comparator);

        ASSERT_EQ(list->size(), 3);
        EXPECT_EQ(list->get(0)->utf8(), "charlie");
        EXPECT_EQ(list->get(1)->utf8(), "bravo");
        EXPECT_EQ(list->get(2)->utf8(), "alpha");
    }

    TEST(ArrayListTest, SortRejectsNullComparatorInCurrentImplementation) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("value"));

        EXPECT_THROW(
            list->sort(nullptr),
            jxx::lang::NullPointerException);
    }

    /*
     * Spliterator
     */

    TEST(ArrayListTest, SpliteratorReportsExpectedCharacteristics) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));

        auto spliterator = list->spliterator();

        ASSERT_NE(spliterator, nullptr);

        const jint characteristics =
            spliterator->characteristics();

        EXPECT_NE(
            characteristics &
            Spliterator<String>::ORDERED,
            0);

        EXPECT_NE(
            characteristics &
            Spliterator<String>::SIZED,
            0);

        EXPECT_NE(
            characteristics &
            Spliterator<String>::SUBSIZED,
            0);
    }

    TEST(ArrayListTest, SpliteratorEstimateSizeMatchesListSize) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("three"));

        auto spliterator = list->spliterator();

        ASSERT_NE(spliterator, nullptr);
        EXPECT_EQ(spliterator->estimateSize(), 3);
    }

    TEST(ArrayListTest, SpliteratorTryAdvanceVisitsElementsInOrder) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("three"));

        auto spliterator = list->spliterator();
        auto consumer = jxx::NEW<CollectingConsumer>();

        EXPECT_TRUE(
            spliterator->tryAdvance(consumer));

        EXPECT_TRUE(
            spliterator->tryAdvance(consumer));

        EXPECT_TRUE(
            spliterator->tryAdvance(consumer));

        EXPECT_FALSE(
            spliterator->tryAdvance(consumer));

        ASSERT_EQ(consumer->values.size(), 3U);
        EXPECT_EQ(consumer->values[0], "one");
        EXPECT_EQ(consumer->values[1], "two");
        EXPECT_EQ(consumer->values[2], "three");
    }

    TEST(ArrayListTest, SpliteratorForEachRemainingVisitsAllElements) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("three"));

        auto spliterator = list->spliterator();
        auto consumer = jxx::NEW<CollectingConsumer>();

        spliterator->forEachRemaining(consumer);

        ASSERT_EQ(consumer->values.size(), 3U);
        EXPECT_EQ(consumer->values[0], "one");
        EXPECT_EQ(consumer->values[1], "two");
        EXPECT_EQ(consumer->values[2], "three");

        EXPECT_EQ(spliterator->estimateSize(), 0);
    }

    TEST(ArrayListTest, SpliteratorTrySplitDividesTraversal) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));
        list->add(S("three"));
        list->add(S("four"));

        auto right = list->spliterator();
        auto left = right->trySplit();

        ASSERT_NE(left, nullptr);

        auto leftConsumer =
            jxx::NEW<CollectingConsumer>();

        auto rightConsumer =
            jxx::NEW<CollectingConsumer>();

        left->forEachRemaining(leftConsumer);
        right->forEachRemaining(rightConsumer);

        EXPECT_EQ(
            leftConsumer->values.size() +
            rightConsumer->values.size(),
            4U);

        ASSERT_EQ(leftConsumer->values.size(), 2U);
        ASSERT_EQ(rightConsumer->values.size(), 2U);

        EXPECT_EQ(leftConsumer->values[0], "one");
        EXPECT_EQ(leftConsumer->values[1], "two");
        EXPECT_EQ(rightConsumer->values[0], "three");
        EXPECT_EQ(rightConsumer->values[1], "four");
    }

    TEST(ArrayListTest, SpliteratorRejectsNullConsumer) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("value"));

        auto spliterator = list->spliterator();

        EXPECT_THROW(
            spliterator->tryAdvance(nullptr),
            jxx::lang::NullPointerException);

        EXPECT_THROW(
            spliterator->forEachRemaining(nullptr),
            jxx::lang::NullPointerException);
    }

    TEST(ArrayListTest, SpliteratorDetectsStructuralModification) {
        auto list = jxx::NEW<StringArrayList>();

        list->add(S("one"));
        list->add(S("two"));

        auto spliterator = list->spliterator();
        auto consumer = jxx::NEW<CollectingConsumer>();

        list->add(S("three"));

        EXPECT_THROW(
            spliterator->tryAdvance(consumer),
            jxx::util::
            ConcurrentModificationException);
    }

    /*
     * Serialization method smoke tests
     *
     * The current methods are empty. These tests only verify that the
     * interface methods are callable. For strict Java behavior, implement
     * actual ObjectInputStream/ObjectOutputStream handling.
     */

    TEST(ArrayListTest, ReadObjectNoDataIsCallable) {
        auto list = jxx::NEW<StringArrayList>();

        EXPECT_NO_THROW(
            list->readObjectNoData());
    }

} // namespace