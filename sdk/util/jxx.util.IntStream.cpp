#include <algorithm>
#include <cstdint>
#include <limits>
#include <unordered_set>
#include <vector>

#include "util/jxx.util.IntStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
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

namespace jxx::util {
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

IntStream::IntStream()
    : data_(::jxx::NEW<::jxx::lang::IntArrayType>(0)) {
}

IntStream::IntStream(const ::jxx::lang::IntArray& backing)
    : data_(backing == nullptr
            ? ::jxx::NEW<::jxx::lang::IntArrayType>(0)
            : copyOf_(backing, backing->length)) {
}

::jxx::Ptr<IntStream> IntStream::empty() {
    return ::jxx::NEW<IntStream>(
        ::jxx::NEW<::jxx::lang::IntArrayType>(0));
}

::jxx::Ptr<IntStream> IntStream::of(::jxx::lang::jint value) {
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(1);
    (*values)[0] = value;
    return ::jxx::NEW<IntStream>(values);
}

::jxx::Ptr<IntStream> IntStream::of(
    const ::jxx::lang::IntArray& values) {
    if (values == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return ::jxx::NEW<IntStream>(values);
}

::jxx::Ptr<IntStream> IntStream::range(
    ::jxx::lang::jint startInclusive,
    ::jxx::lang::jint endExclusive) {
    if (startInclusive >= endExclusive) {
        return empty();
    }
    const auto length =
        static_cast<::jxx::lang::jlong>(endExclusive) -
        static_cast<::jxx::lang::jlong>(startInclusive);
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(
        checkedLength(length));
    for (std::uint32_t index = 0; index < values->length; ++index) {
        (*values)[index] = static_cast<::jxx::lang::jint>(
            static_cast<::jxx::lang::jlong>(startInclusive) + index);
    }
    return ::jxx::NEW<IntStream>(values);
}

::jxx::Ptr<IntStream> IntStream::rangeClosed(
    ::jxx::lang::jint startInclusive,
    ::jxx::lang::jint endInclusive) {
    if (startInclusive > endInclusive) {
        return empty();
    }
    const auto length =
        static_cast<::jxx::lang::jlong>(endInclusive) -
        static_cast<::jxx::lang::jlong>(startInclusive) + 1;
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(
        checkedLength(length));
    for (std::uint32_t index = 0; index < values->length; ++index) {
        (*values)[index] = static_cast<::jxx::lang::jint>(
            static_cast<::jxx::lang::jlong>(startInclusive) + index);
    }
    return ::jxx::NEW<IntStream>(values);
}

::jxx::Ptr<IntStream> IntStream::concat(
    const ::jxx::Ptr<IntStream>& first,
    const ::jxx::Ptr<IntStream>& second) {
    if (first == nullptr || second == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    auto firstData = first->takeData_();
    auto secondData = second->takeData_();
    const auto total =
        static_cast<::jxx::lang::jlong>(firstData->length) +
        static_cast<::jxx::lang::jlong>(secondData->length);
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(
        checkedLength(total));
    std::uint32_t output = 0;
    for (std::uint32_t index = 0; index < firstData->length; ++index) {
        (*values)[output++] = (*firstData)[index];
    }
    for (std::uint32_t index = 0; index < secondData->length; ++index) {
        (*values)[output++] = (*secondData)[index];
    }
    return ::jxx::NEW<IntStream>(values);
}

::jxx::Ptr<IntStream> IntStream::filter(
    const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) {
    if (predicate == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    std::vector<::jxx::lang::jint> values;
    values.reserve(source->length);
    for (std::uint32_t index = 0; index < source->length; ++index) {
        const auto value = (*source)[index];
        if (predicate->test(value)) values.push_back(value);
    }
    return ::jxx::NEW<IntStream>(fromVector(values));
}

::jxx::Ptr<IntStream> IntStream::map(
    const ::jxx::Ptr<::jxx::util::function::IntUnaryOperator>& mapper) {
    if (mapper == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(source->length);
    for (std::uint32_t index = 0; index < source->length; ++index) {
        (*values)[index] = mapper->applyAsInt((*source)[index]);
    }
    return ::jxx::NEW<IntStream>(values);
}

::jxx::Ptr<IntStream> IntStream::distinct() {
    auto source = takeData_();
    std::unordered_set<::jxx::lang::jint> seen;
    std::vector<::jxx::lang::jint> values;
    values.reserve(source->length);
    for (std::uint32_t index = 0; index < source->length; ++index) {
        const auto value = (*source)[index];
        if (seen.insert(value).second) values.push_back(value);
    }
    return ::jxx::NEW<IntStream>(fromVector(values));
}

::jxx::Ptr<IntStream> IntStream::sorted() {
    auto source = takeData_();
    std::vector<::jxx::lang::jint> values;
    values.reserve(source->length);
    for (std::uint32_t index = 0; index < source->length; ++index) {
        values.push_back((*source)[index]);
    }
    std::sort(values.begin(), values.end());
    return ::jxx::NEW<IntStream>(fromVector(values));
}

::jxx::Ptr<IntStream> IntStream::peek(
    const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) {
    if (action == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        action->accept((*source)[index]);
    }
    return ::jxx::NEW<IntStream>(source);
}

::jxx::Ptr<IntStream> IntStream::limit(::jxx::lang::jlong maxSize) {
    if (maxSize < 0) throw ::jxx::lang::IllegalArgumentException();
    auto source = takeData_();
    const auto length = static_cast<std::uint32_t>(
        std::min<::jxx::lang::jlong>(maxSize, source->length));
    return ::jxx::NEW<IntStream>(copyOf_(source, length));
}

::jxx::Ptr<IntStream> IntStream::skip(::jxx::lang::jlong countValue) {
    if (countValue < 0) throw ::jxx::lang::IllegalArgumentException();
    auto source = takeData_();
    const auto start = static_cast<std::uint32_t>(
        std::min<::jxx::lang::jlong>(countValue, source->length));
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(
        source->length - start);
    for (std::uint32_t index = start; index < source->length; ++index) {
        (*values)[index - start] = (*source)[index];
    }
    return ::jxx::NEW<IntStream>(values);
}

void IntStream::forEach(
    const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) {
    if (action == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        action->accept((*source)[index]);
    }
}

void IntStream::forEachOrdered(
    const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) {
    forEach(action);
}

::jxx::lang::IntArray IntStream::toArray() {
    auto source = takeData_();
    return copyOf_(source, source->length);
}

::jxx::lang::jint IntStream::reduce(
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

::jxx::Ptr<OptionalInt> IntStream::reduce(
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

::jxx::lang::jint IntStream::sum() {
    auto source = takeData_();
    ::jxx::lang::jint result = 0;
    for (std::uint32_t index = 0; index < source->length; ++index) {
        result = static_cast<::jxx::lang::jint>(
            static_cast<std::uint32_t>(result) +
            static_cast<std::uint32_t>((*source)[index]));
    }
    return result;
}

::jxx::Ptr<OptionalInt> IntStream::min() {
    auto source = takeData_();
    if (source->length == 0) return OptionalInt::empty();
    auto result = (*source)[0];
    for (std::uint32_t index = 1; index < source->length; ++index) {
        result = std::min(result, (*source)[index]);
    }
    return OptionalInt::of(result);
}

::jxx::Ptr<OptionalInt> IntStream::max() {
    auto source = takeData_();
    if (source->length == 0) return OptionalInt::empty();
    auto result = (*source)[0];
    for (std::uint32_t index = 1; index < source->length; ++index) {
        result = std::max(result, (*source)[index]);
    }
    return OptionalInt::of(result);
}

::jxx::lang::jlong IntStream::count() {
    return static_cast<::jxx::lang::jlong>(takeData_()->length);
}

::jxx::Ptr<OptionalDouble> IntStream::average() {
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

::jxx::Ptr<IntSummaryStatistics> IntStream::summaryStatistics() {
    auto source = takeData_();
    auto statistics = ::jxx::NEW<IntSummaryStatistics>();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        statistics->accept((*source)[index]);
    }
    return statistics;
}

::jxx::lang::jbool IntStream::anyMatch(
    const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) {
    if (predicate == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        if (predicate->test((*source)[index])) return true;
    }
    return false;
}

::jxx::lang::jbool IntStream::allMatch(
    const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) {
    if (predicate == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        if (!predicate->test((*source)[index])) return false;
    }
    return true;
}

::jxx::lang::jbool IntStream::noneMatch(
    const ::jxx::Ptr<::jxx::util::function::IntPredicate>& predicate) {
    if (predicate == nullptr) throw ::jxx::lang::NullPointerException();
    auto source = takeData_();
    for (std::uint32_t index = 0; index < source->length; ++index) {
        if (predicate->test((*source)[index])) return false;
    }
    return true;
}

::jxx::Ptr<OptionalInt> IntStream::findFirst() {
    auto source = takeData_();
    return source->length == 0
        ? OptionalInt::empty()
        : OptionalInt::of((*source)[0]);
}

::jxx::Ptr<OptionalInt> IntStream::findAny() {
    return findFirst();
}

::jxx::lang::jint IntStream::size() const noexcept {
    return data_ == nullptr
        ? 0
        : static_cast<::jxx::lang::jint>(data_->length);
}

void IntStream::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw ::jxx::lang::NullPointerException();
    out->writeObject(data_);
}

void IntStream::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw ::jxx::lang::NullPointerException();
    data_ = ::jxx::CAST<::jxx::lang::IntArrayType>(in->readObject());
    consumed_ = false;
    if (data_ == nullptr) {
        data_ = ::jxx::NEW<::jxx::lang::IntArrayType>(0);
    }
}

void IntStream::readObjectNoData() {
    data_ = ::jxx::NEW<::jxx::lang::IntArrayType>(0);
    consumed_ = false;
}

void IntStream::ensureUsable_() const {
    if (consumed_) throw ::jxx::lang::IllegalStateException();
}

::jxx::lang::IntArray IntStream::takeData_() {
    ensureUsable_();
    consumed_ = true;
    return data_ == nullptr
        ? ::jxx::NEW<::jxx::lang::IntArrayType>(0)
        : data_;
}

::jxx::lang::IntArray IntStream::copyOf_(
    const ::jxx::lang::IntArray& source,
    std::uint32_t length) {
    auto result = ::jxx::NEW<::jxx::lang::IntArrayType>(length);
    if (source == nullptr) return result;
    const auto copyLength = std::min(length, source->length);
    for (std::uint32_t index = 0; index < copyLength; ++index) {
        (*result)[index] = (*source)[index];
    }
    return result;
}

} // namespace jxx::util
