#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::logging
{
	class LogRecord; class Filter : public ::jxx::lang::InterfaceBase<Filter> {
	public: virtual ~Filter() = default; 
		  virtual jxx::lang::jbool isLoggable(const jxx::Ptr<LogRecord>& record) = 0;
	};
}
