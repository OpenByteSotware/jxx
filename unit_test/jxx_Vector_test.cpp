#include <gtest/gtest.h>
#include "jxx.h"

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