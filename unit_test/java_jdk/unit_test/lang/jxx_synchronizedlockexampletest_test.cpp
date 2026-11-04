#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.Thread.h"

class JxxAnonymousRunnable0 final : public ::jxx::lang::ClassBase<JxxAnonymousRunnable0, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<JxxAnonymousRunnable0, ::jxx::lang::Object, ::jxx::lang::Runnable>;
    explicit JxxAnonymousRunnable0(const ::jxx::Ptr<SynchronizedLockExample>& example)
        : example_(example) {}
    virtual void run() override {
        for (::jxx::lang::jint i = 0; (i < 1000); i++) {
            example_->increment();
        }
        example_->add(10);
    }
private:
    ::jxx::Ptr<SynchronizedLockExample> example_;
};

TEST(SynchronizedLockExampleTest, TwoThreadsShouldProduceCountOf2020) {
    const ::jxx::Ptr<SynchronizedLockExample> example = ::jxx::NEW<SynchronizedLockExample>();
    ::jxx::Ptr<::jxx::lang::Runnable> task = ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<JxxAnonymousRunnable0>(example));
    ::jxx::Ptr<::jxx::lang::Thread> firstThread = ::jxx::NEW<::jxx::lang::Thread>(task, ::jxx::NEW<::jxx::lang::String>("FirstThread"));
    ::jxx::Ptr<::jxx::lang::Thread> secondThread = ::jxx::NEW<::jxx::lang::Thread>(task, ::jxx::NEW<::jxx::lang::String>("SecondThread"));
    firstThread->start();
    secondThread->start();
    firstThread->join();
    secondThread->join();
    EXPECT_EQ(2020, example->getCounter());
}

