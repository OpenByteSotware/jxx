#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "util/function/jxx.util.function.BooleanSupplier.h"
#include "util/function/jxx.util.function.DoubleConsumer.h"
#include "util/function/jxx.util.function.DoublePredicate.h"
#include "util/function/jxx.util.function.DoubleUnaryOperator.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/function/jxx.util.function.IntPredicate.h"
#include "util/function/jxx.util.function.IntUnaryOperator.h"
#include "util/function/jxx.util.function.LongConsumer.h"
#include "util/function/jxx.util.function.LongPredicate.h"
#include "util/function/jxx.util.function.LongUnaryOperator.h"

TEST(JxxFunctionRootClassInitialization, ExistingPackageInitializerIsInvoked)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::util::function::BooleanSupplier::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::DoubleConsumer::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::DoublePredicate::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::DoubleUnaryOperator::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::IntConsumer::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::IntPredicate::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::IntUnaryOperator::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::LongConsumer::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::LongPredicate::Class(), nullptr);
    EXPECT_NE(::jxx::util::function::LongUnaryOperator::Class(), nullptr);
}
