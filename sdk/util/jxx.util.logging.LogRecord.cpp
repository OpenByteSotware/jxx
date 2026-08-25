#include "util/jxx.util.logging.LogRecord.h"
#include <atomic>
#include <chrono>
#include <thread>
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::util::logging
{
	namespace
	{
		std::atomic<jxx::lang::jlong> sequence{ 0 };
	}
	LogRecord::LogRecord(const jxx::Ptr<Level>& l, const jxx::Ptr<jxx::lang::String>& m) :level_(l), message_(m), millis_(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()), sequence_(sequence++), threadId_(static_cast<jxx::lang::jint>(std::hash<std::thread::id>{}(std::this_thread::get_id()))), parameters_(nullptr)
	{
		if (l == nullptr)throw jxx::lang::NullPointerException();
	}
#define GETSET(T,N,F) T LogRecord::get##N()const{return F;} void LogRecord::set##N(const T& v){F=v;}
	GETSET(jxx::Ptr<Level>, Level, level_) GETSET(jxx::Ptr<jxx::lang::String>, LoggerName, loggerName_) GETSET(jxx::Ptr<jxx::lang::String>, Message, message_) GETSET(jxx::Ptr<jxx::lang::Throwable>, Thrown, thrown_) GETSET(jxx::Ptr<jxx::lang::String>, SourceClassName, sourceClass_) GETSET(jxx::Ptr<jxx::lang::String>, SourceMethodName, sourceMethod_) GETSET(jxx::Ptr<jxx::util::ResourceBundle>, ResourceBundle, bundle_) GETSET(jxx::Ptr<jxx::lang::String>, ResourceBundleName, bundleName_)
#undef GETSET
		jxx::lang::jlong LogRecord::getMillis()const noexcept
	{
		return millis_;
	}void LogRecord::setMillis(jxx::lang::jlong v)noexcept
	{
		millis_ = v;
	}jxx::lang::jlong LogRecord::getSequenceNumber()const noexcept
	{
		return sequence_;
	}void LogRecord::setSequenceNumber(jxx::lang::jlong v)noexcept
	{
		sequence_ = v;
	}jxx::lang::jint LogRecord::getThreadID()const noexcept
	{
		return threadId_;
	}void LogRecord::setThreadID(jxx::lang::jint v)noexcept
	{
		threadId_ = v;
	}jxx::lang::ObjectArray LogRecord::getParameters()const
	{
		return parameters_;
	}void LogRecord::setParameters(const jxx::lang::ObjectArray& v)
	{
		parameters_ = v;
	}
} // namespace jxx::util::logging
