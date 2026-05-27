#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
class Number : public Object {
public:
    virtual ~Number() override = default;

public:
    virtual jbyte byteValue() const;
    virtual jshort shortValue() const;
    virtual jint intValue() const = 0;
    virtual jlong longValue() const = 0;
    virtual jfloat floatValue() const = 0;
    virtual jdouble doubleValue() const = 0;
};
}