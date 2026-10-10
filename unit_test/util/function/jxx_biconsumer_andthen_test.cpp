#include <gtest/gtest.h>
#include <vector>

#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.BiConsumer.h"

namespace {
class RecordingBiConsumer final
    : public ::jxx::lang::ClassBase<
          RecordingBiConsumer,
          ::jxx::lang::Object,
          ::jxx::util::function::BiConsumer<
              ::jxx::lang::String,
              ::jxx::lang::String>> {
public:
    RecordingBiConsumer(
        std::vector<int>* calls,
        int marker,
        bool fail)
        : calls_(calls), marker_(marker), fail_(fail) {
    }

    void accept(
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::lang::String>&) override {
        calls_->push_back(marker_);
        if (fail_) {
            throw ::jxx::lang::IllegalStateException();
        }
    }

private:
    std::vector<int>* calls_;
    int marker_;
    bool fail_;
};
} // namespace

TEST(JxxBiConsumerAndThenParity, ExecutesInOrder) {
    std::vector<int> calls;
    auto first = ::jxx::CAST<::jxx::util::function::BiConsumer<
        ::jxx::lang::String, ::jxx::lang::String>>(
            ::jxx::NEW<RecordingBiConsumer>(&calls, 1, false));
    auto second = ::jxx::CAST<::jxx::util::function::BiConsumer<
        ::jxx::lang::String, ::jxx::lang::String>>(
            ::jxx::NEW<RecordingBiConsumer>(&calls, 2, false));

    first->andThen(second)->accept(
        ::jxx::NEW<::jxx::lang::String>("a"),
        ::jxx::NEW<::jxx::lang::String>("b"));

    EXPECT_EQ(calls, (std::vector<int>{1, 2}));
}

TEST(JxxBiConsumerAndThenParity, StopsWhenFirstThrows) {
    std::vector<int> calls;
    auto first = ::jxx::CAST<::jxx::util::function::BiConsumer<
        ::jxx::lang::String, ::jxx::lang::String>>(
            ::jxx::NEW<RecordingBiConsumer>(&calls, 1, true));
    auto second = ::jxx::CAST<::jxx::util::function::BiConsumer<
        ::jxx::lang::String, ::jxx::lang::String>>(
            ::jxx::NEW<RecordingBiConsumer>(&calls, 2, false));

    auto chained = first->andThen(second);
    EXPECT_THROW(
        chained->accept(
            ::jxx::NEW<::jxx::lang::String>("a"),
            ::jxx::NEW<::jxx::lang::String>("b")),
        ::jxx::lang::IllegalStateException);
    EXPECT_EQ(calls, (std::vector<int>{1}));
}

TEST(JxxBiConsumerAndThenParity, RejectsNullAfter) {
    std::vector<int> calls;
    auto first = ::jxx::CAST<::jxx::util::function::BiConsumer<
        ::jxx::lang::String, ::jxx::lang::String>>(
            ::jxx::NEW<RecordingBiConsumer>(&calls, 1, false));
    EXPECT_THROW(first->andThen(nullptr), ::jxx::lang::NullPointerException);
}
