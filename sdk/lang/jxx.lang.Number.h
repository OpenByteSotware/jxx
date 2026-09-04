#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {
class Number : public jxx::lang::ClassBase<Number, Object, jxx::io::Serializable> {
public:
    using JavaSuper = Object;
    using Super = jxx::lang::ClassBase<Number, Object, jxx::io::Serializable>;

public:
    virtual ~Number() override = default;

public:
    virtual jxx::lang::jbyte byteValue() const;
    virtual jxx::lang::jshort shortValue() const;
    virtual jxx::lang::jint intValue() const = 0;
    virtual jxx::lang::jlong longValue() const = 0;
    virtual jxx::lang::jfloat floatValue() const = 0;
    virtual jxx::lang::jdouble doubleValue() const = 0;
};
}