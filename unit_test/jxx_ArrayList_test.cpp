#include <gtest/gtest.h>
#include "jxx.h"

class ArrayListIntTest : public jxx::util::ArrayList<int> {
    int value_{ 0 };
    bool runThread_{ true };
public:
    
    virtual ~ArrayListIntTest() {
    } 
};

class TestArrayListTest : public testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    TestArrayListTest() {
        // You can do set-up work for each test here.
    }

    ~TestArrayListTest() override {
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
TEST(TestArrayListTest, BasicAssertions) {
    auto t = JXX_NEW(ArrayListIntTest);
    int val = 5;
    ASSERT_TRUE(t->isEmpty());
    t->add(5);
    ASSERT_FALSE(t->isEmpty());
    EXPECT_EQ(t->at(0), 5);
    t->add(10);
    EXPECT_EQ(t->at(1), 10);
    t->add(15);
    EXPECT_EQ(t->at(2), 15);
    EXPECT_EQ(t->size(), 3);

}
