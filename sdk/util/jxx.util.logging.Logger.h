#pragma once
#include <mutex>
#include <vector>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.ResourceBundle.h"
#include "util/jxx.util.logging.Filter.h"
#include "util/jxx.util.logging.Handler.h"
#include "util/jxx.util.logging.Level.h"
#include "util/jxx.util.logging.LogRecord.h"
namespace jxx::util::logging
{
	class Logger : public jxx::lang::Object
	{
	public:
		static jxx::Ptr<jxx::lang::String> GLOBAL_LOGGER_NAME(); static jxx::Ptr<Logger> global();
		static jxx::Ptr<Logger> getGlobal(); static jxx::Ptr<Logger> getLogger(const jxx::Ptr<jxx::lang::String>& name);
		static jxx::Ptr<Logger> getLogger(const jxx::Ptr<jxx::lang::String>& name, const jxx::Ptr<jxx::lang::String>& resourceBundleName);
		static jxx::Ptr<Logger> getAnonymousLogger(); static jxx::Ptr<Logger> getAnonymousLogger(const jxx::Ptr<jxx::lang::String>& resourceBundleName);
		virtual ~Logger() = default;
		virtual void log(const jxx::Ptr<LogRecord>& record);
		void log(const jxx::Ptr<Level>& level, const jxx::Ptr<jxx::lang::String>& message);
		void log(const jxx::Ptr<Level>& level, const jxx::Ptr<jxx::lang::String>& message, const jxx::Ptr<jxx::lang::Object>& parameter);
		void log(const jxx::Ptr<Level>& level, const jxx::Ptr<jxx::lang::String>& message, const jxx::lang::ObjectArray& parameters);
		void log(const jxx::Ptr<Level>& level, const jxx::Ptr<jxx::lang::String>& message, const jxx::Ptr<jxx::lang::Throwable>& thrown);
		void logp(const jxx::Ptr<Level>& level, const jxx::Ptr<jxx::lang::String>& sourceClass, const jxx::Ptr<jxx::lang::String>& sourceMethod, const jxx::Ptr<jxx::lang::String>& message);
		void entering(const jxx::Ptr<jxx::lang::String>& sourceClass, const jxx::Ptr<jxx::lang::String>& sourceMethod);
		void exiting(const jxx::Ptr<jxx::lang::String>& sourceClass, const jxx::Ptr<jxx::lang::String>& sourceMethod);
		void throwing(const jxx::Ptr<jxx::lang::String>& sourceClass, const jxx::Ptr<jxx::lang::String>& sourceMethod, const jxx::Ptr<jxx::lang::Throwable>& thrown);
		void severe(const jxx::Ptr<jxx::lang::String>& message); void warning(const jxx::Ptr<jxx::lang::String>& message); void info(const jxx::Ptr<jxx::lang::String>& message); void config(const jxx::Ptr<jxx::lang::String>& message); void fine(const jxx::Ptr<jxx::lang::String>& message); void finer(const jxx::Ptr<jxx::lang::String>& message); void finest(const jxx::Ptr<jxx::lang::String>& message);
		jxx::lang::jbool isLoggable(const jxx::Ptr<Level>& level) const;
		void addHandler(const jxx::Ptr<Handler>& handler); void removeHandler(const jxx::Ptr<Handler>& handler); jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<Handler>, 1U>> getHandlers() const;
		jxx::Ptr<Filter> getFilter() const; void setFilter(const jxx::Ptr<Filter>& filter); jxx::Ptr<Level> getLevel() const; void setLevel(const jxx::Ptr<Level>& level);
		jxx::Ptr<jxx::lang::String> getName() const; jxx::Ptr<Logger> getParent() const; void setParent(const jxx::Ptr<Logger>& parent);
		jxx::lang::jbool getUseParentHandlers() const; void setUseParentHandlers(jxx::lang::jbool value);
		jxx::Ptr<jxx::util::ResourceBundle> getResourceBundle() const; jxx::Ptr<jxx::lang::String> getResourceBundleName() const; void setResourceBundle(const jxx::Ptr<jxx::util::ResourceBundle>& bundle);
		
	protected: 
		
		Logger(const jxx::Ptr<jxx::lang::String>& name, const jxx::Ptr<jxx::lang::String>& resourceBundleName);

	private: jxx::Ptr<Level> effectiveLevel_() const; mutable std::recursive_mutex mutex_; jxx::Ptr<jxx::lang::String> name_; jxx::Ptr<jxx::lang::String> bundleName_; jxx::Ptr<jxx::util::ResourceBundle> bundle_; jxx::Ptr<Level> level_; jxx::Ptr<Filter> filter_; std::vector<jxx::Ptr<Handler>> handlers_; jxx::Ptr<Logger> parent_; jxx::lang::jbool useParentHandlers_;
	};
}
