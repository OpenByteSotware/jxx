#include <gtest/gtest.h>
#include <type_traits>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Errors.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"

namespace
{
	using namespace jxx::lang;
	TEST(ErrorHierarchyTest, ErrorExtendsThrowable)
	{
		static_assert(std::is_base_of_v<Throwable, Error>); 
		auto e = jxx::NEW<Error>();
		EXPECT_EQ(e->getMessage(), nullptr);
	}
	
	TEST(ErrorHierarchyTest, MessageConstructorPreservesMessage)
	{
		auto m = jxx::NEW<String>("failure"); 
		auto e = jxx::NEW<Error>(m);
		ASSERT_NE(e->getMessage(), nullptr);
		EXPECT_EQ(e->what(), "failure");
	}
	
	TEST(ErrorHierarchyTest, CauseConstructorPreservesCause)
	{
		auto cause = jxx::NEW<Error>(jxx::NEW<String>("cause")); 
		auto e = jxx::NEW<Error>(jxx::CAST<Throwable>(cause)); 
		EXPECT_EQ(e->getCause(), jxx::CAST<Throwable>(cause));
	}
	
	TEST(ErrorHierarchyTest, AssertionErrorFormatsPrimitiveDetail)
	{
		auto e = jxx::NEW<AssertionError>(jxx::NEW<String>("42"));
		ASSERT_NE(e->getMessage(), nullptr);
		EXPECT_EQ(e->what(), "42");
	}
	/*
	TEST(ErrorHierarchyTest, ExceptionInInitializerRetainsThrownException)
	{
		auto cause = jxx::NEW<Error>(jxx::NEW<String>("init")); 
		auto e = jxx::NEW<ExceptionInInitializerError>(jxx::CAST<Throwable>(cause)); 
		EXPECT_EQ(e->what(), jxx::CAST<Throwable>(cause));
	}*/

} // namespace
