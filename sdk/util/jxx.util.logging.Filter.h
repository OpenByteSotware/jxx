#pragma once
#include "lang/jxx_types.h"
namespace jxx::util::logging
{
	class LogRecord; class Filter
	{
	public: virtual ~Filter() = default; 
		  virtual jxx::lang::jbool isLoggable(const jxx::Ptr<LogRecord>& record) = 0;
	};
}
