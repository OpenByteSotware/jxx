#pragma once
#include <climits>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
#include "util/function/jxx.util.function.LongConsumer.h"
#include "util/function/jxx.util.function.IntConsumer.h"
namespace jxx::util {
class LongSummaryStatistics : public ::jxx::lang::ClassBase<LongSummaryStatistics, ::jxx::lang::Object, ::jxx::util::function::LongConsumer, ::jxx::util::function::IntConsumer> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<LongSummaryStatistics, JxxSuper, ::jxx::util::function::LongConsumer, ::jxx::util::function::IntConsumer>;
    LongSummaryStatistics() = default;
    void accept(::jxx::lang::jlong value) override;
    void accept(::jxx::lang::jint value) override;
    void combine(const ::jxx::Ptr<LongSummaryStatistics>& other);
    ::jxx::lang::jlong getCount() const noexcept;
    ::jxx::lang::jlong getSum() const noexcept;
    ::jxx::lang::jlong getMin() const noexcept;
    ::jxx::lang::jlong getMax() const noexcept;
    ::jxx::lang::jdouble getAverage() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
    ::jxx::lang::jlong count_ = 0;
    ::jxx::lang::jlong sum_ = 0;
    ::jxx::lang::jlong min_ = LLONG_MAX;
    ::jxx::lang::jlong max_ = LLONG_MIN;
};
} // namespace jxx::util
