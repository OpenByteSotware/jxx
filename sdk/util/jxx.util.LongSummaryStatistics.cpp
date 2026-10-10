#include <algorithm>
#include <sstream>
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.LongSummaryStatistics.h"
namespace jxx::util {
void LongSummaryStatistics::accept(::jxx::lang::jlong value) { ++count_; sum_ += value; min_ = std::min(min_, value); max_ = std::max(max_, value); }
void LongSummaryStatistics::accept(::jxx::lang::jint value) { accept(static_cast<::jxx::lang::jlong>(value)); }
void LongSummaryStatistics::combine(const ::jxx::Ptr<LongSummaryStatistics>& other) { if (!other) throw ::jxx::lang::NullPointerException(); count_ += other->count_; sum_ += other->sum_; min_ = std::min(min_, other->min_); max_ = std::max(max_, other->max_); }
::jxx::lang::jlong LongSummaryStatistics::getCount() const noexcept { return count_; }
::jxx::lang::jlong LongSummaryStatistics::getSum() const noexcept { return sum_; }
::jxx::lang::jlong LongSummaryStatistics::getMin() const noexcept { return min_; }
::jxx::lang::jlong LongSummaryStatistics::getMax() const noexcept { return max_; }
::jxx::lang::jdouble LongSummaryStatistics::getAverage() const noexcept { return count_ == 0 ? 0.0 : static_cast<::jxx::lang::jdouble>(sum_) / static_cast<::jxx::lang::jdouble>(count_); }
::jxx::Ptr<::jxx::lang::String> LongSummaryStatistics::toString() const { std::ostringstream out; out << "LongSummaryStatistics{count=" << count_ << ", sum=" << sum_ << ", min=" << min_ << ", average=" << getAverage() << ", max=" << max_ << '}'; return ::jxx::NEW<::jxx::lang::String>(out.str()); }
} // namespace jxx::util
