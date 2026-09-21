#include <gtest/gtest.h>

#include <atomic>
#include <thread>

#include "lang/jxx.lang.ClassValue.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.ThreadLocal.h"
#include "util/function/jxx.util.function.Supplier.h"

namespace {
using ::jxx::lang::ClassAny;
using ::jxx::lang::ClassValue;
using ::jxx::lang::Integer;
using ::jxx::lang::Long;
using ::jxx::lang::String;
using ::jxx::lang::ThreadLocal;

class CountingClassValue final : public ClassValue<::jxx::Ptr<String>> {
public:
    std::atomic<int> computations{0};
protected:
    ::jxx::Ptr<String> computeValue(const ::jxx::Ptr<ClassAny>& type) override {
        (void)type;
        const auto value = ++computations;
        return ::jxx::NEW<String>(std::to_string(value));
    }
};

class CountingSupplier final : public ::jxx::util::function::Supplier<String> {
public:
    std::atomic<int> calls{0};
    ::jxx::Ptr<String> get() override {
        return ::jxx::NEW<String>(std::to_string(++calls));
    }
};

TEST(ClassValueEdgeTest, ComputesOnceUntilRemoveThenRecomputes) {
    const auto values = ::jxx::NEW<CountingClassValue>();
    const auto type = Integer::TYPE;

    EXPECT_EQ("1", values->get(type)->utf8());
    EXPECT_EQ("1", values->get(type)->utf8());
    EXPECT_EQ(1, values->computations.load());

    values->remove(type);
    EXPECT_EQ("2", values->get(type)->utf8());
    EXPECT_EQ(2, values->computations.load());
}

TEST(ClassValueEdgeTest, ValuesAreIndependentPerClassKey) {
    const auto values = ::jxx::NEW<CountingClassValue>();
    EXPECT_EQ("1", values->get(Integer::TYPE)->utf8());
    EXPECT_EQ("2", values->get(Long::TYPE)->utf8());
    EXPECT_EQ("1", values->get(Integer::TYPE)->utf8());
}

TEST(ClassValueEdgeTest, RacingGetsPublishOneInstalledValue) {
    const auto values = ::jxx::NEW<CountingClassValue>();
    ::jxx::Ptr<String> first;
    ::jxx::Ptr<String> second;
    std::thread one([&] { first = values->get(Integer::TYPE); });
    std::thread two([&] { second = values->get(Integer::TYPE); });
    one.join();
    two.join();
    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    EXPECT_EQ(first->utf8(), second->utf8());
    EXPECT_EQ(1, values->computations.load());
}

TEST(ThreadLocalEdgeTest, InitialValueIsLazyPerThreadAndRemoveReinitializes) {
    const auto supplier = ::jxx::NEW<CountingSupplier>();
    const auto local = ThreadLocal<String>::withInitial(supplier);

    EXPECT_EQ("1", local->get()->utf8());
    EXPECT_EQ("1", local->get()->utf8());
    local->remove();
    EXPECT_EQ("2", local->get()->utf8());

    ::jxx::Ptr<String> child;
    std::thread thread([&] { child = local->get(); });
    thread.join();
    ASSERT_NE(nullptr, child);
    EXPECT_EQ("3", child->utf8());
    EXPECT_EQ(3, supplier->calls.load());
}

TEST(ThreadLocalEdgeTest, SetBeforeGetSuppressesInitialValue) {
    const auto supplier = ::jxx::NEW<CountingSupplier>();
    const auto local = ThreadLocal<String>::withInitial(supplier);
    local->set(::jxx::NEW<String>("preset"));
    EXPECT_EQ("preset", local->get()->utf8());
    EXPECT_EQ(0, supplier->calls.load());
}

} // namespace
