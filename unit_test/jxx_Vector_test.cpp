#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <numeric>
#include <limits>
#include <string>
#include "jxx.h"

using jxx::util::Vector;

class VectorIntTest : public jxx::util::Vector<int> {
    int value_{ 0 };
    bool runThread_{ true };
public:
    
    virtual ~VectorIntTest() {
    } 
};

class VectorStringTest : public jxx::util::Vector<std::string> {
public:

    virtual ~VectorStringTest() {
    }
};

class TestVectorTest : public testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    TestVectorTest() {
        // You can do set-up work for each test here.
    }

    ~TestVectorTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    } 

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
       }

    void TearDown() override {
     }

    // Class members declared here can be used by all tests in the test suite
    // for Foo.
};

// Demonstrate some basic assertions.
TEST(TestVectorTest, BasicAssertions) {
    auto t = NEW(VectorIntTest);
    int val = 5;
    ASSERT_TRUE(t->isEmpty());
    t->add(5);
    ASSERT_FALSE(t->isEmpty());
    EXPECT_EQ(t->firstElement(), 5);
    t->add(10);
    EXPECT_EQ(t->firstElement(), 5);
    t->add(15);
    EXPECT_EQ(t->lastElement(), 15);
    EXPECT_EQ(t->size(), 3);

}

TEST(TestVectorTest, StringData) {
    auto t = NEW(VectorStringTest);
    ASSERT_TRUE(t->isEmpty());
    t->add(std::string("5"));
    ASSERT_FALSE(t->isEmpty());
    ASSERT_TRUE(t->firstElement().compare(std::string("5")) == 0);
    t->add(std::string("10"));
    ASSERT_TRUE(t->firstElement().compare("5") == 0);
    t->add(std::string("15"));
    ASSERT_TRUE(t->firstElement().compare("5") == 0);
    ASSERT_TRUE(t->lastElement().compare("15") == 0);
    EXPECT_EQ(t->size(), 3);

}

    // ---------- Constructors, size, capacity ----------

    TEST(Vector_Int, DefaultCtor_ReservesInitialCapacityAndIsEmpty) {
        Vector<int> v; // default initialCapacity=10
        EXPECT_EQ(v.size(), 0u);
        EXPECT_TRUE(v.isEmpty());
        // Reserve is at least the initial capacity; do not assert exact value for portability
        EXPECT_GE(v.capacity(), 10u);
    }

    TEST(Vector_Int, InitListCtor) {
        Vector<int> v({ 1, 2, 3 });
        EXPECT_EQ(v.size(), 3u);
        EXPECT_FALSE(v.isEmpty());
        EXPECT_GE(v.capacity(), 3u);
        EXPECT_EQ(v.firstElement(), 1);
        EXPECT_EQ(v.lastElement(), 3);
    }

    // ---------- ensureCapacity, trimToSize ----------

    TEST(Vector_Int, EnsureCapacityDoesNotChangeSize) {
        Vector<int> v(1);
        EXPECT_EQ(v.size(), 0u);
        v.ensureCapacity(50);
        EXPECT_GE(v.capacity(), 50u);
        EXPECT_EQ(v.size(), 0u);
    }

    TEST(Vector_Int, TrimToSizeIsNonBinding) {
        Vector<int> v(2);
        v.add(42);
        v.add(13);
        auto before = v.capacity();
        v.trimToSize();
        // shrink_to_fit is non-binding; capacity may stay the same or decrease.
        EXPECT_LE(v.capacity(), before);
        EXPECT_EQ(v.size(), 2u);
        EXPECT_EQ(v.get(0), 42);
        EXPECT_EQ(v.get(1), 13);
    }

    // ---------- setSize grow/shrink ----------

    TEST(Vector_Int, SetSizeGrowAndShrink) {
        Vector<int> v(0); // start with no reserved capacity
        v.setSize(3);     // grow -> default-inserted ints == 0
        EXPECT_EQ(v.size(), 3u);
        EXPECT_EQ(v.get(0), 0);
        EXPECT_EQ(v.get(1), 0);
        EXPECT_EQ(v.get(2), 0);

        v.setSize(1); // shrink
        EXPECT_EQ(v.size(), 1u);
        EXPECT_EQ(v.get(0), 0);
    }

    // ---------- get/set and bounds checks ----------

    TEST(Vector_Int, GetSetAndOldValueReturn) {
        Vector<int> v;
        v.add(10);
        v.add(20);
        EXPECT_EQ(v.get(0), 10);
        EXPECT_EQ(v.get(1), 20);

        int old0 = v.set(0, 99);
        EXPECT_EQ(old0, 10);
        EXPECT_EQ(v.get(0), 99);

        int old1 = v.set(1, 123);
        EXPECT_EQ(old1, 20);
        EXPECT_EQ(v.get(1), 123);
    }

    TEST(Vector_Int, GetOutOfRangeThrows) {
        Vector<int> v;
        v.add(1);
        EXPECT_THROW(v.get(1), std::out_of_range);
        EXPECT_THROW(v.get(100), std::out_of_range);
    }

    TEST(Vector_Int, SetOutOfRangeThrows) {
        Vector<int> v;
        v.add(1);
        EXPECT_THROW((void)v.set(1, 5), std::out_of_range);
    }

    // ---------- firstElement / lastElement ----------

    TEST(Vector_Int, FirstLastElementThrowWhenEmpty) {
        Vector<int> v;
        EXPECT_THROW(v.firstElement(), std::out_of_range);
        EXPECT_THROW(v.lastElement(), std::out_of_range);
    }

    TEST(Vector_Int, FirstLastElementReturnValues) {
        Vector<int> v;
        v.add(7);
        v.add(8);
        EXPECT_EQ(v.firstElement(), 7);
        EXPECT_EQ(v.lastElement(), 8);
    }

    // ---------- add push_back and insert ----------

    TEST(Vector_Int, AddPushBackKeepsOrder) {
        Vector<int> v(0);
        v.add(1);
        v.add(2);
        v.add(3);
        EXPECT_EQ(v.size(), 3u);
        EXPECT_EQ(v.get(0), 1);
        EXPECT_EQ(v.get(1), 2);
        EXPECT_EQ(v.get(2), 3);
    }

    TEST(Vector_Int, AddInsertAtIndex) {
        Vector<int> v;
        v.add(1);
        v.add(3);
        v.add(1, 2); // insert 2 at index 1 -> [1,2,3]
        EXPECT_EQ(v.size(), 3u);
        EXPECT_EQ(v.get(0), 1);
        EXPECT_EQ(v.get(1), 2);
        EXPECT_EQ(v.get(2), 3);
    }

    TEST(Vector_Int, AddInsertOutOfRangeThrows) {
        Vector<int> v;
        v.add(1);
        EXPECT_THROW(v.add(2, 99), std::out_of_range); // index>size not allowed
        EXPECT_NO_THROW(v.add(1, 42));                 // index==size is allowed (append)
    }

    // ---------- removeAt / remove(value) / clear ----------

    TEST(Vector_Int, RemoveAtReturnsRemovedElementAndShifts) {
        Vector<int> v({ 10, 20, 30 });
        int removed = v.removeAt(1);
        EXPECT_EQ(removed, 20);
        EXPECT_EQ(v.size(), 2u);
        EXPECT_EQ(v.get(0), 10);
        EXPECT_EQ(v.get(1), 30);
    }

    TEST(Vector_Int, RemoveAtOutOfRangeThrows) {
        Vector<int> v({ 1 });
        EXPECT_THROW(v.removeAt(1), std::out_of_range);
    }

    TEST(Vector_Int, RemoveValueRemovesFirstOccurrence) {
        Vector<int> v({ 1, 2, 3, 2, 4 });
        EXPECT_TRUE(v.remove(2));
        // Now v should be [1,3,2,4]
        EXPECT_EQ(v.size(), 4u);
        EXPECT_EQ(v.get(0), 1);
        EXPECT_EQ(v.get(1), 3);
        EXPECT_EQ(v.get(2), 2);
        EXPECT_EQ(v.get(3), 4);

        EXPECT_TRUE(v.remove(2));   // remove second 2
        EXPECT_FALSE(v.remove(42)); // not found
        EXPECT_EQ(v.size(), 3u);
    }

    TEST(Vector_Int, ClearEmptiesTheVector) {
        Vector<int> v({ 5, 6, 7 });
        v.clear();
        EXPECT_EQ(v.size(), 0u);
        EXPECT_TRUE(v.isEmpty());
    }

    // ---------- indexOf / lastIndexOf ----------

    TEST(Vector_Int, IndexOfAndLastIndexOfBehaviors) {
        Vector<int> v({ 1, 2, 3, 2, 4 });

        EXPECT_EQ(v.indexOf(1), 0u);
        EXPECT_EQ(v.indexOf(2), 1u);
        EXPECT_EQ(v.indexOf(2, 2), 3u);        // from after first 2
        EXPECT_EQ(v.indexOf(999), jxx::NPOS);

        // fromIndex equal to size -> not found
        EXPECT_EQ(v.indexOf(1, v.size()), jxx::NPOS);
        // fromIndex > size -> returns NPOS early
        EXPECT_EQ(v.indexOf(1, v.size() + 1), jxx::NPOS);

        EXPECT_EQ(v.lastIndexOf(2), 3u);
        EXPECT_EQ(v.lastIndexOf(999), jxx::NPOS);

        Vector<int> empty;
        EXPECT_EQ(empty.lastIndexOf(1), jxx::NPOS);
    }

    // ---------- snapshot / toStdVector ----------

    TEST(Vector_Int, SnapshotIsIndependentCopy) {
        Vector<int> v({ 10, 20, 30 });
        auto snap = v.snapshot();
        // mutate original
        v.set(1, 99);
        v.add(123);

        // snapshot remains unchanged
        ASSERT_EQ(snap.size(), 3u);
        EXPECT_EQ(snap[0], 10);
        EXPECT_EQ(snap[1], 20);
        EXPECT_EQ(snap[2], 30);

        // toStdVector returns a copy too
        auto asStd = v.toStdVector();
        ASSERT_EQ(asStd.size(), 4u);
        EXPECT_EQ(asStd[0], 10);
        EXPECT_EQ(asStd[1], 99);
        EXPECT_EQ(asStd[2], 30);
        EXPECT_EQ(asStd[3], 123);
    }

    // ---------- Growth strategy: doubling (capacityIncrement == 0) ----------

    TEST(Vector_Int, Growth_DoublingStrategyLowerBounds) {
        Vector<int> v(/*initialCapacity*/ 0, /*capacityIncrement*/ 0);
        // First push: capacity should be >= 1
        v.add(1);
        EXPECT_GE(v.capacity(), 1u);

        // Second push: capacity should be >= 2
        v.add(2);
        EXPECT_GE(v.capacity(), 2u);

        // Third push: capacity should be >= 4 (doubling from >=2)
        v.add(3);
        EXPECT_GE(v.capacity(), 4u);
    }

    // ---------- Growth strategy: fixed increment (capacityIncrement > 0) ----------

    TEST(Vector_Int, Growth_FixedIncrementLowerBounds) {
        constexpr std::size_t inc = 5;
        Vector<int> v(/*initialCapacity*/ 0, /*capacityIncrement*/ inc);

        // After first push from 0, special-case goes to minCapacity(=1)
        v.add(1);
        EXPECT_GE(v.capacity(), 1u);

        // Next push requires capacity >= 2; growth jumps by +5 from oldCap (1) -> 6
        v.add(2);
        EXPECT_GE(v.capacity(), 1u + inc); // >= 6

        // Push until we exceed capacity 6
        while (v.size() <= 6) v.add(0); // when size==6 add requires minCap==7 -> next cap >= 11
        EXPECT_GE(v.capacity(), 1u + inc * 2); // >= 11
    }
    
    
    // ---------- String type sanity ----------

    TEST(Vector_String, WorksWithStrings) {
        Vector<std::string> v;
        v.add("a");
        v.add("b");
        v.add(1, std::string("x"));
        EXPECT_EQ(v.size(), 3u);
        EXPECT_EQ(v.get(0), "a");
        EXPECT_EQ(v.get(1), "x");
        EXPECT_EQ(v.get(2), "b");

        auto old = v.set(2, "B");
        EXPECT_EQ(old, "b");
        EXPECT_EQ(v.lastElement(), "B");

        EXPECT_TRUE(v.remove("x"));
        EXPECT_EQ(v.indexOf("x"), jxx::NPOS);
        EXPECT_EQ(v.indexOf("a"), 0u);
    }

    TEST(Vector_String, WorksWithStrings2) {
        Vector<jxx::lang::String> v { "1", "2", "3" };
        EXPECT_EQ(v.size(), 3u);
        EXPECT_EQ(v.get(0), "1");
        EXPECT_EQ(v.get(1), "2");
        EXPECT_EQ(v.get(2), "3");

        EXPECT_TRUE(v.remove("2"));
        EXPECT_EQ(v.indexOf("2"), jxx::NPOS);
        EXPECT_EQ(v.indexOf("a"), jxx::NPOS);
        EXPECT_EQ(v.indexOf("3"), 1);
    }