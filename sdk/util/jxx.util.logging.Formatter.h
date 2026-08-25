#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
namespace jxx::util::logging
{
	class Handler; class LogRecord;
	class Formatter : public jxx::lang::Object
	{
	public: virtual ~Formatter() = default;
		  virtual jxx::Ptr<jxx::lang::String> format(const jxx::Ptr<LogRecord>& record) = 0; 
		  virtual jxx::Ptr<jxx::lang::String> getHead(const jxx::Ptr<Handler>& handler); 
		  virtual jxx::Ptr<jxx::lang::String> getTail(const jxx::Ptr<Handler>& handler); 
		  virtual jxx::Ptr<jxx::lang::String> formatMessage(const jxx::Ptr<LogRecord>& record);
	};
}
