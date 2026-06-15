#pragma once
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {
class Number : public Object {
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