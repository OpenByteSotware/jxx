#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "util/jxx.util.Random.h"
namespace jxx::util::concurrent {
class ThreadLocalRandom final : public ::jxx::util::Random {
public:
 using JxxSuper=::jxx::util::Random;
 using JxxClassInfoMarker=::jxx::lang::ClassInfo<ThreadLocalRandom,JxxSuper>;
 static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
 static ::jxx::Ptr<ThreadLocalRandom> current();
 void setSeed(::jxx::lang::jlong seed) override;
 ::jxx::lang::jint nextInt(::jxx::lang::jint origin,::jxx::lang::jint bound);
 ::jxx::lang::jlong nextLong(::jxx::lang::jlong bound);
 ::jxx::lang::jlong nextLong(::jxx::lang::jlong origin,::jxx::lang::jlong bound);
 ::jxx::lang::jdouble nextDouble(::jxx::lang::jdouble bound);
 ::jxx::lang::jdouble nextDouble(::jxx::lang::jdouble origin,::jxx::lang::jdouble bound);
private:
 ThreadLocalRandom();
 ::jxx::lang::jbool initialized_=false;
};}
