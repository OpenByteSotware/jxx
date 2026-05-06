#pragma once
#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
namespace jxx::util {
class TimeZone : public jxx::lang::Object {
public:
    TimeZone() = default;
    explicit TimeZone(jxx::Ptr<jxx::lang::String> id, jint rawOffsetMillis)
        : id_(std::move(id)), rawOffsetMillis_(rawOffsetMillis) {}
    virtual ~TimeZone() = default;
    virtual jint getRawOffset() const { return rawOffsetMillis_; }
    virtual void setRawOffset(jint ms) { rawOffsetMillis_ = ms; }
    virtual jint getOffset(jlong /*epochMillis*/) const { return rawOffsetMillis_; }
    virtual jxx::Ptr<jxx::lang::String> getID() const { return id_ ? id_ : std::make_shared<jxx::lang::String>("GMT"); }
    virtual void setID(jxx::Ptr<jxx::lang::String> id) { id_ = std::move(id); }
    static jxx::Ptr<TimeZone> getTimeZone(jxx::Ptr<jxx::lang::String> id);
    static jxx::Ptr<TimeZone> getDefault();
protected:
    jxx::Ptr<jxx::lang::String> id_{};
    jint rawOffsetMillis_ = 0;
};
}