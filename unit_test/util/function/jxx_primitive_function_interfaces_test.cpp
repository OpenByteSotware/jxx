#include <gtest/gtest.h>
#include "util/function/jxx.util.function.BooleanSupplier.h"
#include "util/function/jxx.util.function.IntPredicate.h"
#include "util/function/jxx.util.function.LongPredicate.h"
#include "util/function/jxx.util.function.DoublePredicate.h"
#include "util/function/jxx.util.function.IntUnaryOperator.h"
#include "util/function/jxx.util.function.LongUnaryOperator.h"
#include "util/function/jxx.util.function.DoubleUnaryOperator.h"
#include "util/function/jxx.util.function.IntBinaryOperator.h"
TEST(JxxPrimitiveFunctionInterfaces, MetadataUsesInterfaceBase) {
    EXPECT_TRUE(::jxx::util::function::BooleanSupplier::Class()->isInterface());
    EXPECT_TRUE(::jxx::util::function::IntPredicate::Class()->isInterface());
    EXPECT_TRUE(::jxx::util::function::IntUnaryOperator::Class()->isInterface());
    EXPECT_TRUE(::jxx::util::function::IntBinaryOperator::Class()->isInterface());
}
