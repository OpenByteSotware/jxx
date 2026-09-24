#pragma once
#include <mutex>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Number.h"
#include "util/function/jxx.util.function.LongBinaryOperator.h"
namespace jxx::lang { class String; }
namespace jxx::util::concurrent::atomic {
class LongAccumulator final:public ::jxx::lang::ClassBase<LongAccumulator,::jxx::lang::Number,::jxx::io::SerializableI>{
public:using JxxSuper=::jxx::lang::Number;using Super=::jxx::lang::ClassBase<LongAccumulator,JxxSuper,::jxx::io::SerializableI>;using JxxClassInfoMarker=::jxx::lang::ClassInfo<LongAccumulator,JxxSuper,::jxx::io::SerializableI>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 LongAccumulator(const ::jxx::Ptr<::jxx::util::function::LongBinaryOperator>& function,::jxx::lang::jlong identity);
 void accumulate(::jxx::lang::jlong value);::jxx::lang::jlong get()const;void reset();::jxx::lang::jlong getThenReset();
 ::jxx::lang::jint intValue()const override;::jxx::lang::jlong longValue()const override;::jxx::lang::jfloat floatValue()const override;::jxx::lang::jdouble doubleValue()const override;::jxx::Ptr<::jxx::lang::String>toString()const override;
 void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)override;void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)override;void readObjectNoData()override;
private:mutable std::mutex mutex_;::jxx::Ptr<::jxx::util::function::LongBinaryOperator> function_;::jxx::lang::jlong identity_;::jxx::lang::jlong value_;};
}
