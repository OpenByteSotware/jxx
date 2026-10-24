#include <algorithm>
#include <cstdint>
#include <limits>
#include <unordered_set>
#include <vector>

#include "util/stream/internal/jxx.util.stream.internal.IntArrayStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.IntSummaryStatistics.h"
#include "util/jxx.util.OptionalDouble.h"
#include "util/jxx.util.OptionalInt.h"
#include "util/function/jxx.util.function.IntBinaryOperator.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/function/jxx.util.function.IntPredicate.h"
#include "util/function/jxx.util.function.IntUnaryOperator.h"

namespace jxx::util::stream::internal {
namespace {

std::uint32_t checkedLength(::jxx::lang::jlong length) {
    if (length < 0 ||
        static_cast<unsigned long long>(length) >
            static_cast<unsigned long long>(
                std::numeric_limits<std::uint32_t>::max())) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    return static_cast<std::uint32_t>(length);
}

::jxx::lang::IntArray fromVector(
    const std::vector<::jxx::lang::jint>& values) {
    auto result = ::jxx::NEW<::jxx::lang::IntArrayType>(
        static_cast<std::uint32_t>(values.size()));
    for (std::uint32_t index = 0; index < result->length; ++index) {
        (*result)[index] = values[index];
    }
    return result;
}

} // namespace

IntArrayStream::IntArrayStream()
    : data_(::jxx::NEW<::jxx::lang::IntArrayType>(0)) {
}

IntArrayStream::IntArrayStream(const ::jxx::lang::IntArray& backing)
    : data_(backing == nullptr
            ? ::jxx::NEW<::jxx::lang::IntArrayType>(0)
            : copyOf_(backing, backing->length)) {
}

::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::filter(
    const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) {
    if (predicate == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    std::vector<::jxx::lang::jint> values;
    values.reserve(source->length);
    for (std::uint32_t index = 0; index < source->length; ++index) {
        const auto value = (*source)[index];
        if (predicate->test(value)) values.push_back(value);
    }
    return ::jxx::NEW<IntArrayStream>(fromVector(values));
}

::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::map(
    const ::jxx::Ptr<::jxx::util::function::IntUnaryOperator>& mapper) {
    if (mapper == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(source->length);
    for (std::uint32_t index = 0; index < source->length; ++index) {
        (*values)[index] = mapper->applyAsInt((*source)[index]);
    }
    return ::jxx::NEW<IntArrayStream>(values);
}

::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::distinct() {
    auto source = takeData_();
    std::unordered_set<::jxx::lang::jint> seen;
    std::vector<::jxx::lang::jint> values;
    values.reserve(source->length);
    for (std::uint32_t index = 0; index < source->length; ++index) {
        const auto value = (*source)[index];
        if (seen.insert(value).second) values.push_back(value);
    }
    return ::jxx::NEW<IntArrayStream>(fromVector(values));
}

::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::sorted() {
    auto source = takeData_();
    std::vector<::jxx::lang::jint> values;
    values.reserve(source->length);
    for (std::uint32_t index = 0; index < source->length; ++index) {
        values.push_back((*source)[index]);
    }
    std::sort(values.begin(), values.end());
    return ::jxx::NEW<IntArrayStream>(fromVector(values));
}

::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::peek(
    const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) {
    if (action == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        action->accept((*source)[index]);
    }
    return ::jxx::NEW<IntArrayStream>(source);
}

::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::limit(::jxx::lang::jlong maxSize) {
    if (maxSize < 0) throw ::jxx::lang::IllegalArgumentException();
    auto source = takeData_();
    const auto sourceLength =
        static_cast<::jxx::lang::jlong>(source->length);
    const auto length = static_cast<std::uint32_t>(
        maxSize < sourceLength ? maxSize : sourceLength);
    return ::jxx::NEW<IntArrayStream>(copyOf_(source, length));
}

::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::skip(::jxx::lang::jlong countValue) {
    if (countValue < 0) throw ::jxx::lang::IllegalArgumentException();
    auto source = takeData_();
    const auto sourceLength =
        static_cast<::jxx::lang::jlong>(source->length);
    const auto start = static_cast<std::uint32_t>(
        countValue < sourceLength ? countValue : sourceLength);
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(
        source->length - start);
    for (std::uint32_t index = start; index < source->length; ++index) {
        (*values)[index - start] = (*source)[index];
    }
    return ::jxx::NEW<IntArrayStream>(values);
}

void IntArrayStream::forEach(
    const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) {
    if (action == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        action->accept((*source)[index]);
    }
}

void IntArrayStream::forEachOrdered(
    const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) {
    forEach(action);
}

::jxx::lang::IntArray IntArrayStream::toArray() {
    auto source = takeData_();
    return copyOf_(source, source->length);
}

::jxx::lang::jint IntArrayStream::reduce(
    ::jxx::lang::jint identity,
    const ::jxx::Ptr<::jxx::util::function::IntBinaryOperator>& operation) {
    if (operation == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    auto result = identity;
    for (std::uint32_t index = 0; index < source->length; ++index) {
        result = operation->applyAsInt(result, (*source)[index]);
    }
    return result;
}

::jxx::Ptr<OptionalInt> IntArrayStream::reduce(
    const ::jxx::Ptr<::jxx::util::function::IntBinaryOperator>& operation) {
    if (operation == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    if (source->length == 0) return OptionalInt::empty();
    auto result = (*source)[0];
    for (std::uint32_t index = 1; index < source->length; ++index) {
        result = operation->applyAsInt(result, (*source)[index]);
    }
    return OptionalInt::of(result);
}

::jxx::lang::jint IntArrayStream::sum() {
    auto source = takeData_();
    ::jxx::lang::jint result = 0;
    for (std::uint32_t index = 0; index < source->length; ++index) {
        result = static_cast<::jxx::lang::jint>(
            static_cast<std::uint32_t>(result) +
            static_cast<std::uint32_t>((*source)[index]));
    }
    return result;
}

::jxx::Ptr<OptionalInt> IntArrayStream::min() {
    auto source = takeData_();
    if (source->length == 0) return OptionalInt::empty();
    auto result = (*source)[0];
    for (std::uint32_t index = 1; index < source->length; ++index) {
        const auto value = (*source)[index];
        if (value < result) result = value;
    }
    return OptionalInt::of(result);
}

::jxx::Ptr<OptionalInt> IntArrayStream::max() {
    auto source = takeData_();
    if (source->length == 0) return OptionalInt::empty();
    auto result = (*source)[0];
    for (std::uint32_t index = 1; index < source->length; ++index) {
        const auto value = (*source)[index];
        if (value > result) result = value;
    }
    return OptionalInt::of(result);
}

::jxx::lang::jlong IntArrayStream::count() {
    return static_cast<::jxx::lang::jlong>(takeData_()->length);
}

::jxx::Ptr<OptionalDouble> IntArrayStream::average() {
    auto source = takeData_();
    if (source->length == 0) return OptionalDouble::empty();
    ::jxx::lang::jlong total = 0;
    for (std::uint32_t index = 0; index < source->length; ++index) {
        total += (*source)[index];
    }
    return OptionalDouble::of(
        static_cast<::jxx::lang::jdouble>(total) /
        static_cast<::jxx::lang::jdouble>(source->length));
}

::jxx::Ptr<IntSummaryStatistics> IntArrayStream::summaryStatistics() {
    auto source = takeData_();
    auto statistics = ::jxx::NEW<IntSummaryStatistics>();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        statistics->accept((*source)[index]);
    }
    return statistics;
}

::jxx::lang::jbool IntArrayStream::anyMatch(
    const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) {
    if (predicate == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        if (predicate->test((*source)[index])) return true;
    }
    return false;
}

::jxx::lang::jbool IntArrayStream::allMatch(
    const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) {
    if (predicate == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        if (!predicate->test((*source)[index])) return false;
    }
    return true;
}

::jxx::lang::jbool IntArrayStream::noneMatch(
    const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) {
    if (predicate == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        if (predicate->test((*source)[index])) return false;
    }
    return true;
}

::jxx::Ptr<OptionalInt> IntArrayStream::findFirst() {
    auto source = takeData_();
    return source->length == 0
        ? OptionalInt::empty()
        : OptionalInt::of((*source)[0]);
}

::jxx::Ptr<OptionalInt> IntArrayStream::findAny() {
    return findFirst();
}


void IntArrayStream::ensureUsable_() const {
    if (consumed_) throw ::jxx::lang::IllegalStateException();
}

::jxx::lang::IntArray IntArrayStream::takeData_() {
    ensureUsable_();
    consumed_ = true;
    return data_ == nullptr
        ? ::jxx::NEW<::jxx::lang::IntArrayType>(0)
        : data_;
}

::jxx::lang::IntArray IntArrayStream::copyOf_(
    const ::jxx::lang::IntArray& source,
    std::uint32_t length) {
    auto result = ::jxx::NEW<::jxx::lang::IntArrayType>(length);
    if (source == nullptr) return result;
    const std::uint32_t sourceLength =
        static_cast<std::uint32_t>(source->length);
    const std::uint32_t copyLength =
        length < sourceLength ? length : sourceLength;
    for (std::uint32_t index = 0; index < copyLength; ++index) {
        (*result)[index] = (*source)[index];
    }
    return result;
}


::jxx::Ptr<::jxx::util::Iterator<::jxx::lang::Integer>> IntArrayStream::iterator() { return nullptr; }
::jxx::Ptr<::jxx::util::Spliterator<::jxx::lang::Integer>> IntArrayStream::spliterator() { return nullptr; }
::jxx::lang::jbool IntArrayStream::isParallel() { ensureUsable_(); return parallel_; }
::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::sequential() { ensureUsable_(); parallel_ = false; return ::jxx::CAST<::jxx::util::stream::IntStream>(thisPtr()); }
::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::parallel() { ensureUsable_(); parallel_ = true; return ::jxx::CAST<::jxx::util::stream::IntStream>(thisPtr()); }
::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::unordered() { ensureUsable_(); return ::jxx::CAST<::jxx::util::stream::IntStream>(thisPtr()); }
::jxx::Ptr<::jxx::util::stream::IntStream> IntArrayStream::onClose(const ::jxx::Ptr<::jxx::lang::Runnable>& closeHandler) { if (!closeHandler) throw ::jxx::lang::NullPointerException(); ensureUsable_(); closeHandler_ = closeHandler; return ::jxx::CAST<::jxx::util::stream::IntStream>(thisPtr()); }
void IntArrayStream::close() { if (closed_) return; closed_ = true; if (closeHandler_) closeHandler_->run(); }
} // namespace jxx::util
