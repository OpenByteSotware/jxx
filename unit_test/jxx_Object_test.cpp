#include <gtest/gtest.h>
#include "jxx.h"

using namespace jxx::lang;

class TestInteger : public Object, Cloneable {
    int value_{ 0 };
public:
    static bool _destructorCalled;

    TestInteger(int v) : value_(v) {
        TestInteger::_destructorCalled = false;
    }

    virtual ~TestInteger() {
        TestInteger::_destructorCalled = true;
    }

    int intValue() const noexcept { return value_; }

    bool equals(const jxx::lang::Object& other) const noexcept override {
        auto* o = dynamic_cast<const TestInteger*>(&other);
        return o && value_ == o->value_;
    }
    std::size_t hashCode() const noexcept override {
        return std::hash<int>{}(value_);
    }
    std::string toString() const override {
        return std::to_string(value_);
    }
protected:
    // Implement cloneImpl for deep copy, Ojbect uses this for C++ to mimic java like clone
    virtual std::shared_ptr<Object> cloneImpl() const override {
        return JXX_NEW<TestInteger>(this->value_);    
    }
};

bool TestInteger::_destructorCalled = false;

class TestIntegerTest : public testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    TestIntegerTest() {
        // You can do set-up work for each test here.
    }

    ~TestIntegerTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        TestInteger::_destructorCalled = false;
      }

    void TearDown() override {
     }

    // Class members declared here can be used by all tests in the test suite
    // for Foo.
};

// Demonstrate some basic assertions.
TEST(TestIntegerTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(TestIntegerTest, IntValueTest) {
    //const std::string input_filepath = "this/package/testdata/myinputfile.dat";
    //const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
    int val = 1;
    auto ixx = JXX_NEW<TestInteger>(val);
    EXPECT_EQ(val, ixx->intValue());
    ASSERT_EQ(TestInteger::_destructorCalled, false);
    ixx.reset(); // only one reference counter, should be 0, delete object
    ASSERT_EQ(TestInteger::_destructorCalled, true);
}

TEST(TestIntegerTest, IntValueTestScope) {
    //const std::string input_filepath = "this/package/testdata/myinputfile.dat";
    //const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
    int val = 5;
    
    {
        auto ixx = JXX_NEW<TestInteger>(val);
        EXPECT_EQ(val, ixx->intValue());
        ASSERT_EQ(TestInteger::_destructorCalled, false);
    }

    ASSERT_EQ(TestInteger::_destructorCalled, true);
}