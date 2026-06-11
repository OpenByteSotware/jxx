#include "util/jxx.util.Locale.h"
#include "util/jxx.util.DecimalFormat.h"
#include "util/jxx.util.StringPool.h"
#include "util/jxx.util.NumberFormat.h"

namespace jxx::util {

NumberFormat::NumberFormat()
    : decimalSeparator_('.'), groupingSeparator_(','), groupingUsed_(true) {}

jxx::Ptr<NumberFormat> NumberFormat::getInstance(const jxx::Ptr<Locale>& locale) {
    return DecimalFormat::ofPattern(StringPool::intern("#,##0.###"), locale ? locale : Locale::getDefault());
}

void NumberFormat::setGroupingUsed(jxx::lang::jbool groupingUsed) { groupingUsed_ = groupingUsed; }
jxx::lang::jbool NumberFormat::isGroupingUsed() const { return groupingUsed_; }
char NumberFormat::getDecimalSeparator() const { return decimalSeparator_; }
char NumberFormat::getGroupingSeparator() const { return groupingSeparator_; }

} // namespace jxx::util
