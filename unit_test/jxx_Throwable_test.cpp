#include <gtest/gtest.h>
#include "jxx.h"

class ThrowableTest : public jxx::lang::Exception {
    int value_{ 0 };
public:
    using Exception::Exception;
    
    virtual ~ThrowableTest() {
    }

    int intValue() const noexcept { return value_; }
    
    bool equals(const jxx::lang::Object& other) const noexcept override {
        auto* o = dynamic_cast<const ThrowableTest*>(&other);
        return o && value_ == o->value_;
    }
    std::size_t hashCode() const noexcept override {
        return std::hash<int>{}(value_);
    }
    std::string toString() const override {
        return std::to_string(value_);
    }

    std::shared_ptr<jxx::lang::Object> clone() const override {
        return std::shared_ptr<jxx::lang::Object>(nullptr);
    }
};

class TestThrowableTest : public testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    TestThrowableTest() {
        // You can do set-up work for each test here.
    }

    ~TestThrowableTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void throw_test() {
        JXX_THROW(ThrowableTest, "cleanup failed");
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
TEST(TestThrowableTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

void throw_test() {
    JXX_THROW(ThrowableTest, "cleanup failed");
}

TEST(TestThrowableTest, ThrowException) {
    //const std::string input_filepath = "this/package/testdata/myinputfile.dat";
    //const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
    int val = 1;

    EXPECT_THROW(throw_test(), ThrowableTest);
     //   ASSERT_THROW(statement, exception_type)
}

TEST(TestThrowableTest, IntValueTestScope) {
    //const std::string input_filepath = "this/package/testdata/myinputfile.dat";
    //const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
    int val = 5;
    
 
}
