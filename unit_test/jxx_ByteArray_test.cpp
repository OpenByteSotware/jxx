#include <gtest/gtest.h>
#include <cstddef>       // std::byte, std::to_integer
#include "jxx.h" // Your ByteNDArray implementation

using namespace std;

class ByteNDArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Called before each test
    }

    void TearDown() override {
        // Called after each test
    }
};

TEST(ByteArrayTest, RectangularArrayStoresAndRetrievesValues) {
    ByteArray arr({ 3, 2, 4 }); // 3x2x4 rectangular

    arr[0][1].leaf()[2] = byte{ 42 };
    EXPECT_EQ(arr[0][1].leaf()[2], 42);

    arr[2][0].leaf()[3] = byte{ 99 };
    EXPECT_EQ(arr[2][0].leaf()[3], 99);
}

TEST(ByteArrayTest, JaggedArrayStoresAndRetrievesValues) {
    ByteArray arr(3); // new byte[3][]

    arr[0].allocate(2); // arr[0] = new byte[2]
    arr[1].allocate(5); // arr[1] = new byte[5]

    arr[0].leaf()[1] = ::byte{ 77 };
    arr[1].leaf()[4] = ::byte{ 88 };

    EXPECT_EQ(arr[0].leaf()[1], 77);
    EXPECT_EQ(arr[1].leaf()[4], 88);
}

TEST(ByteArrayTest, MixedJaggedArrayWorks) {
    ByteArray arr(2); // new byte[2][][]
    arr[0].allocate(2, false); // arr[0] = new byte[2][]
    arr[0][0].allocate(3);     // arr[0][0] = new byte[3]
    arr[0][1].allocate(1);     // arr[0][1] = new byte[1]

    arr[0][0].leaf()[2] = ::byte{ 55 };
    arr[0][1].leaf()[0] = ::byte{ 66 };

    EXPECT_EQ(arr[0][0].leaf()[2], 55);
    EXPECT_EQ(arr[0][1].leaf()[0], 66);
}

TEST(ByteArrayTest, ThrowsWhenAccessingUnallocatedLeaf) {
    ByteArray arr(2); // new byte[2][]
    EXPECT_THROW(arr[0].leaf()[0], std::runtime_error);
}

TEST(ByteArrayTest, ThrowsWhenTooManyIndices) {
    ByteArray arr({ 2, 2 }); // 2x2
    EXPECT_THROW(arr[0][0][0], std::runtime_error); // No leaf() call
}
