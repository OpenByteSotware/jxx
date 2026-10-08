#include <gtest/gtest.h>
#include <type_traits>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Errors.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"

namespace
{
	using namespace jxx::lang;
	TEST(ErrorHierarchyTest, AssertionErrorFormatsPrimitiveDetail)
	{
		const auto error =
			jxx::NEW<AssertionError>(
				static_cast<jint>(42));

		ASSERT_NE(nullptr, error->getMessage());
		EXPECT_STREQ("42", error->what());
	}

	TEST(ErrorHierarchyTest, MessageConstructorPreservesMessage)
	{
		const auto message = jxx::NEW<String>("failure");
		const auto error = jxx::NEW<Error>(message);

		ASSERT_NE(nullptr, error->getMessage());
		EXPECT_STREQ("failure", error->what());
	}

	TEST(ErrorHierarchyTest, AssertionErrorPreservesStringDetail)
	{
		const auto error =
			jxx::NEW<AssertionError>(
				jxx::NEW<String>("42"));

		ASSERT_NE(nullptr, error->getMessage());
		EXPECT_STREQ("42", error->what());
	}
} // namespace
