#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.SerializableI.h"

namespace jxx::lang {
class Number : public jxx::lang::ClassBase<Number, Object, jxx::io::SerializableI> {
public:
    using JxxSuper = Object;
    using Super = jxx::lang::ClassBase<Number, JxxSuper, jxx::io::SerializableI>;

public:
    virtual ~Number() override = default;

public:
    virtual jxx::lang::jbyte byteValue() const;
    virtual jxx::lang::jshort shortValue() const;
    virtual jxx::lang::jint intValue() const = 0;
    virtual jxx::lang::jlong longValue() const = 0;
    virtual jxx::lang::jfloat floatValue() const = 0;
    virtual jxx::lang::jdouble doubleValue() const = 0;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override { (void)out; }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override { (void)in; }
    void readObjectNoData() override {}
};
}