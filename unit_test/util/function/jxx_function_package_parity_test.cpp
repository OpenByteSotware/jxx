#include <gtest/gtest.h>
#include <type_traits>

#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.BinaryOperator.h"
#include "util/function/jxx.util.function.ClassInitialization.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.IntPredicate.h"
#include "util/function/jxx.util.function.Predicate.h"
#include "util/function/jxx.util.function.UnaryOperator.h"
#include "jxx.initialize.h"

namespace function = ::jxx::util::function;
using Object = ::jxx::lang::Object;

TEST(FunctionPackageInheritanceParity, OperatorInterfacesExtendFunctionInterfaces) {
    static_assert(std::is_base_of_v<
        function::Function<Object, Object>,
        function::UnaryOperator<Object>>);
    static_assert(std::is_base_of_v<
        function::BiFunction<Object, Object, Object>,
        function::BinaryOperator<Object>>);
    SUCCEED();
}

TEST(FunctionPackageInheritanceParity, PublicFunctionalTypesRemainInterfaces) {
    static_assert(std::is_abstract_v<function::Function<Object, Object>>);
    static_assert(std::is_abstract_v<function::Predicate<Object>>);
    static_assert(std::is_abstract_v<function::IntPredicate>);
    SUCCEED();
}

TEST(FunctionPackageInitializationParity, RegistersPrimitiveAndErasedGenericInterfaces) {
    jxx::initialize();
    EXPECT_NE(nullptr, function::IntPredicate::Class());    
    EXPECT_NE(nullptr, function::Predicate<Object>::Class());
    EXPECT_NE(nullptr, function::UnaryOperator<Object>::Class());
    EXPECT_NE(nullptr, function::BinaryOperator<Object>::Class());
}
