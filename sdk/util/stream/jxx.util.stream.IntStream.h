#pragma once

#include <type_traits>
#include <utility>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx_types.h"
#include "util/stream/jxx.util.stream.BaseStream.h"
#include "util/function/jxx.util.function.IntConsumer.h"

namespace jxx::util::function { class IntBinaryOperator; class IntConsumer; class IntPredicate; class IntUnaryOperator; }
namespace jxx::util { class IntSummaryStatistics; class OptionalDouble; class OptionalInt; }

namespace jxx::util::stream {
class IntStream
    : public ::jxx::lang::InterfaceBase<
          IntStream,
          BaseStream<::jxx::lang::Integer, IntStream>> {
public:
    ~IntStream() override = default;
    static ::jxx::Ptr<IntStream> empty();
    static ::jxx::Ptr<IntStream> of(::jxx::lang::jint value);
    static ::jxx::Ptr<IntStream> of(const ::jxx::lang::IntArray& values);
    static ::jxx::Ptr<IntStream> range(::jxx::lang::jint startInclusive, ::jxx::lang::jint endExclusive);
    static ::jxx::Ptr<IntStream> rangeClosed(::jxx::lang::jint startInclusive, ::jxx::lang::jint endInclusive);
    static ::jxx::Ptr<IntStream> concat(const ::jxx::Ptr<IntStream>& first, const ::jxx::Ptr<IntStream>& second);
    virtual ::jxx::Ptr<IntStream> filter(const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) = 0;
    virtual ::jxx::Ptr<IntStream> map(const ::jxx::Ptr<::jxx::util::function::IntUnaryOperator>& mapper) = 0;
    virtual ::jxx::Ptr<IntStream> distinct() = 0;
    virtual ::jxx::Ptr<IntStream> sorted() = 0;
    virtual ::jxx::Ptr<IntStream> peek(const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) = 0;
    virtual ::jxx::Ptr<IntStream> limit(::jxx::lang::jlong maxSize) = 0;
    virtual ::jxx::Ptr<IntStream> skip(::jxx::lang::jlong count) = 0;
    virtual void forEach(const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) = 0;

    template <typename Callable,
        std::enable_if_t<
            std::is_invocable_r_v<void, Callable&, ::jxx::lang::jint> &&
            !std::is_same_v<std::decay_t<Callable>,
                ::jxx::Ptr<::jxx::util::function::IntConsumer>>,
            int> = 0>
    void forEach(Callable&& action) {
        forEach(::jxx::util::function::int_consumer_detail::fromCallable(
            std::forward<Callable>(action)));
    }

    virtual void forEachOrdered(const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) = 0;

    template <typename Callable,
        std::enable_if_t<
            std::is_invocable_r_v<void, Callable&, ::jxx::lang::jint> &&
            !std::is_same_v<std::decay_t<Callable>,
                ::jxx::Ptr<::jxx::util::function::IntConsumer>>,
            int> = 0>
    void forEachOrdered(Callable&& action) {
        forEachOrdered(
            ::jxx::util::function::int_consumer_detail::fromCallable(
                std::forward<Callable>(action)));
    }
    virtual ::jxx::lang::IntArray toArray() = 0;
    virtual ::jxx::lang::jint reduce(::jxx::lang::jint identity, const ::jxx::Ptr<::jxx::util::function::IntBinaryOperator>& operation) = 0;
    virtual ::jxx::Ptr<::jxx::util::OptionalInt> reduce(const ::jxx::Ptr<::jxx::util::function::IntBinaryOperator>& operation) = 0;
    virtual ::jxx::lang::jint sum() = 0;
    virtual ::jxx::Ptr<::jxx::util::OptionalInt> min() = 0;
    virtual ::jxx::Ptr<::jxx::util::OptionalInt> max() = 0;
    virtual ::jxx::lang::jlong count() = 0;
    virtual ::jxx::Ptr<::jxx::util::OptionalDouble> average() = 0;
    virtual ::jxx::Ptr<::jxx::util::IntSummaryStatistics> summaryStatistics() = 0;
    virtual ::jxx::lang::jbool anyMatch(const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) = 0;
    virtual ::jxx::lang::jbool allMatch(const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) = 0;
    virtual ::jxx::lang::jbool noneMatch(const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) = 0;
    virtual ::jxx::Ptr<::jxx::util::OptionalInt> findFirst() = 0;
    virtual ::jxx::Ptr<::jxx::util::OptionalInt> findAny() = 0;
};
} // namespace jxx::util::stream
