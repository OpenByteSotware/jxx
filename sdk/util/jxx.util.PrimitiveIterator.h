#pragma once

#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/function/jxx.util.function.DoubleConsumer.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/function/jxx.util.function.LongConsumer.h"
#include "util/jxx.util.Iterator.h"

namespace jxx::util {

template <typename T, typename TConsumer>
class PrimitiveIterator : public virtual Iterator<T> {
public:
    ~PrimitiveIterator() override = default;

    virtual void forEachRemaining(
        const ::jxx::Ptr<TConsumer>& action) = 0;
};

class PrimitiveIteratorOfInt
    : public virtual PrimitiveIterator<
          ::jxx::lang::Integer,
          ::jxx::util::function::IntConsumer> {
public:
    ~PrimitiveIteratorOfInt() override = default;
    virtual ::jxx::lang::jint nextInt() = 0;

    ::jxx::Ptr<::jxx::lang::Integer> next() override {
        return ::jxx::lang::Integer::valueOf(nextInt());
    }

    void forEachRemaining(
        const ::jxx::Ptr<::jxx::util::function::IntConsumer>& action) override {
        if (action == nullptr) throw ::jxx::lang::NullPointerException();
        while (hasNext()) action->accept(nextInt());
    }
};

class PrimitiveIteratorOfLong
    : public virtual PrimitiveIterator<
          ::jxx::lang::Long,
          ::jxx::util::function::LongConsumer> {
public:
    ~PrimitiveIteratorOfLong() override = default;
    virtual ::jxx::lang::jlong nextLong() = 0;

    ::jxx::Ptr<::jxx::lang::Long> next() override {
        return ::jxx::lang::Long::valueOf(nextLong());
    }

    void forEachRemaining(
        const ::jxx::Ptr<::jxx::util::function::LongConsumer>& action) override {
        if (action == nullptr) throw ::jxx::lang::NullPointerException();
        while (hasNext()) action->accept(nextLong());
    }
};

class PrimitiveIteratorOfDouble
    : public virtual PrimitiveIterator<
          ::jxx::lang::Double,
          ::jxx::util::function::DoubleConsumer> {
public:
    ~PrimitiveIteratorOfDouble() override = default;
    virtual ::jxx::lang::jdouble nextDouble() = 0;

    ::jxx::Ptr<::jxx::lang::Double> next() override {
        return ::jxx::lang::Double::valueOf(nextDouble());
    }

    void forEachRemaining(
        const ::jxx::Ptr<::jxx::util::function::DoubleConsumer>& action) override {
        if (action == nullptr) throw ::jxx::lang::NullPointerException();
        while (hasNext()) action->accept(nextDouble());
    }
};

} // namespace jxx::util
