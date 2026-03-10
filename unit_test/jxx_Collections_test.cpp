#include <gtest/gtest.h>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <numeric> // iota
#include <functional> // greater
#include "jxx.h"

using namespace jxx::util;

// ---------- Helpers ----------
template <typename T>
static bool IsPermutation(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) return false;
    std::vector<T> sa = a, sb = b;
    std::sort(sa.begin(), sa.end());
    std::sort(sb.begin(), sb.end());
    return sa == sb;
}

template <typename T>
static std::vector<T> V(std::initializer_list<T> il) { return std::vector<T>(il); }

// ============================================================
// reverse (vector)
// ============================================================
TEST(CollectionsVectorTest, Reverse_EmptyAndSingleton_NoChange) {
    std::vector<int> a;
    Collections::reverse(a);
    EXPECT_TRUE(a.empty());

    std::vector<int> b{ 42 };
    Collections::reverse(b);
    EXPECT_EQ(b, V<int>({ 42 }));
}

TEST(CollectionsVectorTest, Reverse_Multiple_ReversesOrder) {
    std::vector<int> a{ 1,2,3,4,5 };
    Collections::reverse(a);
    EXPECT_EQ(a, (std::vector<int>{5, 4, 3, 2, 1}));
}

// ============================================================
// sort (vector)
// ============================================================
TEST(CollectionsVectorTest, Sort_DefaultComparator_SortsAscending) {
    std::vector<int> a{ 7,3,5,1,9,1 };
    Collections::sort(a);
    EXPECT_EQ(a, (std::vector<int>{1, 1, 3, 5, 7, 9}));
}

TEST(CollectionsVectorTest, Sort_CustomComparator_SortsDescending) {
    std::vector<int> a{ 7,3,5,1,9,1 };
    Collections::sort(a, std::greater<int>{});
    EXPECT_EQ(a, (std::vector<int>{9, 7, 5, 3, 1, 1}));
}

// ============================================================
// shuffle (vector) — property-based (permutation), no RNG dependency
// ============================================================
TEST(CollectionsVectorTest, Shuffle_SizeZeroOrOne_NoChange) {
    std::vector<int> empty;
    Collections::shuffle(empty, nullptr);
    EXPECT_TRUE(empty.empty());

    std::vector<int> one{ 123 };
    auto before = one;
    Collections::shuffle(one, nullptr);
    EXPECT_EQ(one, before);
}

TEST(CollectionsVectorTest, Shuffle_PermutationProperty_PreservesMultiset) {
    std::vector<int> orig(100);
    std::iota(orig.begin(), orig.end(), 0);

    std::vector<int> shuffled = orig;
    Collections::shuffle(shuffled, nullptr);

    // It should be a permutation of original (order may or may not change)
    EXPECT_TRUE(IsPermutation(orig, shuffled));
}

// ============================================================
// binarySearch (vector)
// ============================================================
TEST(CollectionsVectorTest, BinarySearch_DefaultComparator_FoundAndNotFound) {
    std::vector<int> a{ 1,2,4,4,5,7,9 };
    // Found cases
    EXPECT_EQ(Collections::binarySearch(a, 1), 0);
    EXPECT_EQ(Collections::binarySearch(a, 4), 2); // first 4 via lower_bound
    EXPECT_EQ(Collections::binarySearch(a, 9), 6);
    // Not found: returns -(insertion_point + 1)
    // key=0 -> insert at 0 => -(0+1) = -1
    EXPECT_EQ(Collections::binarySearch(a, 0), -1);
    // key=3 -> insert at index 2 => -(2+1) = -3
    EXPECT_EQ(Collections::binarySearch(a, 3), -3);
    // key=10 -> insert at 7 => -(7+1) = -8
    EXPECT_EQ(Collections::binarySearch(a, 10), -8);
}

TEST(CollectionsVectorTest, BinarySearch_CustomComparator_Descending) {
    std::vector<int> a{ 9,7,5,3,1 };
    // Found
    EXPECT_EQ(Collections::binarySearch(a, 9, std::greater<int>{}), 0);
    EXPECT_EQ(Collections::binarySearch(a, 5, std::greater<int>{}), 2);
    EXPECT_EQ(Collections::binarySearch(a, 1, std::greater<int>{}), 4);
    // Not found: e.g., key=6 goes between 7 and 5 -> insertion point 2 -> -(2+1) = -3
    EXPECT_EQ(Collections::binarySearch(a, 6, std::greater<int>{}), -3);
}

// ============================================================
// min / max (vector)
// ============================================================
TEST(CollectionsVectorTest, MinMax_Typical) {
    std::vector<int> a{ 5,2,9,1,7 };
    EXPECT_EQ(Collections::min(a), 1);
    EXPECT_EQ(Collections::max(a), 9);
}

TEST(CollectionsVectorTest, MinMax_CustomComparator) {
    // Using absolute value comparator
    auto absLess = [](int x, int y) { return std::abs(x) < std::abs(y); };
    std::vector<int> a{ -10, 2, -3, 4, -5 };
    EXPECT_EQ(Collections::min(a, absLess), 2);
    EXPECT_EQ(Collections::max(a, absLess), -10);
}

TEST(CollectionsVectorTest, MinMax_Empty_Throws) {
    std::vector<int> empty;
    EXPECT_THROW((void)Collections::min(empty), NoSuchElementException);
    EXPECT_THROW((void)Collections::max(empty), NoSuchElementException);
}

// ============================================================
// frequency (vector)
// ============================================================
TEST(CollectionsVectorTest, Frequency_CountsOccurrences) {
    std::vector<int> a{ 1,2,3,2,2,4 };
    EXPECT_EQ(Collections::frequency(a, 2), 3);
    EXPECT_EQ(Collections::frequency(a, 5), 0);
}

// ============================================================
// disjoint (vector)
// ============================================================
TEST(CollectionsVectorTest, Disjoint_ReturnsTrueOrFalse) {
    std::vector<int> a{ 1,3,5 };
    std::vector<int> b{ 2,4,6 };
    std::vector<int> c{ 6,5,4 };
    EXPECT_TRUE(Collections::disjoint(a, b));
    EXPECT_FALSE(Collections::disjoint(a, c));
}

// ============================================================
// fill (vector)
// ============================================================
TEST(CollectionsVectorTest, Fill_SetsAllElements) {
    std::vector<int> a{ 1,2,3,4 };
    Collections::fill(a, 99);
    EXPECT_EQ(a, (std::vector<int>{99, 99, 99, 99}));
}

// ============================================================
// copy (vector)
// ============================================================
TEST(CollectionsVectorTest, Copy_SucceedsWhenDestAtLeastSrcSize) {
    std::vector<int> src{ 10,20,30 };
    std::vector<int> dest{ 0,0,0,777,888 };
    Collections::copy(dest, src);
    EXPECT_EQ(dest, (std::vector<int>{10, 20, 30, 777, 888})); // trailing untouched
}

TEST(CollectionsVectorTest, Copy_ThrowsWhenDestTooSmall) {
    std::vector<int> src{ 10,20,30 };
    std::vector<int> dest{ 0,0 };
    EXPECT_THROW(Collections::copy(dest, src), std::out_of_range);
}

// ============================================================
// swap (vector)
// ============================================================
TEST(CollectionsVectorTest, Swap_ValidIndices_SwapsElements) {
    std::vector<int> a{ 1,2,3,4 };
    Collections::swap(a, 1, 3);
    EXPECT_EQ(a, (std::vector<int>{1, 4, 3, 2}));
}

TEST(CollectionsVectorTest, Swap_InvalidIndices_Throws) {
    std::vector<int> a{ 1,2,3 };
    EXPECT_THROW(Collections::swap(a, -1, 0), std::out_of_range);
    EXPECT_THROW(Collections::swap(a, 0, 3), std::out_of_range);
}

// ============================================================
// rotate (vector)
// ============================================================
TEST(CollectionsVectorTest, Rotate_ZeroNoop_PositiveNegativeAndWrap) {
    std::vector<int> base{ 1,2,3,4,5 };

    // k = 0 -> noop
    {
        auto a = base;
        Collections::rotate(a, 0);
        EXPECT_EQ(a, base);
    }
    // k = 1 -> right-rotate by 1
    {
        auto a = base;
        Collections::rotate(a, 1);
        EXPECT_EQ(a, (std::vector<int>{5, 1, 2, 3, 4}));
    }
    // k = n-1
    {
        auto a = base;
        Collections::rotate(a, 4);
        EXPECT_EQ(a, (std::vector<int>{2, 3, 4, 5, 1}));
    }
    // k = n -> same as 0
    {
        auto a = base;
        Collections::rotate(a, 5);
        EXPECT_EQ(a, base);
    }
    // k = -1 -> left-rotate by 1 (equivalent to right by n-1)
    {
        auto a = base;
        Collections::rotate(a, -1);
        EXPECT_EQ(a, (std::vector<int>{2, 3, 4, 5, 1}));
    }
    // k = -n -> same as 0
    {
        auto a = base;
        Collections::rotate(a, -5);
        EXPECT_EQ(a, base);
    }
    // k = large positive
    {
        auto a = base;
        Collections::rotate(a, 12); // 12 % 5 = 2
        EXPECT_EQ(a, (std::vector<int>{4, 5, 1, 2, 3}));
    }
    // k = large negative
    {
        auto a = base;
        Collections::rotate(a, -12); // -12 % 5 == -2 -> +5 -> 3
        EXPECT_EQ(a, (std::vector<int>{3, 4, 5, 1, 2}));
    }
}

TEST(CollectionsVectorTest, Rotate_Empty_NoCrash) {
    std::vector<int> empty;
    Collections::rotate(empty, 7);
    EXPECT_TRUE(empty.empty());
}

// ============================================================
// nCopies (vector)
// ============================================================
TEST(CollectionsVectorTest, NCopies_CreatesVectorOfCopies) {
    auto v0 = Collections::nCopies<int>(0, 123);
    EXPECT_TRUE(v0.empty());

    auto v3 = Collections::nCopies<std::string>(3, std::string("x"));
    EXPECT_EQ(v3, (std::vector<std::string>{"x", "x", "x"}));
}

TEST(CollectionsVectorTest, NCopies_Negative_Throws) {
    EXPECT_THROW((void)Collections::nCopies<int>(-1, 0), std::out_of_range);
}

// ============================================================
// replaceAll (vector)
// ============================================================
TEST(CollectionsVectorTest, ReplaceAll_ReplacesAndReturnsModifiedFlag) {
    std::vector<int> a{ 1,2,2,3 };
    bool changed = Collections::replaceAll(a, 2, 9);
    EXPECT_TRUE(changed);
    EXPECT_EQ(a, (std::vector<int>{1, 9, 9, 3}));

    changed = Collections::replaceAll(a, 2, 9); // no 2s remain
    EXPECT_FALSE(changed);
    EXPECT_EQ(a, (std::vector<int>{1, 9, 9, 3}));
}

// ============================================================
// emptyList / singletonList
// ============================================================
/*
TEST(CollectionsVectorTest, EmptyListAndSingletonList) {
    auto e = Collections::emptyList<int>();
    EXPECT_TRUE(e.empty());

    auto s = Collections::singletonList<std::string>("hello");
    ASSERT_EQ(s.size(), 1u);
    EXPECT_EQ(s[0], "hello");
}
*/
