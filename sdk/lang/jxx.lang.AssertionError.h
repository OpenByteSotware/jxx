#pragma once
#include "lang/jxx.lang.Error.h"
namespace jxx::lang
{
	class AssertionError : public Error
	{
	public:
		using Error::Error;

	protected:
		JXX_OBJECT_CLONE(AssertionError)
			const char* typeName() const noexcept override
		{
			return "AssertionError";
		}
	};
}
