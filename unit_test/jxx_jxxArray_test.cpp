#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <numeric>
#include <limits>
#include <string>
#include <array>
#include <cstdint>
#include <type_traits>
#include <utility>
#include "jxx.lang.buildin_array.h"
#include "jxx.lang.String.h"

using jxx::lang::JxxArray;

// ---------------------------
// Helpers
// ---------------------------
struct Tracked {
    static inline int ctor = 0;
    static inline int copy_ctor = 0;
    static inline int move_ctor = 0;
    static inline int copy_assign = 0;
    static inline int move_assign = 0;
    static inline int dtor = 0;

    int v{ 0 };

    Tracked() { ++ctor; }
    explicit Tracked(int x) : v(x) { ++ctor; }

    Tracked(const Tracked& o) : v(o.v) { ++copy_ctor; }
    Tracked(Tracked&& o) noexcept : v(o.v) { o.v = -1; ++move_ctor; }

    Tracked& operator=(const Tracked& o) {
        v = o.v;
        ++copy_assign;
        return *this;
    }
    Tracked& operator=(Tracked&& o) noexcept {
        v = o.v;
        o.v = -1;
        ++move_assign;
        return *this;
    }

    ~Tracked() { ++dtor; }

    friend bool operator==(const Tracked& a, const Tracked& b) { return a.v == b.v; }
    friend bool operator!=(const Tracked& a, const Tracked& b) { return !(a == b); }

    static void ResetCounts() {
        ctor = copy_ctor = move_ctor = copy_assign = move_assign = dtor = 0;
    }
};

// ======================================================================
// Rank-1 tests: JxxArray<T,1>
// ======================================================================

TEST(JxxArrayRank1, DefaultConstructedIsEmpty) {
    JxxArray<int, 1> a;
    EXPECT_EQ(a.length, 0u);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.capacity(), 0u);
    EXPECT_EQ(a.data(), nullptr);
    EXPECT_EQ(a.begin(), a.end());
}

TEST(JxxArrayRank1, SizedConstructionSetsLengthAndCapacity) {
    JxxArray<int, 1> a(5);
    EXPECT_EQ(a.length, 5u);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.size(), 5u);
    EXPECT_EQ(a.capacity(), 5u);
    EXPECT_NE(a.data(), nullptr);

    // make_unique<T[]>(n) value-initializes elements -> ints should be zero
    for (std::uint32_t i = 0; i < a.length; ++i) {
        EXPECT_EQ(a[i], 0);
    }
}

TEST(JxxArrayRank1, InitializerListConstructionCopiesValues) {
    JxxArray<int, 1> a{ 1, 2, 3, 4 };
    ASSERT_EQ(a.length, 4u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 3);
    EXPECT_EQ(a[3], 4);
}

TEST(JxxArrayRank1, AtThrowsOutOfRange) {
    JxxArray<int, 1> a{ 10, 20, 30 };
    EXPECT_NO_THROW(a.at(0));
    EXPECT_NO_THROW(a.at(2));
    EXPECT_THROW(a.at(3), std::out_of_range);
}

TEST(JxxArrayRank1, IterationWorksAndMatchesStdAlgorithms) {
    JxxArray<int, 1> a{ 1, 2, 3, 4, 5 };
    int sum = 0;
    for (int x : a) sum += x;
    EXPECT_EQ(sum, 15);

    // using iterators directly
    int sum2 = std::accumulate(a.begin(), a.end(), 0);
    EXPECT_EQ(sum2, 15);
}

TEST(JxxArrayRank1, FillSetsAllElements) {
    JxxArray<int, 1> a(6);
    a.fill(42);
    for (std::uint32_t i = 0; i < a.length; ++i) {
        EXPECT_EQ(a[i], 42);
    }
}

TEST(JxxArrayRank1, ToVectorProducesEquivalentVector) {
    JxxArray<int, 1> a{ 7, 8, 9 };
    std::vector<int> v = a.to_vector();
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 7);
    EXPECT_EQ(v[1], 8);
    EXPECT_EQ(v[2], 9);
}

TEST(JxxArrayRank1, CopyConstructionAndAssignmentDeepCopy) {
    JxxArray<int, 1> a{ 1, 2, 3 };
    JxxArray<int, 1> b(a);  // copy ctor
    EXPECT_EQ(a, b);

    b[0] = 99;
    EXPECT_NE(a, b);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(b[0], 99);

    JxxArray<int, 1> c;
    c = a; // copy assign
    EXPECT_EQ(a, c);
    c[1] = 77;
    EXPECT_NE(a, c);
}

TEST(JxxArrayRank1, MoveConstructionAndAssignmentTransfersOwnership) {
    JxxArray<int, 1> a{ 1, 2, 3 };
    auto* old_ptr = a.data();

    JxxArray<int, 1> b(std::move(a));
    EXPECT_EQ(b.length, 3u);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b.data(), old_ptr);

    EXPECT_EQ(a.length, 0u);
    EXPECT_EQ(a.capacity(), 0u);
    EXPECT_EQ(a.data(), nullptr);

    JxxArray<int, 1> c{ 9, 9 };
    c = std::move(b);
    EXPECT_EQ(c.length, 3u);
    EXPECT_EQ(c[2], 3);
    EXPECT_EQ(b.length, 0u);
    EXPECT_EQ(b.capacity(), 0u);
    EXPECT_EQ(b.data(), nullptr);
}

TEST(JxxArrayRank1, ReserveDoesNotChangeLengthButIncreasesCapacity) {
    JxxArray<int, 1> a{ 1, 2, 3 };
    const auto old_len = a.length;
    const auto old_cap = a.capacity();
    a.reserve(old_cap + 10);

    EXPECT_EQ(a.length, old_len);
    EXPECT_GE(a.capacity(), old_cap + 10);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 3);
}

TEST(JxxArrayRank1, PushBackGrowsAndPreservesExistingElements) {
    JxxArray<int, 1> a;
    EXPECT_EQ(a.length, 0u);

    for (int i = 0; i < 10; ++i) a.push_back(i * 10);

    EXPECT_EQ(a.length, 10u);
    for (std::uint32_t i = 0; i < a.length; ++i) {
        EXPECT_EQ(a[i], static_cast<int>(i) * 10);
    }
    EXPECT_GE(a.capacity(), a.length);
}

TEST(JxxArrayRank1, EmplaceBackWorksForTrivialTypes) {
    static_assert(std::is_trivial_v<int>, "Test assumes int is trivial");
    JxxArray<int, 1> a;
    int& r = a.emplace_back(123);
    EXPECT_EQ(r, 123);
    EXPECT_EQ(a.length, 1u);
    EXPECT_EQ(a[0], 123);
}

TEST(JxxArrayRank1, ShrinkToFitReducesCapacityToLength) {
    JxxArray<int, 1> a;
    a.reserve(64);
    EXPECT_GE(a.capacity(), 64u);
    EXPECT_EQ(a.length, 0u);

    for (int i = 0; i < 5; ++i) a.push_back(i + 1);
    EXPECT_EQ(a.length, 5u);

    a.shrink_to_fit();
    EXPECT_EQ(a.capacity(), a.length);
    EXPECT_EQ(a.capacity(), 5u);

    for (std::uint32_t i = 0; i < a.length; ++i) {
        EXPECT_EQ(a[i], static_cast<int>(i) + 1);
    }
}

TEST(JxxArrayRank1, ResizeWithinCapacityFillsTail) {
    JxxArray<int, 1> a;
    a.reserve(10);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    EXPECT_EQ(a.length, 3u);

    a.resize(6, 99);
    EXPECT_EQ(a.length, 6u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 3);
    EXPECT_EQ(a[3], 99);
    EXPECT_EQ(a[4], 99);
    EXPECT_EQ(a[5], 99);

    a.resize(2);
    EXPECT_EQ(a.length, 2u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
}

TEST(JxxArrayRank1, SwapExchangesBuffersAndMetadata) {
    JxxArray<int, 1> a{ 1, 2, 3 };
    JxxArray<int, 1> b{ 9, 8 };

    auto* a_ptr = a.data();
    auto* b_ptr = b.data();

    a.swap(b);

    EXPECT_EQ(a.length, 2u);
    EXPECT_EQ(b.length, 3u);
    EXPECT_EQ(a.data(), b_ptr);
    EXPECT_EQ(b.data(), a_ptr);
    EXPECT_EQ(a[0], 9);
    EXPECT_EQ(b[0], 1);
}

TEST(JxxArrayRank1, TrackedTypeCopyAndMoveBehavior) {
    Tracked::ResetCounts();

    {
        JxxArray<Tracked, 1> a;
        a.push_back(Tracked(1));         // move assignment or move into element
        a.push_back(Tracked(2));
        a.push_back(Tracked(3));

        EXPECT_EQ(a.length, 3u);

        JxxArray<Tracked, 1> b(a);       // copy ctor (deep copy)
        EXPECT_EQ(b.length, 3u);
        EXPECT_EQ(b[0].v, 1);

        JxxArray<Tracked, 1> c(std::move(b)); // move ctor
        EXPECT_EQ(c.length, 3u);
    }

    // We can’t assert exact counts tightly because growth policy and assignments affect it,
    // but we can ensure we did at least some tracked operations and dtors ran.
    EXPECT_GT(Tracked::ctor + Tracked::copy_ctor + Tracked::move_ctor, 0);
    EXPECT_GT(Tracked::dtor, 0);
}

// ======================================================================
// Rank-N tests: JxxArray<T,Rank>=2
// ======================================================================

TEST(JxxArrayRankN, DefaultConstructedRank2IsEmpty) {
    JxxArray<int, 2> a;
    EXPECT_EQ(a.length, 0u);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.data(), nullptr);
    EXPECT_EQ(a.begin(), a.end());
}

TEST(JxxArrayRankN, SizedOuterDimensionConstructsDefaultSubarrays) {
    JxxArray<int, 2> a(3);
    EXPECT_EQ(a.length, 3u);
    EXPECT_FALSE(a.empty());

    // Subarrays are default constructed rank-1 arrays -> length=0
    EXPECT_EQ(a[0].length, 0u);
    EXPECT_EQ(a[1].length, 0u);
    EXPECT_EQ(a[2].length, 0u);
}

TEST(JxxArrayRankN, RectangularConstructionBuildsAllDimensions) {
    std::array<std::uint32_t, 2> dims{ 3, 4 };
    JxxArray<int, 2> a(dims);

    EXPECT_EQ(a.length, 3u);
    for (std::uint32_t i = 0; i < a.length; ++i) {
        EXPECT_EQ(a[i].length, 4u);
        // value-initialized ints for rank-1 ctor -> should be 0
        for (std::uint32_t j = 0; j < a[i].length; ++j) {
            EXPECT_EQ(a[i][j], 0);
        }
    }
}

TEST(JxxArrayRankN, NestedInitializerListConstruction) {
    JxxArray<int, 2> a{ { {1, 2}, {3, 4}, {5, 6} } };
    EXPECT_EQ(a.length, 3u);
    EXPECT_EQ(a[0].length, 2u);
    EXPECT_EQ(a[2][1], 6);
}

TEST(JxxArrayRankN, AtThrowsOutOfRangeOnOuterDimension) {
    JxxArray<int, 2> a(2);
    EXPECT_NO_THROW(a.at(0));
    EXPECT_NO_THROW(a.at(1));
    EXPECT_THROW(a.at(2), std::out_of_range);
}

TEST(JxxArrayRankN, FillRecursivelyFillsAllElements) {
    std::array<std::uint32_t, 3> dims{ 2, 3, 4 };
    JxxArray<int, 3> a(dims);

    a.fill(7);
    EXPECT_EQ(a.length, 2u);
    for (std::uint32_t i = 0; i < a.length; ++i) {
        EXPECT_EQ(a[i].length, 3u);
        for (std::uint32_t j = 0; j < a[i].length; ++j) {
            EXPECT_EQ(a[i][j].length, 4u);
            for (std::uint32_t k = 0; k < a[i][j].length; ++k) {
                EXPECT_EQ(a[i][j][k], 7);
            }
        }
    }
}

TEST(JxxArrayRankN, PushBackSubArrayAndInitListOverload) {
    // Start with 2D array with outer dimension 0
    JxxArray<int, 2> a;

    // push_back via nested init-list overload (Rank-1 tail init-list)
    a.push_back({ 1, 2, 3 });
    a.push_back({ 4, 5 });

    EXPECT_EQ(a.length, 2u);
    EXPECT_EQ(a[0].length, 3u);
    EXPECT_EQ(a[1].length, 2u);
    EXPECT_EQ(a[0][2], 3);
    EXPECT_EQ(a[1][1], 5);

    // push_back an existing subarray
    JxxArray<int, 1> sub{ 9, 9, 9, 9 };
    a.push_back(sub);

    EXPECT_EQ(a.length, 3u);
    EXPECT_EQ(a[2].length, 4u);
    EXPECT_EQ(a[2][3], 9);
}

TEST(JxxArrayRankN, ResizeOuterDimensionDefaultConstructsNewSubarrays) {
    std::array<std::uint32_t, 2> dims{ 2, 3 };
    JxxArray<int, 2> a(dims);
    EXPECT_EQ(a.length, 2u);
    EXPECT_EQ(a[0].length, 3u);

    a.resize(5);
    EXPECT_EQ(a.length, 5u);

    // First 2 subarrays remain
    EXPECT_EQ(a[0].length, 3u);
    EXPECT_EQ(a[1].length, 3u);

    // New subarrays are default-constructed (rank-1) -> length 0
    EXPECT_EQ(a[2].length, 0u);
    EXPECT_EQ(a[3].length, 0u);
    EXPECT_EQ(a[4].length, 0u);
}

TEST(JxxArrayRankN, ResizeWithTailDimsConstructsNewSubarraysWithGivenDims) {
    std::array<std::uint32_t, 2> dims{ 2, 3 };
    JxxArray<int, 2> a(dims);

    // grow to 4 outer elements, new ones should be length 3
    std::array<std::uint32_t, 1> tail{ 3 };
    a.resize(4, tail);

    EXPECT_EQ(a.length, 4u);
    EXPECT_EQ(a[0].length, 3u);
    EXPECT_EQ(a[1].length, 3u);
    EXPECT_EQ(a[2].length, 3u);
    EXPECT_EQ(a[3].length, 3u);
}

TEST(JxxArrayRankN, DeepEqualityAndInequality) {
    JxxArray<int, 2> a{ { {1, 2}, {3, 4} } };
    JxxArray<int, 2> b{ { {1, 2}, {3, 4} } };
    EXPECT_TRUE(a == b);

    b[1][1] = 99;
    EXPECT_TRUE(a != b);
}

TEST(JxxArrayRankN, SwapExchangesOuterVectorsAndLength) {
    JxxArray<int, 2> a{ { {1, 2, 3} } };
    JxxArray<int, 2> b{ { {9}, {8} } };

    a.swap(b);

    EXPECT_EQ(a.length, 2u);
    EXPECT_EQ(b.length, 1u);
    EXPECT_EQ(a[0].length, 1u);
    EXPECT_EQ(a[1].length, 1u);
    EXPECT_EQ(a[0][0], 9);
    EXPECT_EQ(a[1][0], 8);
    EXPECT_EQ(b[0][2], 3);
}
