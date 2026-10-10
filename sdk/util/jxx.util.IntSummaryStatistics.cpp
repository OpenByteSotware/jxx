#include <algorithm>
#include <sstream>
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.IntSummaryStatistics.h"
namespace jxx::util {
void IntSummaryStatistics::accept(::jxx::lang::jint value) { ++count_; sum_ += value; min_ = std::min(min_, value); max_ = std::max(max_, value); }
void IntSummaryStatistics::combine(const ::jxx::Ptr<IntSummaryStatistics>& other) { if (!other) throw ::jxx::lang::NullPointerException(); count_ += other->count_; sum_ += other->sum_; min_ = std::min(min_, other->min_); max_ = std::max(max_, other->max_); }
::jxx::lang::jlong IntSummaryStatistics::getCount() const noexcept { return count_; }
::jxx::lang::jlong IntSummaryStatistics::getSum() const noexcept { return sum_; }
::jxx::lang::jint IntSummaryStatistics::getMin() const noexcept { return min_; }
::jxx::lang::jint IntSummaryStatistics::getMax() const noexcept { return max_; }
::jxx::lang::jdouble IntSummaryStatistics::getAverage() const noexcept { return count_ == 0 ? 0.0 : static_cast<::jxx::lang::jdouble>(sum_) / static_cast<::jxx::lang::jdouble>(count_); }
::jxx::Ptr<::jxx::lang::String> IntSummaryStatistics::toString() const { std::ostringstream out; out << "IntSummaryStatistics{count=" << count_ << ", sum=" << sum_ << ", min=" << min_ << ", average=" << getAverage() << ", max=" << max_ << '}'; return ::jxx::NEW<::jxx::lang::String>(out.str()); }
} // namespace jxx::util
