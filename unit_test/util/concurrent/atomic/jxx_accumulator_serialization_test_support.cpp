#include "../../../jxx_accumulator_serialization_test_support.h"
#include "io/jxx.io.InvalidObjectException.h"

namespace jxx::unit_test::atomic
{
::jxx::lang::jdouble SerializableDoubleSumOperator::applyAsDouble(
    ::jxx::lang::jdouble left, ::jxx::lang::jdouble right)
{ return left + right; }
void SerializableDoubleSumOperator::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) {}
void SerializableDoubleSumOperator::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) {}
void SerializableDoubleSumOperator::readObjectNoData() {}

::jxx::lang::jlong SerializableLongSumOperator::applyAsLong(
    ::jxx::lang::jlong left, ::jxx::lang::jlong right)
{ return left + right; }
void SerializableLongSumOperator::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&) {}
void SerializableLongSumOperator::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>&) {}
void SerializableLongSumOperator::readObjectNoData() {}
}
