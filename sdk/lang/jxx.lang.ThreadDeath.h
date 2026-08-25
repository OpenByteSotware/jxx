#pragma once
#include "lang/jxx.lang.Error.h"
namespace jxx::lang
{
	class ThreadDeath : public Error
	{
		using Error::Error;
	protected:
		JXX_OBJECT_CLONE(ThreadDeath)
			const char* typeName() const noexcept override
		{
			return "ThreadDeath";
		}
	};
}
