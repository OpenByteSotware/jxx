#include "util/jxx.util.SplittableRandom.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include <atomic>
#include <chrono>
#include <cmath>
#include <limits>
namespace jxx::util {
namespace { constexpr std::uint64_t GOLDEN_GAMMA=0x9e3779b97f4a7c15ULL; std::atomic<std::uint64_t> seedSource{0x6a09e667f3bcc909ULL}; }
SplittableRandom::SplittableRandom():seed_(mix64_(defaultSeed_())),gamma_(GOLDEN_GAMMA){}
SplittableRandom::SplittableRandom(::jxx::lang::jlong seed):seed_(static_cast<std::uint64_t>(seed)),gamma_(GOLDEN_GAMMA){}
SplittableRandom::SplittableRandom(std::uint64_t seed,std::uint64_t gamma):seed_(seed),gamma_(gamma){}
std::uint64_t SplittableRandom::defaultSeed_() noexcept { auto t=static_cast<std::uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count()); return seedSource.fetch_add(2*GOLDEN_GAMMA,std::memory_order_relaxed)^t; }
std::uint64_t SplittableRandom::nextSeed_() noexcept { return seed_+=gamma_; }
std::uint64_t SplittableRandom::mix64_(std::uint64_t z) noexcept { z=(z^(z>>30))*0xbf58476d1ce4e5b9ULL;z=(z^(z>>27))*0x94d049bb133111ebULL;return z^(z>>31); }
std::uint32_t SplittableRandom::mix32_(std::uint64_t z) noexcept { z=(z^(z>>33))*0x62a9d9ed799705f5ULL;return static_cast<std::uint32_t>(((z^(z>>28))*0xcb24d0a5c88c35b3ULL)>>32); }
std::uint64_t SplittableRandom::mixGamma_(std::uint64_t z) noexcept { z=(z^(z>>33))*0xff51afd7ed558ccdULL;z=(z^(z>>33))*0xc4ceb9fe1a85ec53ULL;z=(z^(z>>33))|1ULL;if(bitCount_(z^(z>>1))<24)z^=0xaaaaaaaaaaaaaaaaULL;return z; }
int SplittableRandom::bitCount_(std::uint64_t value) noexcept { int count=0; while(value){value&=value-1;++count;} return count; }
::jxx::Ptr<SplittableRandom> SplittableRandom::split(){auto value=std::shared_ptr<SplittableRandom>(new SplittableRandom(mix64_(nextSeed_()),mixGamma_(nextSeed_())));return ::jxx::ADOPT(value);}
::jxx::lang::jint SplittableRandom::nextInt(){return static_cast<::jxx::lang::jint>(mix32_(nextSeed_()));}
::jxx::lang::jint SplittableRandom::nextInt(::jxx::lang::jint bound){if(bound<=0)throw ::jxx::lang::IllegalArgumentException();auto r=static_cast<std::uint32_t>(mix32_(nextSeed_()));auto m=static_cast<std::uint32_t>(bound-1);if((bound&m)==0)return static_cast<::jxx::lang::jint>(r&m);std::uint32_t u=r>>1;while(u+m-(u%static_cast<std::uint32_t>(bound))<u)u=mix32_(nextSeed_())>>1;return static_cast<::jxx::lang::jint>(u%static_cast<std::uint32_t>(bound));}
::jxx::lang::jint SplittableRandom::nextInt(::jxx::lang::jint origin,::jxx::lang::jint bound){if(origin>=bound)throw ::jxx::lang::IllegalArgumentException();auto n=static_cast<std::int64_t>(bound)-origin;if(n<=std::numeric_limits<::jxx::lang::jint>::max())return origin+nextInt(static_cast<::jxx::lang::jint>(n));::jxx::lang::jint r;do{r=nextInt();}while(r<origin||r>=bound);return r;}
::jxx::lang::jlong SplittableRandom::nextLong(){return static_cast<::jxx::lang::jlong>(mix64_(nextSeed_()));}
::jxx::lang::jlong SplittableRandom::nextLong(::jxx::lang::jlong bound){if(bound<=0)throw ::jxx::lang::IllegalArgumentException();auto r=mix64_(nextSeed_());auto m=static_cast<std::uint64_t>(bound-1);if((bound&m)==0)return static_cast<::jxx::lang::jlong>(r&m);auto u=r>>1;while(u+m-(u%static_cast<std::uint64_t>(bound))<u)u=mix64_(nextSeed_())>>1;return static_cast<::jxx::lang::jlong>(u%static_cast<std::uint64_t>(bound));}
::jxx::lang::jlong SplittableRandom::nextLong(::jxx::lang::jlong origin,::jxx::lang::jlong bound){if(origin>=bound)throw ::jxx::lang::IllegalArgumentException();auto n=static_cast<std::uint64_t>(bound)-static_cast<std::uint64_t>(origin);if(n<=static_cast<std::uint64_t>(std::numeric_limits<::jxx::lang::jlong>::max()))return origin+nextLong(static_cast<::jxx::lang::jlong>(n));::jxx::lang::jlong r;do{r=nextLong();}while(r<origin||r>=bound);return r;}
::jxx::lang::jdouble SplittableRandom::nextDouble(){return static_cast<::jxx::lang::jdouble>(mix64_(nextSeed_())>>11)*0x1.0p-53;}
::jxx::lang::jdouble SplittableRandom::nextDouble(::jxx::lang::jdouble bound){if(!(bound>0.0))throw ::jxx::lang::IllegalArgumentException();auto r=nextDouble()*bound;return r<bound?r:std::nextafter(bound,-std::numeric_limits<double>::infinity());}
::jxx::lang::jdouble SplittableRandom::nextDouble(::jxx::lang::jdouble origin,::jxx::lang::jdouble bound){if(!(origin<bound))throw ::jxx::lang::IllegalArgumentException();auto r=nextDouble()*(bound-origin)+origin;return r<bound?r:std::nextafter(bound,-std::numeric_limits<double>::infinity());}
::jxx::lang::jbool SplittableRandom::nextBoolean(){return mix32_(nextSeed_())<0x80000000U;}
} // namespace jxx::util
