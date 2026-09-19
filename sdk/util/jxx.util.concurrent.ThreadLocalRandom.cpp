#include "util/jxx.util.concurrent.ThreadLocalRandom.h"
#include <chrono>
#include <limits>
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::util::concurrent {
::jxx::Ptr<::jxx::lang::ClassAny> ThreadLocalRandom::Class(){return JxxClassInfoMarker::Class();}
ThreadLocalRandom::ThreadLocalRandom():JxxSuper(static_cast<::jxx::lang::jlong>(std::chrono::high_resolution_clock::now().time_since_epoch().count())){initialized_=true;}
::jxx::Ptr<ThreadLocalRandom> ThreadLocalRandom::current(){thread_local ::jxx::Ptr<ThreadLocalRandom> value(new ThreadLocalRandom());return value;}
void ThreadLocalRandom::setSeed(::jxx::lang::jlong seed){if(initialized_)throw ::jxx::lang::UnsupportedOperationException();JxxSuper::setSeed(seed);}
::jxx::lang::jint ThreadLocalRandom::nextInt(::jxx::lang::jint origin,::jxx::lang::jint bound){if(origin>=bound)throw ::jxx::lang::IllegalArgumentException();return origin+JxxSuper::nextInt(bound-origin);}
::jxx::lang::jlong ThreadLocalRandom::nextLong(::jxx::lang::jlong bound){if(bound<=0)throw ::jxx::lang::IllegalArgumentException();auto r=JxxSuper::nextLong();auto u=static_cast<std::uint64_t>(r);return static_cast<::jxx::lang::jlong>(u%static_cast<std::uint64_t>(bound));}
::jxx::lang::jlong ThreadLocalRandom::nextLong(::jxx::lang::jlong origin,::jxx::lang::jlong bound){if(origin>=bound)throw ::jxx::lang::IllegalArgumentException();return origin+nextLong(bound-origin);}
::jxx::lang::jdouble ThreadLocalRandom::nextDouble(::jxx::lang::jdouble bound){if(!(bound>0.0))throw ::jxx::lang::IllegalArgumentException();return JxxSuper::nextDouble()*bound;}
::jxx::lang::jdouble ThreadLocalRandom::nextDouble(::jxx::lang::jdouble origin,::jxx::lang::jdouble bound){if(!(origin<bound))throw ::jxx::lang::IllegalArgumentException();return origin+(bound-origin)*JxxSuper::nextDouble();}
}
