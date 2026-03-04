#include <gtest/gtest.h>
#include "jxx.h"

class TheadTest : public jxx::lang::Thread {
    int value_{ 0 };
    bool runThread_{ true };
public:
    using Thread::Thread;
    
    virtual ~TheadTest() {
    }

    int intValue() const noexcept { return value_; }
    void stop() { runThread_ = false; }

    virtual void run() override {
        while (runThread_) {
            value_++;
            jxx::lang::Thread::sleep(300);
        }
    }   
 
};

class Thead2Test : public jxx::lang::Thread {
    int value_{ 0 };
    bool runThread_{ true };
public:
    using Thread::Thread;

    virtual ~Thead2Test() {
    }

    int intValue() const noexcept { return value_; }
    void stop() { runThread_ = false; }

    virtual void run() override {
        while (runThread_) {
            value_++;
            jxx::lang::Thread::sleep(300);
        }
    }

};
class RunnableTest : public jxx::lang::Runnable {
    int value_{ 0 };
    bool runThread_{ true };
public:

    int intValue() const noexcept { return value_; }
    void stop() { runThread_ = false; }

    virtual void run() override {
        while (runThread_) {
            value_++;
            jxx::lang::Thread::sleep(300);
        }
    }
};

class TestTheadTest : public testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    TestTheadTest() {
        // You can do set-up work for each test here.
    }

    ~TestTheadTest() override {
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
TEST(TestTheadTest, BasicAssertions) {
    auto t = NEW(TheadTest);
    EXPECT_EQ(0, t->intValue());
    t->start();
    jxx::lang::Thread::sleep(500);    
    EXPECT_EQ(2, t->intValue());
    t->stop();
    jxx::lang::Thread::sleep(1000);
    EXPECT_EQ(2, t->intValue());
    jxx::lang::Thread::sleep(1000);
    EXPECT_EQ(2, t->intValue());
}

TEST(TestTheadTest, Runnable) {
    auto r = NEW(RunnableTest);
    auto t = NEW(jxx::lang::Thread, r);
      
    // Start the thread
    t->start();
    EXPECT_EQ(0, r->intValue());
    jxx::lang::Thread::sleep(500);
    
    EXPECT_EQ(2, r->intValue());
    r->stop();
    jxx::lang::Thread::sleep(1000);
    EXPECT_EQ(2, r->intValue());
    jxx::lang::Thread::sleep(1000);
    EXPECT_EQ(2, r->intValue());
    t->join();
}
TEST(TestTheadTest, MultiThread) {

    auto r = NEW(RunnableTest);
    auto t1 = NEW(jxx::lang::Thread, r, "Worker1");
    auto t2 = NEW(jxx::lang::Thread, r, "Worker2");
    auto t3 = NEW(jxx::lang::Thread, r, "Worker3");

    EXPECT_EQ(0, r->intValue());

    // Start the thread
    t1->start();
    t2->start();
    t3->start();

    jxx::lang::Thread::sleep(500);
    r->stop();
 
    // we waited for 2 seconds, count should be 6
    // all 3 threads should run twice
    EXPECT_EQ(6, r->intValue());

    try {
        // Wait for both threads to finish
        t1->join();
        t2->join();
        t3->join();
    }
    catch (jxx::lang::Exception *e) {
    }
}

TEST(TestTheadTest, IntValueTestScope) {
      
    auto t = NEW(TheadTest);
    auto t2 = NEW(Thead2Test);
    EXPECT_EQ(0, t->intValue());
    EXPECT_EQ(0, t2->intValue());
    t->start();
    t2->start();
    jxx::lang::Thread::sleep(500);
   
    EXPECT_EQ(2, t->intValue());
    EXPECT_EQ(2, t2->intValue());
    t->stop();
    t2->stop();
    jxx::lang::Thread::sleep(1000);
    EXPECT_EQ(2, t->intValue());
    EXPECT_EQ(2, t2->intValue());
    jxx::lang::Thread::sleep(1000);
    EXPECT_EQ(2, t->intValue());
    EXPECT_EQ(2, t2->intValue());

    t->join();
    t2->join();
}
