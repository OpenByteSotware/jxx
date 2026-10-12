#include "util/function/jxx.util.function.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "util/function/jxx.util.function.BooleanSupplier.h"
#include "util/function/jxx.util.function.DoubleBinaryOperator.h"
#include "util/function/jxx.util.function.DoubleConsumer.h"
#include "util/function/jxx.util.function.DoublePredicate.h"
#include "util/function/jxx.util.function.DoubleSupplier.h"
#include "util/function/jxx.util.function.DoubleToIntFunction.h"
#include "util/function/jxx.util.function.DoubleToLongFunction.h"
#include "util/function/jxx.util.function.DoubleUnaryOperator.h"
#include "util/function/jxx.util.function.IntBinaryOperator.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/function/jxx.util.function.IntPredicate.h"
#include "util/function/jxx.util.function.IntSupplier.h"
#include "util/function/jxx.util.function.IntToDoubleFunction.h"
#include "util/function/jxx.util.function.IntToLongFunction.h"
#include "util/function/jxx.util.function.IntUnaryOperator.h"
#include "util/function/jxx.util.function.LongBinaryOperator.h"
#include "util/function/jxx.util.function.LongConsumer.h"
#include "util/function/jxx.util.function.LongPredicate.h"
#include "util/function/jxx.util.function.LongSupplier.h"
#include "util/function/jxx.util.function.LongToDoubleFunction.h"
#include "util/function/jxx.util.function.LongToIntFunction.h"
#include "util/function/jxx.util.function.LongUnaryOperator.h"

namespace jxx::util::function {
namespace {

template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<BooleanSupplier>();
        registerType<DoubleBinaryOperator>();
        registerType<DoubleConsumer>();
        registerType<DoublePredicate>();
        registerType<DoubleSupplier>();
        registerType<DoubleToIntFunction>();
        registerType<DoubleToLongFunction>();
        registerType<DoubleUnaryOperator>();
        registerType<IntBinaryOperator>();
        registerType<IntConsumer>();
        registerType<IntPredicate>();
        registerType<IntSupplier>();
        registerType<IntToDoubleFunction>();
        registerType<IntToLongFunction>();
        registerType<IntUnaryOperator>();
        registerType<LongBinaryOperator>();
        registerType<LongConsumer>();
        registerType<LongPredicate>();
        registerType<LongSupplier>();
        registerType<LongToDoubleFunction>();
        registerType<LongToIntFunction>();
        registerType<LongUnaryOperator>();
    });
}

} // namespace jxx::util::function
