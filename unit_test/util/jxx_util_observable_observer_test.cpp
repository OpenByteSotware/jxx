#include <gtest/gtest.h>

#include <vector>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Observable.h"
#include "util/jxx.util.Observer.h"

namespace {

class TestObservable final
    : public ::jxx::lang::ClassBase<
          TestObservable,
          ::jxx::util::Observable> {
public:
    using JxxSuper = ::jxx::util::Observable;
    using Super = ::jxx::lang::ClassBase<TestObservable, JxxSuper>;

    void markChanged() { setChanged(); }
    void clearChange() { clearChanged(); }
};

class RecordingObserver final
    : public ::jxx::lang::ClassBase<
          RecordingObserver,
          ::jxx::lang::Object,
          ::jxx::util::Observer> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        RecordingObserver, JxxSuper, ::jxx::util::Observer>;

    explicit RecordingObserver(::jxx::lang::jint identifier)
        : identifier_(identifier) {}

    void update(
        const ::jxx::Ptr<::jxx::util::Observable>& observable,
        const ::jxx::Ptr<::jxx::lang::Object>& argument) override {
        ++updates;
        source = observable;
        lastArgument = argument;
        if (order != nullptr) order->push_back(identifier_);
    }

    ::jxx::lang::jint updates = 0;
    ::jxx::Ptr<::jxx::util::Observable> source;
    ::jxx::Ptr<::jxx::lang::Object> lastArgument;
    std::vector<::jxx::lang::jint>* order = nullptr;

private:
    ::jxx::lang::jint identifier_;
};

TEST(ObservableParityTest, RequiresChangedStateAndPassesArgument) {
    const auto observable = ::jxx::NEW<TestObservable>();
    const auto observer = ::jxx::NEW<RecordingObserver>(1);
    const auto argument = ::jxx::NEW<::jxx::lang::String>("value");
    observable->addObserver(
        ::jxx::CAST<::jxx::util::Observer>(observer));

    observable->notifyObservers(
        ::jxx::CAST<::jxx::lang::Object>(argument));
    EXPECT_EQ(0, observer->updates);

    observable->markChanged();
    EXPECT_TRUE(observable->hasChanged());
    observable->notifyObservers(
        ::jxx::CAST<::jxx::lang::Object>(argument));

    EXPECT_EQ(1, observer->updates);
    EXPECT_EQ(observable.get(), observer->source.get());
    EXPECT_EQ(argument.get(), observer->lastArgument.get());
    EXPECT_FALSE(observable->hasChanged());
}

TEST(ObservableParityTest, SuppressesDuplicateObserverAndSupportsDeletion) {
    const auto observable = ::jxx::NEW<TestObservable>();
    const auto observer = ::jxx::NEW<RecordingObserver>(1);
    const auto observerInterface =
        ::jxx::CAST<::jxx::util::Observer>(observer);

    observable->addObserver(observerInterface);
    observable->addObserver(observerInterface);
    EXPECT_EQ(1, observable->countObservers());

    observable->deleteObserver(observerInterface);
    EXPECT_EQ(0, observable->countObservers());
    observable->deleteObserver(observerInterface);
    EXPECT_EQ(0, observable->countObservers());
}

TEST(ObservableParityTest, NotifiesSnapshotInRegistrationOrder) {
    const auto observable = ::jxx::NEW<TestObservable>();
    const auto first = ::jxx::NEW<RecordingObserver>(1);
    const auto second = ::jxx::NEW<RecordingObserver>(2);
    std::vector<::jxx::lang::jint> order;
    first->order = &order;
    second->order = &order;

    observable->addObserver(::jxx::CAST<::jxx::util::Observer>(first));
    observable->addObserver(::jxx::CAST<::jxx::util::Observer>(second));
    observable->markChanged();
    observable->notifyObservers();

    ASSERT_EQ(2U, order.size());
    EXPECT_EQ(1, order[0]);
    EXPECT_EQ(2, order[1]);
}

TEST(ObservableParityTest, DeleteObserversAndProtectedClearChangedWork) {
    const auto observable = ::jxx::NEW<TestObservable>();
    observable->addObserver(::jxx::CAST<::jxx::util::Observer>(
        ::jxx::NEW<RecordingObserver>(1)));
    observable->markChanged();
    observable->clearChange();
    EXPECT_FALSE(observable->hasChanged());
    observable->deleteObservers();
    EXPECT_EQ(0, observable->countObservers());
}

TEST(ObservableParityTest, NullObserverIsRejected) {
    const auto observable = ::jxx::NEW<TestObservable>();
    EXPECT_THROW(
        observable->addObserver(nullptr),
        ::jxx::lang::NullPointerException);
}

} // namespace
