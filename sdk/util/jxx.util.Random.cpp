#include <chrono>
#include <cmath>
#include <limits>
#include "util/jxx.util.Random.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Exceptions.h"

namespace jxx::util {
namespace {
std::atomic<std::uint64_t> seedUniquifier{8682522807148012ULL};

std::uint64_t nextSeedUniquifier() noexcept {
    std::uint64_t current = seedUniquifier.load(std::memory_order_relaxed);
    for (;;) {
        const std::uint64_t next = current * 181783497276652981ULL;
        if (seedUniquifier.compare_exchange_weak(
                current, next, std::memory_order_relaxed, std::memory_order_relaxed)) {
            return next;
        }
    }
}
}

std::uint64_t Random::initialScramble_(jxx::lang::jlong seed) noexcept {
    return (static_cast<std::uint64_t>(seed) ^ MULTIPLIER) & MASK;
}

jxx::lang::jlong Random::defaultSeed_() noexcept {
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return static_cast<jxx::lang::jlong>(nextSeedUniquifier() ^ static_cast<std::uint64_t>(now));
}

Random::Random() : Random(defaultSeed_()) {}

Random::Random(jxx::lang::jlong seed)
    : seed_(initialScramble_(seed)), nextNextGaussian_(0.0), haveNextNextGaussian_(false) {}

void Random::setSeed(jxx::lang::jlong seed) {
    seed_.store(initialScramble_(seed), std::memory_order_relaxed);
    std::lock_guard<std::mutex> lock(gaussianMutex_);
    haveNextNextGaussian_ = false;
}

jxx::lang::jint Random::next(jxx::lang::jint bits) {
    if (bits < 0 || bits > 32) {
        throw jxx::lang::IllegalArgumentException();
    }

    std::uint64_t oldSeed = seed_.load(std::memory_order_relaxed);
    std::uint64_t nextSeed;
    do {
        nextSeed = (oldSeed * MULTIPLIER + ADDEND) & MASK;
    } while (!seed_.compare_exchange_weak(
        oldSeed, nextSeed, std::memory_order_relaxed, std::memory_order_relaxed));

    return static_cast<jxx::lang::jint>(
        static_cast<std::uint32_t>(nextSeed >> (48 - bits)));
}

void Random::nextBytes(jxx::lang::ByteArray bytes) {
    if (bytes == nullptr) {
        throw jxx::lang::NullPointerException();
    }

    jxx::lang::jint i = 0;
    const auto length = static_cast<jxx::lang::jint>(bytes->length);
    while (i < length) {
        jxx::lang::jint random = nextInt();
        const jxx::lang::jint count = (length - i < 4) ? (length - i) : 4;
        for (jxx::lang::jint n = 0; n < count; ++n) {
            (*bytes)[i++] = static_cast<jxx::lang::jbyte>(random);
            random >>= 8;
        }
    }
}

jxx::lang::jint Random::nextInt() {
    return next(32);
}

jxx::lang::jint Random::nextInt(jxx::lang::jint bound) {
    if (bound <= 0) {
        throw jxx::lang::IllegalArgumentException();
    }

    const jxx::lang::jint r = next(31);
    const jxx::lang::jint m = bound - 1;
    if ((bound & m) == 0) {
        return static_cast<jxx::lang::jint>(
            (static_cast<jxx::lang::jlong>(bound) * static_cast<jxx::lang::jlong>(r)) >> 31);
    }

    jxx::lang::jint u = r;
    jxx::lang::jint candidate;
    do {
        candidate = u % bound;
        const std::uint32_t overflowCheck =
            static_cast<std::uint32_t>(u) - static_cast<std::uint32_t>(candidate) +
            static_cast<std::uint32_t>(m);
        if (static_cast<std::int32_t>(overflowCheck) >= 0) {
            return candidate;
        }
        u = next(31);
    } while (true);
}

jxx::lang::jlong Random::nextLong() {
    const std::uint64_t high = static_cast<std::uint32_t>(next(32));
    const std::uint64_t low = static_cast<std::uint32_t>(next(32));
    return static_cast<jxx::lang::jlong>((high << 32U) | low);
}

jxx::lang::jbool Random::nextBoolean() {
    return static_cast<jxx::lang::jbool>(next(1) != 0);
}

jxx::lang::jfloat Random::nextFloat() {
    return static_cast<jxx::lang::jfloat>(next(24)) / static_cast<jxx::lang::jfloat>(1 << 24);
}

jxx::lang::jdouble Random::nextDouble() {
    const std::uint64_t high = static_cast<std::uint32_t>(next(26));
    const std::uint64_t low = static_cast<std::uint32_t>(next(27));
    return static_cast<jxx::lang::jdouble>((high << 27U) + low) * DOUBLE_UNIT;
}

jxx::lang::jdouble Random::nextGaussian() {
    std::lock_guard<std::mutex> lock(gaussianMutex_);

    if (haveNextNextGaussian_) {
        haveNextNextGaussian_ = false;
        return nextNextGaussian_;
    }

    jxx::lang::jdouble v1;
    jxx::lang::jdouble v2;
    jxx::lang::jdouble s;
    do {
        v1 = 2.0 * nextDouble() - 1.0;
        v2 = 2.0 * nextDouble() - 1.0;
        s = v1 * v1 + v2 * v2;
    } while (s >= 1.0 || s == 0.0);

    const jxx::lang::jdouble multiplier = std::sqrt(-2.0 * std::log(s) / s);
    nextNextGaussian_ = v2 * multiplier;
    haveNextNextGaussian_ = true;
    return v1 * multiplier;
}

void Random::writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) {
    if (out == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    std::lock_guard<std::mutex> lock(gaussianMutex_);
    out->writeLong(static_cast<jxx::lang::jlong>(seed_.load(std::memory_order_relaxed)));
    out->writeDouble(nextNextGaussian_);
    out->writeBoolean(haveNextNextGaussian_);
}

void Random::readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) {
    if (in == nullptr) {
        throw jxx::lang::NullPointerException();
    }
    const auto seed = static_cast<std::uint64_t>(in->readLong());
    const auto gaussian = in->readDouble();
    const auto haveGaussian = in->readBoolean();
    if ((seed & ~MASK) != 0U) {
        throw jxx::lang::IllegalArgumentException();
    }
    seed_.store(seed, std::memory_order_relaxed);
    std::lock_guard<std::mutex> lock(gaussianMutex_);
    nextNextGaussian_ = gaussian;
    haveNextNextGaussian_ = haveGaussian;
}

void Random::readObjectNoData() {
    setSeed(0);
}

} // namespace jxx::util
