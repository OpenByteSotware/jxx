#pragma once
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang
{
	class BootstrapMethodError :
		public LinkageError
	{
	public:
		using LinkageError::LinkageError;
	protected:
		JXX_OBJECT_CLONE(BootstrapMethodError)
			const char* typeName() const noexcept override
		{
			return "BootstrapMethodError";
		}
	};
}
