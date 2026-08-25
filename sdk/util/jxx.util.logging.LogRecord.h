#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.ResourceBundle.h"
#include "util/jxx.util.logging.Level.h"
namespace jxx::util::logging {
class LogRecord : public jxx::lang::Object {
public:
    explicit LogRecord(const jxx::Ptr<Level>& level, const jxx::Ptr<jxx::lang::String>& message);
    jxx::Ptr<Level> getLevel() const; void setLevel(const jxx::Ptr<Level>& level);
    jxx::Ptr<jxx::lang::String> getLoggerName() const; void setLoggerName(const jxx::Ptr<jxx::lang::String>& name);
    jxx::Ptr<jxx::lang::String> getMessage() const; void setMessage(const jxx::Ptr<jxx::lang::String>& message);
    jxx::lang::jlong getMillis() const noexcept; void setMillis(jxx::lang::jlong millis) noexcept;
    jxx::lang::jlong getSequenceNumber() const noexcept; void setSequenceNumber(jxx::lang::jlong value) noexcept;
    jxx::lang::jint getThreadID() const noexcept; void setThreadID(jxx::lang::jint value) noexcept;
    jxx::Ptr<jxx::lang::Throwable> getThrown() const; void setThrown(const jxx::Ptr<jxx::lang::Throwable>& thrown);
    jxx::Ptr<jxx::lang::String> getSourceClassName() const; void setSourceClassName(const jxx::Ptr<jxx::lang::String>& value);
    jxx::Ptr<jxx::lang::String> getSourceMethodName() const; void setSourceMethodName(const jxx::Ptr<jxx::lang::String>& value);
    jxx::lang::ObjectArray getParameters() const; void setParameters(const jxx::lang::ObjectArray& value);
    jxx::Ptr<jxx::util::ResourceBundle> getResourceBundle() const; void setResourceBundle(const jxx::Ptr<jxx::util::ResourceBundle>& value);
    jxx::Ptr<jxx::lang::String> getResourceBundleName() const; void setResourceBundleName(const jxx::Ptr<jxx::lang::String>& value);
    JXX_OBJECT_CLONE(LogRecord)
   
private:
    jxx::Ptr<Level> level_; jxx::Ptr<jxx::lang::String> loggerName_; jxx::Ptr<jxx::lang::String> message_;
    jxx::lang::jlong millis_; jxx::lang::jlong sequence_; jxx::lang::jint threadId_;
    jxx::Ptr<jxx::lang::Throwable> thrown_; jxx::Ptr<jxx::lang::String> sourceClass_; jxx::Ptr<jxx::lang::String> sourceMethod_;
    jxx::lang::ObjectArray parameters_; jxx::Ptr<jxx::util::ResourceBundle> bundle_; jxx::Ptr<jxx::lang::String> bundleName_;
};
}
