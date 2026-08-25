#include "util/jxx.util.logging.SimpleFormatter.h"
#include "util/jxx.util.logging.LogRecord.h"
#include "util/jxx.util.logging.Level.h"
#include <string>
namespace jxx::util::logging
{
	jxx::Ptr<jxx::lang::String> SimpleFormatter::format(const jxx::Ptr<LogRecord>& r)
	{
		std::string s = std::to_string(r->getMillis()) + " " + r->getLevel()->getName()->utf8() + " "; if (r->getLoggerName())s += r->getLoggerName()->utf8() + " "; if (r->getMessage())s += r->getMessage()->utf8(); s += '\n'; return jxx::NEW<jxx::lang::String>(s);
	}
} // namespace jxx::util::logging
