#include <algorithm>
#include <unordered_map>
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.logging.Logger.h"
namespace jxx::util::logging
{
	namespace
	{
		std::recursive_mutex registryMutex; std::unordered_map<std::string, std::weak_ptr<Logger>> registry; jxx::Ptr<jxx::lang::String> S(const char* v)
		{
			return jxx::NEW<jxx::lang::String>(v);
		}
	}

	Logger::Logger(const jxx::Ptr<jxx::lang::String>& n,
		const jxx::Ptr<jxx::lang::String>& b) :name_(n), bundleName_(b), level_(nullptr), useParentHandlers_(true)
	{
	}
	
	jxx::Ptr<jxx::lang::String>Logger::GLOBAL_LOGGER_NAME()
	{
		return S("global");
	}
	
	jxx::Ptr<Logger>Logger::global()
	{
		return getGlobal();
	}
	
	jxx::Ptr<Logger>Logger::getGlobal()
	{
		return getLogger(GLOBAL_LOGGER_NAME());
	}
	
	jxx::Ptr<Logger> Logger::getLogger(const jxx::Ptr<jxx::lang::String>& n)
	{
		return getLogger(n, nullptr);
	}
	
	jxx::Ptr<Logger>Logger::getLogger(const jxx::Ptr<jxx::lang::String>& n, const jxx::Ptr<jxx::lang::String>& b)
	{
		if (n == nullptr)throw jxx::lang::NullPointerException(); std::lock_guard<std::recursive_mutex>l(registryMutex); auto key = n->utf8(); if (auto p = registry[key].lock())return p; auto x = jxx::Ptr<Logger>(new Logger(n, b)); x->thisPtr() = x; registry[key] = x; return x;
	}
	
	jxx::Ptr<Logger>Logger::getAnonymousLogger()
	{
		return getAnonymousLogger(nullptr);
	}
	
	jxx::Ptr<Logger>Logger::getAnonymousLogger(const jxx::Ptr<jxx::lang::String>& b)
	{
		auto x = jxx::Ptr<Logger>(new Logger(nullptr, b)); x->thisPtr() = x; return x;
	}
	jxx::Ptr<Level>Logger::effectiveLevel_()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); if (level_)return level_; return parent_ ? parent_->effectiveLevel_() : Level::INFO();
	}
	jxx::lang::jbool Logger::isLoggable(const jxx::Ptr<Level>& l)const
	{
		if (l == nullptr)throw jxx::lang::NullPointerException(); return l->intValue() >= effectiveLevel_()->intValue() && l->intValue() != Level::OFF()->intValue();
	}
	void Logger::log(const jxx::Ptr<LogRecord>& r)
	{
		if (r == nullptr || !isLoggable(r->getLevel()))return; std::vector<jxx::Ptr<Handler>> hs; jxx::Ptr<Logger>p;
		{
			std::lock_guard<std::recursive_mutex>l(mutex_); if (filter_ && !filter_->isLoggable(r))return; r->setLoggerName(name_); hs = handlers_; p = useParentHandlers_ ? parent_ : nullptr;
		}for (auto& h : hs)if (h && h->isLoggable(r))h->publish(r); if (p)p->log(r);
	}
	void Logger::log(const jxx::Ptr<Level>& l, const jxx::Ptr<jxx::lang::String>& m)
	{
		if (isLoggable(l))log(jxx::NEW<LogRecord>(l, m));
	}
	void Logger::log(const jxx::Ptr<Level>& l, const jxx::Ptr<jxx::lang::String>& m, const jxx::Ptr<jxx::lang::Object>& p)
	{
		auto r = jxx::NEW<LogRecord>(l, m); auto a = jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>>(1); (*a)[0] = p; r->setParameters(a); log(r);
	}
	void Logger::log(const jxx::Ptr<Level>& l, const jxx::Ptr<jxx::lang::String>& m, const jxx::lang::ObjectArray& p)
	{
		auto r = jxx::NEW<LogRecord>(l, m); r->setParameters(p); log(r);
	}
	void Logger::log(const jxx::Ptr<Level>& l, const jxx::Ptr<jxx::lang::String>& m, const jxx::Ptr<jxx::lang::Throwable>& t)
	{
		auto r = jxx::NEW<LogRecord>(l, m); r->setThrown(t); log(r);
	}
	void Logger::logp(const jxx::Ptr<Level>& l, const jxx::Ptr<jxx::lang::String>& c, const jxx::Ptr<jxx::lang::String>& m, const jxx::Ptr<jxx::lang::String>& s)
	{
		auto r = jxx::NEW<LogRecord>(l, s); r->setSourceClassName(c); r->setSourceMethodName(m); log(r);
	}
	void Logger::entering(const jxx::Ptr<jxx::lang::String>& c, const jxx::Ptr<jxx::lang::String>& m)
	{
		logp(Level::FINER(), c, m, S("ENTRY"));
	}
	void Logger::exiting(const jxx::Ptr<jxx::lang::String>& c, const jxx::Ptr<jxx::lang::String>& m)
	{
		logp(Level::FINER(), c, m, S("RETURN"));
	}
	void Logger::throwing(const jxx::Ptr<jxx::lang::String>& c, const jxx::Ptr<jxx::lang::String>& m, const jxx::Ptr<jxx::lang::Throwable>& t)
	{
		auto r = jxx::NEW<LogRecord>(Level::FINER(), S("THROW")); r->setSourceClassName(c); r->setSourceMethodName(m); r->setThrown(t); log(r);
	}

#define CONV(N,L) void Logger::N(const jxx::Ptr<jxx::lang::String>&m){log(Level::L(),m);}
	CONV(severe, SEVERE) CONV(warning, WARNING) CONV(info, INFO) CONV(config, CONFIG) CONV(fine, FINE) CONV(finer, FINER) CONV(finest, FINEST)
#undef CONV
	
	void Logger::addHandler(const jxx::Ptr<Handler>& h)
	{
		if (h == nullptr)throw jxx::lang::NullPointerException();
		std::lock_guard<std::recursive_mutex>l(mutex_); handlers_.push_back(h);
	}
	
	void Logger::removeHandler(const jxx::Ptr<Handler>& h)
	{
		std::lock_guard<std::recursive_mutex>l(mutex_);
		handlers_.erase(std::remove(handlers_.begin(), handlers_.end(), h), handlers_.end());
	}
	
	jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<Handler>, 1U>>Logger::getHandlers()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_);
		auto a = jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<Handler>, 1U>>(handlers_.size());
		for (std::size_t i = 0; i < handlers_.size(); ++i)(*a)[i] = handlers_[i]; return a;
	}
	
	jxx::Ptr<Filter>Logger::getFilter()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return filter_;
	}void Logger::setFilter(const jxx::Ptr<Filter>& f)
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); filter_ = f;
	}
	
	jxx::Ptr<Level>Logger::getLevel()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return level_;
	}
	
	void Logger::setLevel(const jxx::Ptr<Level>& v)
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); level_ = v;
	}
	
	jxx::Ptr<jxx::lang::String>Logger::getName()const
	{
		return name_;
	}
	
	jxx::Ptr<Logger>Logger::getParent()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return parent_;
	}
	void Logger::setParent(const jxx::Ptr<Logger>& p)
	{
		if (p == nullptr)throw jxx::lang::NullPointerException(); std::lock_guard<std::recursive_mutex>l(mutex_); parent_ = p;
	}
	
	jxx::lang::jbool Logger::getUseParentHandlers()const
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); return useParentHandlers_;
	}
	void Logger::setUseParentHandlers(jxx::lang::jbool v)
	{
		std::lock_guard<std::recursive_mutex>l(mutex_); useParentHandlers_ = v;
	}
	
	jxx::Ptr<jxx::util::ResourceBundle>Logger::getResourceBundle()const
	{
		return bundle_;
	}
	
	jxx::Ptr<jxx::lang::String>Logger::getResourceBundleName()const
	{
		return bundleName_;
	}
	
	void Logger::setResourceBundle(const jxx::Ptr<jxx::util::ResourceBundle>& b)
	{
		if (b == nullptr)throw jxx::lang::NullPointerException(); bundle_ = b;
	}
} // namespace jxx::util::logging
