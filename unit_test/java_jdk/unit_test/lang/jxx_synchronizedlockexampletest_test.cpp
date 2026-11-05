#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.Objects.h"

class SynchronizedLockExample final : public ::jxx::lang::ClassBase<SynchronizedLockExample, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SynchronizedLockExample, ::jxx::lang::Object>;
    using JxxClassInfoMarker = ::jxx::lang::ClassInfo<SynchronizedLockExample, ::jxx::lang::Object>;
    virtual void increment() {
        counterLock->synchronized([&]() {
            counter++;
        });
    }
    virtual void add(::jxx::lang::jint amount) {
        if ((amount <= 0)) {
            return;
        }
        counterLock->synchronized([&]() {
            counter += amount;
        });
    }
    virtual ::jxx::lang::jint getCounter() {
        return counterLock->synchronized([&]() -> ::jxx::lang::jint {
            return counter;
        });
    }
private:
    ::jxx::lang::jint counter = 0;
    ::jxx::Ptr<::jxx::lang::Object> counterLock = ::jxx::NEW<::jxx::lang::Object>();
};

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

