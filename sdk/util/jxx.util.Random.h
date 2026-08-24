#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
#include "io/jxx.io.Serializable.h"

namespace jxx::util {

/**
 * C++17/JXX implementation of java.util.Random (Java 8 core API).
 *
 * Uses Java's specified 48-bit linear-congruential generator.
 * Public Java reference types use jxx::Ptr<T>; STL is private only.
 */
class Random : public jxx::lang::Object,
               public virtual jxx::io::Serializable {
public:
    Random();
    Random(jxx::lang::jlong seed);
    virtual ~Random() = default;

    virtual void setSeed(jxx::lang::jlong seed);

protected:
    virtual jxx::lang::jint next(jxx::lang::jint bits);

public:
    virtual void nextBytes(jxx::lang::ByteArray bytes);
    virtual jxx::lang::jint nextInt();
    virtual jxx::lang::jint nextInt(jxx::lang::jint bound);
    virtual jxx::lang::jlong nextLong();
    virtual jxx::lang::jbool nextBoolean();
    virtual jxx::lang::jfloat nextFloat();
    virtual jxx::lang::jdouble nextDouble();
    virtual jxx::lang::jdouble nextGaussian();

    virtual void writeObject(const jxx::Ptr<jxx::io::ObjectOutputStream> out) override;
    virtual void readObject(const jxx::Ptr<jxx::io::ObjectInputStream> in) override;
    virtual void readObjectNoData() override;

private:
    static constexpr std::uint64_t MULTIPLIER = 0x5DEECE66DULL;
    static constexpr std::uint64_t ADDEND = 0xBULL;
    static constexpr std::uint64_t MASK = (1ULL << 48U) - 1ULL;
    static constexpr jxx::lang::jdouble DOUBLE_UNIT = 1.0 / 9007199254740992.0; // 2^-53

    std::atomic<std::uint64_t> seed_;

    mutable std::mutex gaussianMutex_;
    jxx::lang::jdouble nextNextGaussian_ = 0.0;
    jxx::lang::jbool haveNextNextGaussian_ = false;

    static std::uint64_t initialScramble_(jxx::lang::jlong seed) noexcept;
    static jxx::lang::jlong defaultSeed_() noexcept;
};

} // namespace jxx::util
