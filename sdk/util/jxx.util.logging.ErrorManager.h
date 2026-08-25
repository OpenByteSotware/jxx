#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::logging
{
	class ErrorManager : public jxx::lang::Object
	{
	public:
		static constexpr jxx::lang::jint GENERIC_FAILURE = 0,
			WRITE_FAILURE = 1,
			FLUSH_FAILURE = 2, 
			CLOSE_FAILURE = 3, 
			OPEN_FAILURE = 4, 
			FORMAT_FAILURE = 5;
		virtual void error(const jxx::Ptr<jxx::lang::String>& message,
			const jxx::Ptr<jxx::lang::Exception>& exception, jxx::lang::jint code);
	protected:
		JXX_OBJECT_CLONE(ErrorManager)
	};
}
