#pragma once

#include <cstdint>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "util/stream/jxx.util.stream.IntStream.h"

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

namespace jxx::util::stream::internal {

class IntArrayStream final
    : public ::jxx::lang::ClassBase<
          IntArrayStream,
          ::jxx::lang::Object,
          ::jxx::util::stream::IntStream> {
public:
    ::jxx::Ptr<::jxx::util::Iterator<::jxx::lang::Integer>> iterator() override;
    ::jxx::Ptr<::jxx::util::Spliterator<::jxx::lang::Integer>> spliterator() override;
    ::jxx::lang::jbool isParallel() override;
    ::jxx::Ptr<::jxx::util::stream::IntStream> sequential() override;
    ::jxx::Ptr<::jxx::util::stream::IntStream> parallel() override;
    ::jxx::Ptr<::jxx::util::stream::IntStream> unordered() override;
    ::jxx::Ptr<::jxx::util::stream::IntStream> onClose(const ::jxx::Ptr<::jxx::lang::Runnable>& closeHandler) override;
    void close() override;
    explicit IntArrayStream(const ::jxx::lang::IntArray& backing);


    ::jxx::Ptr<::jxx::util::stream::IntStream> filter(
        const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate);
    ::jxx::Ptr<::jxx::util::stream::IntStream> map(
        const ::jxx::Ptr<::jxx::util::function::IntUnaryOperator>& mapper);
    ::jxx::Ptr<::jxx::util::stream::IntStream> distinct();
    ::jxx::Ptr<::jxx::util::stream::IntStream> sorted();
    ::jxx::Ptr<::jxx::util::stream::IntStream> peek(
        const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action);
    ::jxx::Ptr<::jxx::util::stream::IntStream> limit(::jxx::lang::jlong maxSize);
    ::jxx::Ptr<::jxx::util::stream::IntStream> skip(::jxx::lang::jlong count);

    void forEach(
        const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action);
    void forEachOrdered(
        const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action);
    ::jxx::lang::IntArray toArray();
    ::jxx::lang::jint reduce(
        ::jxx::lang::jint identity,
        const ::jxx::Ptr<::jxx::util::function::IntBinaryOperator>& operation);
    ::jxx::Ptr<::jxx::util::OptionalInt> reduce(
        const ::jxx::Ptr<::jxx::util::function::IntBinaryOperator>& operation);
    ::jxx::lang::jint sum();
    ::jxx::Ptr<::jxx::util::OptionalInt> min();
    ::jxx::Ptr<::jxx::util::OptionalInt> max();
    ::jxx::lang::jlong count();
    ::jxx::Ptr<::jxx::util::OptionalDouble> average();
    ::jxx::Ptr<::jxx::util::IntSummaryStatistics> summaryStatistics();
    ::jxx::lang::jbool anyMatch(
        const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate);
    ::jxx::lang::jbool allMatch(
        const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate);
    ::jxx::lang::jbool noneMatch(
        const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate);
    ::jxx::Ptr<::jxx::util::OptionalInt> findFirst();
    ::jxx::Ptr<::jxx::util::OptionalInt> findAny();


private:
    IntArrayStream();

    void ensureUsable_() const;
    ::jxx::lang::IntArray takeData_();
    static ::jxx::lang::IntArray copyOf_(
        const ::jxx::lang::IntArray& source,
        std::uint32_t length);

    ::jxx::lang::IntArray data_;
    ::jxx::lang::jbool consumed_ = false;
    ::jxx::lang::jbool parallel_ = false;
    ::jxx::lang::jbool closed_ = false;
    ::jxx::Ptr<::jxx::lang::Runnable> closeHandler_;
};

} // namespace jxx::util::stream::internal
