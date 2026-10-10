#include <gtest/gtest.h>
#include "util/function/jxx.util.function.IntUnaryOperator.h"
namespace
{
	class Add final : public ::jxx::lang::ClassBase<Add, ::jxx::lang::Object,
		::jxx::util::function::IntUnaryOperator>
	{
	public: explicit Add(::jxx::lang::jint v) :v_(v)
	{
	} ::jxx::lang::jint applyAsInt(::jxx::lang::jint x) override
	{
		return x + v_;
	} private: ::jxx::lang::jint v_;
	};
}
TEST(JxxPrimitiveUnaryOperatorParity, CompositionOrderAndIdentity)
{
	auto a = ::jxx::CAST<::jxx::util::function::IntUnaryOperator>(::jxx::NEW<Add>(2)); 
	auto b = ::jxx::CAST<::jxx::util::function::IntUnaryOperator>(::jxx::NEW<Add>(3)); 
	EXPECT_EQ(a->compose(b)->applyAsInt(1), 6); 
	EXPECT_EQ(a->andThen(b)->applyAsInt(1), 6); 
	EXPECT_EQ(::jxx::util::function::IntUnaryOperator::identity()->applyAsInt(9), 9);
}
TEST(JxxPrimitiveUnaryOperatorParity, RejectsNull)
{
	auto a = ::jxx::CAST<::jxx::util::function::IntUnaryOperator>(::jxx::NEW<Add>(2)); 
	EXPECT_THROW(a->compose(nullptr), ::jxx::lang::NullPointerException);
	EXPECT_THROW(a->andThen(nullptr), ::jxx::lang::NullPointerException);
}
