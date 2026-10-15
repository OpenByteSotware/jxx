#pragma once

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::util {

class Date
    : public ::jxx::lang::ClassBase<
          Date,
          ::jxx::lang::Object,
          ::jxx::lang::Cloneable,
          ::jxx::lang::Comparable<Date>,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Date,
        JxxSuper,
        ::jxx::lang::Cloneable,
        ::jxx::lang::Comparable<Date>,
        ::jxx::io::SerializableI>;

    Date();
    explicit Date(::jxx::lang::jlong epochMillis);
    Date(::jxx::lang::jint year, ::jxx::lang::jint month,
         ::jxx::lang::jint date);
    Date(::jxx::lang::jint year, ::jxx::lang::jint month,
         ::jxx::lang::jint date, ::jxx::lang::jint hours,
         ::jxx::lang::jint minutes);
    Date(::jxx::lang::jint year, ::jxx::lang::jint month,
         ::jxx::lang::jint date, ::jxx::lang::jint hours,
         ::jxx::lang::jint minutes, ::jxx::lang::jint seconds);
    explicit Date(const ::jxx::Ptr<::jxx::lang::String>& value);

    static ::jxx::lang::jlong UTC(
        ::jxx::lang::jint year, ::jxx::lang::jint month,
        ::jxx::lang::jint date, ::jxx::lang::jint hours,
        ::jxx::lang::jint minutes, ::jxx::lang::jint seconds);
    static ::jxx::lang::jlong parse(
        const ::jxx::Ptr<::jxx::lang::String>& value);

    ::jxx::lang::jlong getTime() const;
    void setTime(::jxx::lang::jlong epochMillis);

    ::jxx::lang::jint getYear() const;
    void setYear(::jxx::lang::jint year);
    ::jxx::lang::jint getMonth() const;
    void setMonth(::jxx::lang::jint month);
    ::jxx::lang::jint getDate() const;
    void setDate(::jxx::lang::jint date);
    ::jxx::lang::jint getDay() const;
    ::jxx::lang::jint getHours() const;
    void setHours(::jxx::lang::jint hours);
    ::jxx::lang::jint getMinutes() const;
    void setMinutes(::jxx::lang::jint minutes);
    ::jxx::lang::jint getSeconds() const;
    void setSeconds(::jxx::lang::jint seconds);
    ::jxx::Ptr<::jxx::lang::String> toLocaleString() const;
    ::jxx::Ptr<::jxx::lang::String> toGMTString() const;
    ::jxx::lang::jint getTimezoneOffset() const;

    ::jxx::lang::jbool after(const ::jxx::Ptr<Date>& when) const;
    ::jxx::lang::jbool before(const ::jxx::Ptr<Date>& when) const;
    ::jxx::lang::jint compareTo(const ::jxx::Ptr<Date>& anotherDate) const override;

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {
        if (out == nullptr) throw ::jxx::lang::NullPointerException();
        out->writeLong(time_);
    }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {
        if (in == nullptr) throw ::jxx::lang::NullPointerException();
        time_ = in->readLong();
    }
    void readObjectNoData() override { time_ = 0; }

protected:
    JXX_OBJECT_CLONE(Date)

private:
    ::jxx::lang::jlong time_ = 0;
};

} // namespace jxx::util
