#include "util/jxx.util.Locale.h"
#include "text/jxx.text.DecimalFormat.h"
#include "util/jxx.util.StringPool.h"
#include "text/jxx.text.NumberFormat.h"

namespace jxx::text {

::jxx::Ptr<::jxx::lang::ClassAny>
NumberFormat::Class() {
    return JxxClassInfoMarker::Class();
}

NumberFormat::NumberFormat()
    : decimalSeparator_('.'), groupingSeparator_(','), groupingUsed_(true) {}

::jxx::Ptr<NumberFormat> NumberFormat::getInstance(const ::jxx::Ptr<::jxx::util::Locale>& locale) {
    return DecimalFormat::ofPattern(::jxx::util::StringPool::intern("#,##0.###"), locale ? locale : ::jxx::util::Locale::getDefault());
}

void NumberFormat::setGroupingUsed(::jxx::lang::jbool groupingUsed) { groupingUsed_ = groupingUsed; }
::jxx::lang::jbool NumberFormat::isGroupingUsed() const { return groupingUsed_; }
char NumberFormat::getDecimalSeparator() const { return decimalSeparator_; }
char NumberFormat::getGroupingSeparator() const { return groupingSeparator_; }

} // namespace jxx::text
