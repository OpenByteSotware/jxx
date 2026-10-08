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

    ::jxx::lang::jlong getTime() const;
    void setTime(::jxx::lang::jlong epochMillis);

    ::jxx::lang::jbool after(const ::jxx::Ptr<Date>& when) const;
    ::jxx::lang::jbool before(const ::jxx::Ptr<Date>& when) const;
    ::jxx::lang::jint compareTo(const ::jxx::Ptr<Date>& anotherDate) const override;

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
    ::jxx::lang::jint hashCode() const override;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {
        (void)out;
    }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {
        (void)in;
    }
    void readObjectNoData() override {}

protected:
    JXX_OBJECT_CLONE(Date)

private:
    ::jxx::lang::jlong time_ = 0;
};

} // namespace jxx::util
