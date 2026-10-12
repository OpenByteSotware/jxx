#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/function/jxx.util.function.Predicate.h"
namespace
{
	class NonEmpty final : public ::jxx::lang::ClassBase<NonEmpty, ::jxx::lang::Object, ::jxx::util::function::Predicate<::jxx::lang::String>>
	{
	public: ::jxx::lang::jbool test(const ::jxx::Ptr<::jxx::lang::String>& v) override
	{
		return v != nullptr && !v->utf8().empty();
	}
	};
}
TEST(JxxPredicateCompositionParity, CompositionAndEquality)
{
	auto p = ::jxx::CAST<::jxx::util::function::Predicate<::jxx::lang::String>>(::jxx::NEW<NonEmpty>()); 
	auto a = ::jxx::NEW<::jxx::lang::String>("a"); 
	EXPECT_TRUE(p->and_(p)->test(a));
	EXPECT_TRUE(p->or_(p)->test(a)); 
	EXPECT_FALSE(p->negate()->test(a)); 
	auto eq = ::jxx::util::function::Predicate<::jxx::lang::String>::isEqual<::jxx::lang::String>(::jxx::CAST<::jxx::lang::Object>(a)); EXPECT_TRUE(eq->test(a));
}
TEST(JxxPredicateCompositionParity, NullValidation)
{
	auto p = ::jxx::CAST<::jxx::util::function::Predicate<::jxx::lang::String>>(::jxx::NEW<NonEmpty>());
	EXPECT_THROW(p->and_(nullptr), ::jxx::lang::NullPointerException);
	EXPECT_THROW(p->or_(nullptr), ::jxx::lang::NullPointerException);
}
