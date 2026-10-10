#pragma once
#include <cstdint>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util {
class SplittableRandom final : public ::jxx::lang::ClassBase<SplittableRandom, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SplittableRandom, JxxSuper>;
    SplittableRandom();
    explicit SplittableRandom(::jxx::lang::jlong seed);
    ::jxx::Ptr<SplittableRandom> split();
    ::jxx::lang::jint nextInt();
    ::jxx::lang::jint nextInt(::jxx::lang::jint bound);
    ::jxx::lang::jint nextInt(::jxx::lang::jint origin, ::jxx::lang::jint bound);
    ::jxx::lang::jlong nextLong();
    ::jxx::lang::jlong nextLong(::jxx::lang::jlong bound);
    ::jxx::lang::jlong nextLong(::jxx::lang::jlong origin, ::jxx::lang::jlong bound);
    ::jxx::lang::jdouble nextDouble();
    ::jxx::lang::jdouble nextDouble(::jxx::lang::jdouble bound);
    ::jxx::lang::jdouble nextDouble(::jxx::lang::jdouble origin, ::jxx::lang::jdouble bound);
    ::jxx::lang::jbool nextBoolean();
private:
    std::uint64_t seed_;
    std::uint64_t gamma_;
    SplittableRandom(std::uint64_t seed, std::uint64_t gamma);
    std::uint64_t nextSeed_() noexcept;
    static std::uint64_t mix64_(std::uint64_t value) noexcept;
    static std::uint32_t mix32_(std::uint64_t value) noexcept;
    static std::uint64_t mixGamma_(std::uint64_t value) noexcept;
    static std::uint64_t defaultSeed_() noexcept;
    static int bitCount_(std::uint64_t value) noexcept;
};
} // namespace jxx::util
