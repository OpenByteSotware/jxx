#include <gtest/gtest.h>
#include <vector>

#include "util/function/jxx.util.function.IntConsumer.h"

namespace {
class RecordingIntConsumer final
    : public ::jxx::lang::ClassBase<
          RecordingIntConsumer,
          ::jxx::lang::Object,
          ::jxx::util::function::IntConsumer> {
public:
    RecordingIntConsumer(
        std::vector<::jxx::lang::jint>* values,
        ::jxx::lang::jint adjustment,
        bool fail)
        : values_(values), adjustment_(adjustment), fail_(fail) {
    }

    void accept(::jxx::lang::jint value) override {
        values_->push_back(value + adjustment_);
        if (fail_) {
            throw ::jxx::lang::IllegalStateException();
        }
    }

private:
    std::vector<::jxx::lang::jint>* values_;
    ::jxx::lang::jint adjustment_;
    bool fail_;
};
}

TEST(JxxPrimitiveConsumerParity, AndThenExecutesInOrder) {
    std::vector<::jxx::lang::jint> values;
    auto first = ::jxx::CAST<::jxx::util::function::IntConsumer>(
        ::jxx::NEW<RecordingIntConsumer>(&values, 1, false));
    auto second = ::jxx::CAST<::jxx::util::function::IntConsumer>(
        ::jxx::NEW<RecordingIntConsumer>(&values, 2, false));

    first->andThen(second)->accept(10);
    EXPECT_EQ(values, (std::vector<::jxx::lang::jint>{11, 12}));
}

TEST(JxxPrimitiveConsumerParity, AndThenDoesNotRunSecondAfterFailure) {
    std::vector<::jxx::lang::jint> values;
    auto first = ::jxx::CAST<::jxx::util::function::IntConsumer>(
        ::jxx::NEW<RecordingIntConsumer>(&values, 1, true));
    auto second = ::jxx::CAST<::jxx::util::function::IntConsumer>(
        ::jxx::NEW<RecordingIntConsumer>(&values, 2, false));

    auto chained = first->andThen(second);
    EXPECT_THROW(chained->accept(10), ::jxx::lang::IllegalStateException);
    EXPECT_EQ(values, (std::vector<::jxx::lang::jint>{11}));
}

TEST(JxxPrimitiveConsumerParity, AndThenRejectsNull) {
    std::vector<::jxx::lang::jint> values;
    auto first = ::jxx::CAST<::jxx::util::function::IntConsumer>(
        ::jxx::NEW<RecordingIntConsumer>(&values, 0, false));
    EXPECT_THROW(first->andThen(nullptr), ::jxx::lang::NullPointerException);
}
