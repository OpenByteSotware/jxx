#pragma once
#include <limits>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
#include "util/function/jxx.util.function.DoubleConsumer.h"
namespace jxx::util {
class DoubleSummaryStatistics : public ::jxx::lang::ClassBase<DoubleSummaryStatistics, ::jxx::lang::Object, ::jxx::util::function::DoubleConsumer> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<DoubleSummaryStatistics,JxxSuper,::jxx::util::function::DoubleConsumer>;
 DoubleSummaryStatistics()=default; void accept(::jxx::lang::jdouble value) override; void combine(const ::jxx::Ptr<DoubleSummaryStatistics>& other);
 ::jxx::lang::jlong getCount() const noexcept; ::jxx::lang::jdouble getSum() const noexcept; ::jxx::lang::jdouble getMin() const noexcept; ::jxx::lang::jdouble getMax() const noexcept; ::jxx::lang::jdouble getAverage() const noexcept; ::jxx::Ptr<::jxx::lang::String> toString() const override;
private:
 ::jxx::lang::jlong count_=0; ::jxx::lang::jdouble sum_=0.0, compensation_=0.0, simpleSum_=0.0; ::jxx::lang::jdouble min_=std::numeric_limits<::jxx::lang::jdouble>::infinity(), max_=-std::numeric_limits<::jxx::lang::jdouble>::infinity(); void sumWithCompensation_(::jxx::lang::jdouble value) noexcept;
}; }
