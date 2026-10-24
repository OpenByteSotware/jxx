#pragma once

#include <cstdint>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::io {
class ObjectInputStream;
class ObjectOutputStream;
}

namespace jxx::util::function {
class IntBinaryOperator;
class IntConsumer;
class IntPredicate;
class IntUnaryOperator;
}

namespace jxx::util {
class IntSummaryStatistics;
class OptionalDouble;
class OptionalInt;

class IntStream final
    : public ::jxx::lang::ClassBase<
          IntStream,
          ::jxx::lang::Object> {
public:
    explicit IntStream(const ::jxx::lang::IntArray& backing);

    static ::jxx::Ptr<IntStream> empty();
    static ::jxx::Ptr<IntStream> of(::jxx::lang::jint value);
    static ::jxx::Ptr<IntStream> of(const ::jxx::lang::IntArray& values);
    static ::jxx::Ptr<IntStream> range(
        ::jxx::lang::jint startInclusive,
        ::jxx::lang::jint endExclusive);
    static ::jxx::Ptr<IntStream> rangeClosed(
        ::jxx::lang::jint startInclusive,
        ::jxx::lang::jint endInclusive);
    static ::jxx::Ptr<IntStream> concat(
        const ::jxx::Ptr<IntStream>& first,
        const ::jxx::Ptr<IntStream>& second);

    ::jxx::Ptr<IntStream> filter(
        const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate);
    ::jxx::Ptr<IntStream> map(
        const ::jxx::Ptr<::jxx::util::function::IntUnaryOperator>& mapper);
    ::jxx::Ptr<IntStream> distinct();
    ::jxx::Ptr<IntStream> sorted();
    ::jxx::Ptr<IntStream> peek(
        const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action);
    ::jxx::Ptr<IntStream> limit(::jxx::lang::jlong maxSize);
    ::jxx::Ptr<IntStream> skip(::jxx::lang::jlong count);

    void forEach(
        const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action);
    void forEachOrdered(
        const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action);
    ::jxx::lang::IntArray toArray();
    ::jxx::lang::jint reduce(
        ::jxx::lang::jint identity,
        const ::jxx::Ptr<::jxx::util::function::IntBinaryOperator>& operation);
    ::jxx::Ptr<OptionalInt> reduce(
        const ::jxx::Ptr<::jxx::util::function::IntBinaryOperator>& operation);
    ::jxx::lang::jint sum();
    ::jxx::Ptr<OptionalInt> min();
    ::jxx::Ptr<OptionalInt> max();
    ::jxx::lang::jlong count();
    ::jxx::Ptr<OptionalDouble> average();
    ::jxx::Ptr<IntSummaryStatistics> summaryStatistics();
    ::jxx::lang::jbool anyMatch(
        const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate);
    ::jxx::lang::jbool allMatch(
        const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate);
    ::jxx::lang::jbool noneMatch(
        const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate);
    ::jxx::Ptr<OptionalInt> findFirst();
    ::jxx::Ptr<OptionalInt> findAny();

    // Existing JXX convenience. This does not consume the stream.
    ::jxx::lang::jint size() const noexcept;


private:
    IntStream();

    void ensureUsable_() const;
    ::jxx::lang::IntArray takeData_();
    static ::jxx::lang::IntArray copyOf_(
        const ::jxx::lang::IntArray& source,
        std::uint32_t length);

    ::jxx::lang::IntArray data_;
    ::jxx::lang::jbool consumed_ = false;
};

} // namespace jxx::util
