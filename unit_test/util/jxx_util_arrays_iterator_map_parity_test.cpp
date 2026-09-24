#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.Iterator.h"
#include "util/function/jxx.util.function.Consumer.h"

namespace {
using ::jxx::lang::Integer;
using ::jxx::lang::String;
using ::jxx::util::Arrays;
using ::jxx::util::HashMap;
using ::jxx::util::Iterator;

class StringIterator final : public Iterator<String> {
public:
    explicit StringIterator(
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>>& values)
        : values_(values) {}

    ::jxx::lang::jbool hasNext() override {
        return position_ < static_cast<::jxx::lang::jint>(values_->length);
    }

    ::jxx::Ptr<String> next() override {
        return (*values_)[position_++];
    }

private:
    ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>> values_;
    ::jxx::lang::jint position_ = 0;
};

class RecordingConsumer final : public ::jxx::util::function::Consumer<String> {
public:
    void acceptSuper(const ::jxx::Ptr<String>& value) override {
        values += value->utf8();
    }
    std::string values;
};

TEST(ArraysParityTest, PrimitiveSortSearchCopyFillEqualsAndText) {
    auto values = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint, 1U>>(
        std::initializer_list<::jxx::lang::jint>{4, 1, 3, 2});
    Arrays::sort(values);
    EXPECT_EQ(1, (*values)[0]);
    EXPECT_EQ(3, Arrays::binarySearch(values, 4));
    EXPECT_EQ(-5, Arrays::binarySearch(values, 5));

    auto copy = Arrays::copyOf(values, 6);
    EXPECT_EQ(6U, copy->length);
    EXPECT_EQ(0, (*copy)[5]);
    Arrays::fill(copy, 4, 6, 9);
    EXPECT_EQ(9, (*copy)[4]);
    EXPECT_FALSE(Arrays::equals(values, copy));
    EXPECT_EQ("[1, 2, 3, 4]", Arrays::toString(values)->utf8());
}

TEST(ArraysParityTest, ReferenceOperationsUseObjectSemantics) {
    auto values = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>>(
        std::initializer_list<::jxx::Ptr<String>>{
            ::jxx::NEW<String>("b"), ::jxx::NEW<String>("a")});
    Arrays::sort(values);
    EXPECT_EQ("a", (*values)[0]->utf8());
    auto copy = Arrays::copyOf(values, 2);
    EXPECT_TRUE(Arrays::equals(values, copy));
    EXPECT_EQ("[a, b]", Arrays::toString(values)->utf8());
}

TEST(IteratorParityTest, DefaultMethodsMatchExpectedBehavior) {
    auto values = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::Ptr<String>, 1U>>(
        std::initializer_list<::jxx::Ptr<String>>{
            ::jxx::NEW<String>("a"), ::jxx::NEW<String>("b")});
    const auto iterator = ::jxx::NEW<StringIterator>(values);
    const auto consumer = ::jxx::NEW<RecordingConsumer>();
    iterator->forEachRemaining(consumer);
    EXPECT_EQ("ab", consumer->values);
    EXPECT_THROW(iterator->remove(), ::jxx::lang::UnsupportedOperationException);
}

TEST(MapParityTest, DefaultReplacementMethodsHandlePresenceAndNull) {
    const auto map = ::jxx::NEW<HashMap<String, String>>();
    const auto key = ::jxx::NEW<String>("key");
    const auto first = ::jxx::NEW<String>("first");
    const auto second = ::jxx::NEW<String>("second");

    EXPECT_EQ(nullptr, map->putIfAbsent(key, first));
    EXPECT_EQ(first.get(), map->putIfAbsent(key, second).get());
    EXPECT_TRUE(map->replace(key, first, second));
    EXPECT_EQ(second.get(), map->get(::jxx::CAST<::jxx::lang::Object>(key)).get());
    EXPECT_TRUE(map->remove(::jxx::CAST<::jxx::lang::Object>(key),
                            ::jxx::CAST<::jxx::lang::Object>(second)));
    EXPECT_TRUE(map->isEmpty());
}

} // namespace
