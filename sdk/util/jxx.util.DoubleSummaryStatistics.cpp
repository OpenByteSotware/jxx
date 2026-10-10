#include <algorithm>
#include <cmath>
#include <sstream>
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.DoubleSummaryStatistics.h"
namespace jxx::util {
void DoubleSummaryStatistics::sumWithCompensation_(::jxx::lang::jdouble value) noexcept { auto corrected=value-compensation_; auto next=sum_+corrected; compensation_=(next-sum_)-corrected; sum_=next; }
void DoubleSummaryStatistics::accept(::jxx::lang::jdouble value){++count_;simpleSum_+=value;sumWithCompensation_(value);min_=std::min(min_,value);max_=std::max(max_,value);}
void DoubleSummaryStatistics::combine(const ::jxx::Ptr<DoubleSummaryStatistics>& o){if(!o)throw ::jxx::lang::NullPointerException();count_+=o->count_;simpleSum_+=o->simpleSum_;sumWithCompensation_(o->sum_);sumWithCompensation_(-o->compensation_);min_=std::min(min_,o->min_);max_=std::max(max_,o->max_);}
::jxx::lang::jlong DoubleSummaryStatistics::getCount()const noexcept{return count_;} ::jxx::lang::jdouble DoubleSummaryStatistics::getSum()const noexcept{auto corrected=sum_-compensation_;return std::isnan(corrected)&&std::isinf(simpleSum_)?simpleSum_:corrected;} ::jxx::lang::jdouble DoubleSummaryStatistics::getMin()const noexcept{return min_;} ::jxx::lang::jdouble DoubleSummaryStatistics::getMax()const noexcept{return max_;} ::jxx::lang::jdouble DoubleSummaryStatistics::getAverage()const noexcept{return count_==0?0.0:getSum()/static_cast<::jxx::lang::jdouble>(count_);} ::jxx::Ptr<::jxx::lang::String> DoubleSummaryStatistics::toString()const{std::ostringstream out;out<<"DoubleSummaryStatistics{count="<<count_<<", sum="<<getSum()<<", min="<<min_<<", average="<<getAverage()<<", max="<<max_<<'}';return ::jxx::NEW<::jxx::lang::String>(out.str());}
}
