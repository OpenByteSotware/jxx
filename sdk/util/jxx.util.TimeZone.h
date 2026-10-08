#pragma once

#include "lang/jxx_types.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util {

class Date;

// Java-like TimeZone backed by TZif zoneinfo.
class TimeZone
    : public ::jxx::lang::ClassBase<
          TimeZone,
          ::jxx::lang::Object,
          ::jxx::lang::Cloneable,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        TimeZone, JxxSuper, ::jxx::lang::Cloneable, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    virtual ~TimeZone() = default;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {
        (void)out;
    }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {
        (void)in;
    }
    void readObjectNoData() override {}

    virtual ::jxx::Ptr<::jxx::lang::String> getID() const = 0;

    // Total offset (raw + dst) at instant, in milliseconds.
    virtual ::jxx::lang::jint getOffset(::jxx::lang::jlong epochMillis) const = 0;

    // Abbreviation at instant (e.g., EST/EDT).
    virtual ::jxx::Ptr<::jxx::lang::String> getAbbreviation(::jxx::lang::jlong epochMillis) const = 0;

    virtual ::jxx::lang::jint getRawOffset() const = 0;
    virtual ::jxx::lang::jbool useDaylightTime() const = 0;
    virtual ::jxx::lang::jbool inDaylightTime(const ::jxx::Ptr<Date>& d) const = 0;
    static ::jxx::Ptr<TimeZone> getTimeZone(const ::jxx::Ptr<::jxx::lang::String>& id);
    static ::jxx::Ptr<TimeZone> getDefault();
};

} // namespace jxx::util
