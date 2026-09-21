#include <gtest/gtest.h>

#include <vector>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.EmptyStackException.h"
#include "util/jxx.util.EventListenerProxy.h"
#include "util/jxx.util.PrimitiveIterator.h"
#include "util/jxx.util.Stack.h"

namespace {

class Listener final : public ::jxx::util::EventListener {
public:
    ~Listener() override = default;
};

class ListenerProxy final
    : public ::jxx::util::EventListenerProxy<Listener> {
public:
    explicit ListenerProxy(const ::jxx::Ptr<Listener>& listener)
        : EventListenerProxy(listener) {}
};

class IntIterator final : public ::jxx::util::PrimitiveIteratorOfInt {
public:
    explicit IntIterator(std::vector<::jxx::lang::jint> values)
        : values_(std::move(values)) {}

    ::jxx::lang::jbool hasNext() override {
        return index_ < values_.size();
    }

    ::jxx::lang::jint nextInt() override {
        if (!hasNext()) throw ::jxx::util::NoSuchElementException();
        return values_[index_++];
    }

private:
    std::vector<::jxx::lang::jint> values_;
    std::size_t index_ = 0;
};

class SumConsumer final : public ::jxx::util::function::IntConsumer {
public:
    void accept(::jxx::lang::jint value) override { sum += value; }
    ::jxx::lang::jint sum = 0;
};

TEST(EventListenerProxyParityTest, RetainsSuppliedListener) {
    const auto listener = ::jxx::NEW<Listener>();
    const auto proxy = ::jxx::NEW<ListenerProxy>(listener);
    EXPECT_EQ(listener.get(), proxy->getListener().get());
}

TEST(PrimitiveIteratorParityTest, SupportsPrimitiveAndBoxedIteration) {
    auto iterator = ::jxx::NEW<IntIterator>(
        std::vector<::jxx::lang::jint>{1, 2, 3});
    EXPECT_EQ(1, iterator->nextInt());
    EXPECT_EQ(2, iterator->next()->intValue());
    const auto consumer = ::jxx::NEW<SumConsumer>();
    iterator->forEachRemaining(
        ::jxx::CAST<::jxx::util::function::IntConsumer>(consumer));
    EXPECT_EQ(3, consumer->sum);
}

TEST(StackParityTest, UsesLastInFirstOutOperationsAndOneBasedSearch) {
    auto stack = ::jxx::NEW<::jxx::util::Stack<::jxx::lang::String>>();
    const auto first = ::jxx::NEW<::jxx::lang::String>("first");
    const auto second = ::jxx::NEW<::jxx::lang::String>("second");
    stack->push(first);
    stack->push(second);
    EXPECT_EQ(second.get(), stack->peek().get());
    EXPECT_EQ(1, stack->search(::jxx::CAST<::jxx::lang::Object>(second)));
    EXPECT_EQ(2, stack->search(::jxx::CAST<::jxx::lang::Object>(first)));
    EXPECT_EQ(second.get(), stack->pop().get());
    EXPECT_FALSE(stack->empty());
    EXPECT_EQ(first.get(), stack->pop().get());
    EXPECT_TRUE(stack->empty());
    EXPECT_THROW(stack->peek(), ::jxx::util::EmptyStackException);
}

} // namespace
