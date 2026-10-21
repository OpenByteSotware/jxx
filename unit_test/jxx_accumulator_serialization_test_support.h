#pragma once

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.DoubleBinaryOperator.h"
#include "util/function/jxx.util.function.LongBinaryOperator.h"

namespace jxx::unit_test::atomic
{
class SerializableDoubleSumOperator final
    : public ::jxx::lang::ClassBase<SerializableDoubleSumOperator,
          ::jxx::lang::Object,
          ::jxx::util::function::DoubleBinaryOperator,
          ::jxx::io::SerializableI>
{
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SerializableDoubleSumOperator, JxxSuper,
        ::jxx::util::function::DoubleBinaryOperator, ::jxx::io::SerializableI>;
    SerializableDoubleSumOperator() = default;
    ::jxx::lang::jdouble applyAsDouble(::jxx::lang::jdouble left,
        ::jxx::lang::jdouble right) override;
    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;
};

class SerializableLongSumOperator final
    : public ::jxx::lang::ClassBase<SerializableLongSumOperator,
          ::jxx::lang::Object,
          ::jxx::util::function::LongBinaryOperator,
          ::jxx::io::SerializableI>
{
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SerializableLongSumOperator, JxxSuper,
        ::jxx::util::function::LongBinaryOperator, ::jxx::io::SerializableI>;
    SerializableLongSumOperator() = default;
    ::jxx::lang::jlong applyAsLong(::jxx::lang::jlong left,
        ::jxx::lang::jlong right) override;
    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;
};
}
