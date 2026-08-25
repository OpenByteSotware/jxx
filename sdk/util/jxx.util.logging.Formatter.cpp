#include "util/jxx.util.logging.Formatter.h"
#include "util/jxx.util.logging.LogRecord.h"
namespace jxx::util::logging
{
	namespace
	{
		jxx::Ptr<jxx::lang::String> S(const char* v)
		{
			return jxx::NEW<jxx::lang::String>(v);
		}
	}
	jxx::Ptr<jxx::lang::String> Formatter::getHead(const jxx::Ptr<Handler>&)
	{
		return S("");
	}jxx::Ptr<jxx::lang::String> Formatter::getTail(const jxx::Ptr<Handler>&)
	{
		return S("");
	}jxx::Ptr<jxx::lang::String> Formatter::formatMessage(const jxx::Ptr<LogRecord>& r)
	{
		return r->getMessage();
	}
} // namespace jxx::util::logging
