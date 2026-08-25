#pragma once
#include "util/jxx.util.logging.Formatter.h"
namespace jxx::util::logging
{
	class SimpleFormatter : public Formatter
	{
	public: jxx::Ptr<jxx::lang::String> format(const jxx::Ptr<LogRecord>& record) override; 
		  JXX_OBJECT_CLONE(SimpleFormatter)

	};
}
