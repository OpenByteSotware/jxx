#include "util/function/jxx.util.function.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.BiConsumer.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.BiPredicate.h"
#include "util/function/jxx.util.function.BinaryOperator.h"
#include "util/function/jxx.util.function.BooleanSupplier.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.DoubleBinaryOperator.h"
#include "util/function/jxx.util.function.DoubleConsumer.h"
#include "util/function/jxx.util.function.DoubleFunction.h"
#include "util/function/jxx.util.function.DoublePredicate.h"
#include "util/function/jxx.util.function.DoubleSupplier.h"
#include "util/function/jxx.util.function.DoubleToIntFunction.h"
#include "util/function/jxx.util.function.DoubleToLongFunction.h"
#include "util/function/jxx.util.function.DoubleUnaryOperator.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.IntBinaryOperator.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/function/jxx.util.function.IntFunction.h"
#include "util/function/jxx.util.function.IntPredicate.h"
#include "util/function/jxx.util.function.IntSupplier.h"
#include "util/function/jxx.util.function.IntToDoubleFunction.h"
#include "util/function/jxx.util.function.IntToLongFunction.h"
#include "util/function/jxx.util.function.IntUnaryOperator.h"
#include "util/function/jxx.util.function.LongBinaryOperator.h"
#include "util/function/jxx.util.function.LongConsumer.h"
#include "util/function/jxx.util.function.LongFunction.h"
#include "util/function/jxx.util.function.LongPredicate.h"
#include "util/function/jxx.util.function.LongSupplier.h"
#include "util/function/jxx.util.function.LongToDoubleFunction.h"
#include "util/function/jxx.util.function.LongToIntFunction.h"
#include "util/function/jxx.util.function.LongUnaryOperator.h"
#include "util/function/jxx.util.function.ObjDoubleConsumer.h"
#include "util/function/jxx.util.function.ObjIntConsumer.h"
#include "util/function/jxx.util.function.ObjLongConsumer.h"
#include "util/function/jxx.util.function.Predicate.h"
#include "util/function/jxx.util.function.Supplier.h"
#include "util/function/jxx.util.function.ToDoubleBiFunction.h"
#include "util/function/jxx.util.function.ToDoubleFunction.h"
#include "util/function/jxx.util.function.ToIntBiFunction.h"
#include "util/function/jxx.util.function.ToIntFunction.h"
#include "util/function/jxx.util.function.ToLongBiFunction.h"
#include "util/function/jxx.util.function.ToLongFunction.h"
#include "util/function/jxx.util.function.UnaryOperator.h"

namespace jxx::util::function {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        using Object = ::jxx::lang::Object;

        (void)BooleanSupplier::Class();
        (void)DoubleBinaryOperator::Class();
        (void)DoubleConsumer::Class();
        (void)DoublePredicate::Class();
        (void)DoubleSupplier::Class();
        (void)DoubleToIntFunction::Class();
        (void)DoubleToLongFunction::Class();
        (void)DoubleUnaryOperator::Class();
        (void)IntBinaryOperator::Class();
        (void)IntConsumer::Class();
        (void)IntPredicate::Class();
        (void)IntSupplier::Class();
        (void)IntToDoubleFunction::Class();
        (void)IntToLongFunction::Class();
        (void)IntUnaryOperator::Class();
        (void)LongBinaryOperator::Class();
        (void)LongConsumer::Class();
        (void)LongPredicate::Class();
        (void)LongSupplier::Class();
        (void)LongToDoubleFunction::Class();
        (void)LongToIntFunction::Class();
        (void)LongUnaryOperator::Class();

        (void)BiConsumer<Object, Object>::Class();
        (void)BiFunction<Object, Object, Object>::Class();
        (void)BiPredicate<Object, Object>::Class();
        (void)BinaryOperator<Object>::Class();
        (void)Consumer<Object>::Class();
        (void)DoubleFunction<Object>::Class();
        (void)Function<Object, Object>::Class();
        (void)IntFunction<Object>::Class();
        (void)LongFunction<Object>::Class();
        (void)ObjDoubleConsumer<Object>::Class();
        (void)ObjIntConsumer<Object>::Class();
        (void)ObjLongConsumer<Object>::Class();
        (void)Predicate<Object>::Class();
        (void)Supplier<Object>::Class();
        (void)ToDoubleBiFunction<Object, Object>::Class();
        (void)ToDoubleFunction<Object>::Class();
        (void)ToIntBiFunction<Object, Object>::Class();
        (void)ToIntFunction<Object>::Class();
        (void)ToLongBiFunction<Object, Object>::Class();
        (void)ToLongFunction<Object>::Class();
        (void)UnaryOperator<Object>::Class();
    });
}

void initialize()
{
    initializeClasses();
}

} // namespace jxx::util::function
