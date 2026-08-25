#include "util/jxx.util.logging.Handler.h"
#include "util/jxx.util.logging.LogRecord.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.logging.ErrorManager.h"
#include "util/jxx.util.logging.Filter.h"
#include "util/jxx.util.logging.Level.h"
#include "util/jxx.util.logging.Formatter.h"
namespace jxx::util::logging
{
	Handler::Handler() :errorManager_(jxx::NEW<ErrorManager>()), level_(Level::ALL())
	{
	}void Handler::setFormatter(const jxx::Ptr<Formatter>& v)
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); if (v == nullptr)throw jxx::lang::NullPointerException(); formatter_ = v;
	}jxx::Ptr<Formatter>Handler::getFormatter()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return formatter_;
	}void Handler::setEncoding(const jxx::Ptr<jxx::lang::String>& v)
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); encoding_ = v;
	}jxx::Ptr<jxx::lang::String>Handler::getEncoding()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return encoding_;
	}void Handler::setFilter(const jxx::Ptr<Filter>& v)
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); filter_ = v;
	}jxx::Ptr<Filter>Handler::getFilter()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return filter_;
	}void Handler::setErrorManager(const jxx::Ptr<ErrorManager>& v)
	{
		if (v == nullptr)throw jxx::lang::NullPointerException(); std::lock_guard<std::recursive_mutex>l(mutex_); errorManager_ = v;
	}jxx::Ptr<ErrorManager>Handler::getErrorManager()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return errorManager_;
	}void Handler::setLevel(const jxx::Ptr<Level>& v)
	{
		if (v == nullptr)throw jxx::lang::NullPointerException(); std::lock_guard<std::recursive_mutex>l(mutex_); level_ = v;
	}jxx::Ptr<Level>Handler::getLevel()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return level_;
	}jxx::lang::jbool Handler::isLoggable(const jxx::Ptr<LogRecord>& r)const
	{
		if (r == nullptr)return false; std::lock_guard<std::recursive_mutex>l(mutex_); return r->getLevel()->intValue() >= level_->intValue() && (filter_ == nullptr || filter_->isLoggable(r));
	}void Handler::reportError(const jxx::Ptr<jxx::lang::String>& m, const jxx::Ptr<jxx::lang::Exception>& e, jxx::lang::jint c)
	{
		getErrorManager()->error(m, e, c);
	}
} // namespace jxx::util::logging
