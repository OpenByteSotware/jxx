#pragma once
#include <mutex>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Number.h"
namespace jxx::lang { class String; }
namespace jxx::util::concurrent::atomic {
class DoubleAdder final : public ::jxx::lang::ClassBase<DoubleAdder,::jxx::lang::Number,::jxx::io::SerializableI> {
public: using JxxSuper=::jxx::lang::Number; using Super=::jxx::lang::ClassBase<DoubleAdder,JxxSuper,::jxx::io::SerializableI>; using JxxClassInfoMarker=::jxx::lang::ClassInfo<DoubleAdder,JxxSuper,::jxx::io::SerializableI>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class(); DoubleAdder(); void add(::jxx::lang::jdouble); ::jxx::lang::jdouble sum()const; void reset(); ::jxx::lang::jdouble sumThenReset(); ::jxx::lang::jint intValue()const override; ::jxx::lang::jlong longValue()const override; ::jxx::lang::jfloat floatValue()const override; ::jxx::lang::jdouble doubleValue()const override; ::jxx::Ptr<::jxx::lang::String>toString()const override; void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&)override;void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&)override;void readObjectNoData()override;
private:mutable std::mutex mutex_;::jxx::lang::jdouble value_=0;};}
