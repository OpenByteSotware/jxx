#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::io { class InputStream; class OutputStream; }
namespace jxx::util::concurrent { class TimeUnit; }
namespace jxx::lang {

class Process : public ClassBase<Process, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<Process, JxxSuper>;
    ~Process() override = default;

    virtual jxx::Ptr<jxx::io::OutputStream> getOutputStream() = 0;
    virtual jxx::Ptr<jxx::io::InputStream> getInputStream() = 0;
    virtual jxx::Ptr<jxx::io::InputStream> getErrorStream() = 0;
    virtual jint waitFor() = 0;
    virtual jbool waitFor(jlong timeout,
        const jxx::Ptr<jxx::util::concurrent::TimeUnit>& unit);
    virtual jint exitValue() = 0;
    virtual void destroy() = 0;
    virtual jxx::Ptr<Process> destroyForcibly();
    virtual jbool isAlive();

protected:
    Process() = default;
};
} // namespace jxx::lang
