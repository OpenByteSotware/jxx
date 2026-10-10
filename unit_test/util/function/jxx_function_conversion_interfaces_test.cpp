#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.IntFunction.h"
#include "util/function/jxx.util.function.IntToLongFunction.h"
#include "util/function/jxx.util.function.ToIntFunction.h"
#include "util/function/jxx.util.function.ToDoubleBiFunction.h"
#include "util/function/jxx.util.function.ObjLongConsumer.h"

TEST(JxxFunctionConversionInterfaces, MetadataUsesInterfaceBase) {
    EXPECT_TRUE((::jxx::util::function::IntFunction<::jxx::lang::String>::Class()->isInterface()));
    EXPECT_TRUE(::jxx::util::function::IntToLongFunction::Class()->isInterface());
    EXPECT_TRUE((::jxx::util::function::ToIntFunction<::jxx::lang::String>::Class()->isInterface()));
    EXPECT_TRUE((::jxx::util::function::ToDoubleBiFunction<::jxx::lang::String, ::jxx::lang::String>::Class()->isInterface()));
    EXPECT_TRUE((::jxx::util::function::ObjLongConsumer<::jxx::lang::String>::Class()->isInterface()));
}
