#pragma once
#include <climits>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
#include "util/function/jxx.util.function.IntConsumer.h"
namespace jxx::util {
class IntSummaryStatistics : public ::jxx::lang::ClassBase<IntSummaryStatistics, ::jxx::lang::Object, ::jxx::util::function::IntConsumer> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<IntSummaryStatistics, JxxSuper, ::jxx::util::function::IntConsumer>;
    IntSummaryStatistics() = default;
    void accept(::jxx::lang::jint value) override;
    void combine(const ::jxx::Ptr<IntSummaryStatistics>& other);
    ::jxx::lang::jlong getCount() const noexcept;
    ::jxx::lang::jlong getSum() const noexcept;
    ::jxx::lang::jint getMin() const noexcept;
    ::jxx::lang::jint getMax() const noexcept;
    ::jxx::lang::jdouble getAverage() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
    ::jxx::lang::jlong count_ = 0;
    ::jxx::lang::jlong sum_ = 0;
    ::jxx::lang::jint min_ = INT_MAX;
    ::jxx::lang::jint max_ = INT_MIN;
};
} // namespace jxx::util
